#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue {
private:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;

public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(m);
        q.push(value);
        cv.notify_one();
    }

    bool pop(T& result) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] {return !q.empty();});
        result = q.front();
        q.pop();
        return true;
    }

/*     cv.wait(lock, [&] {return !q.empty;}); 
*  <=> while(q.empty()) {
          cv.wait(lock);
        }
*/

    void empty() {
        std::lock_guard<std::mutex> lock(m);
        return q.empty;
    }
};