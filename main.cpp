#include <opencv2/opencv.hpp>
#include "safe_queue.h"
#include "patches_detector.h"
#include <chrono>
#include <atomic>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <thread>


void worker(SafeQueue<std::string>& imageQueue, std::atomic<int>& counter, int thread_id) {
    std::string path;
    
    while (true) {
        if(!imageQueue.pop(path)) break;
        if(path == "EXIT") break;
        int localCount = ++counter;
        process_image_for_patches(path, thread_id, localCount); // 這邊換成處理scale的
    }
    
    // std::cout << "Thread " << std::this_thread::get_id() << "exiting. \n";
    // std::cout << "Worker #" << thread_id << "  processing: " << path << std::endl;
    
}

namespace fs = std::filesystem;

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::atomic<int> totalProcess = 0;
    SafeQueue<std::string> imageQueue;
    const std::string folder = "patches";
    const int numThreads = 4;

    // 遍歷folder 內所有檔案並把路徑內檔案放到imageQueue裡排隊
    for (const auto& entry : fs::directory_iterator(folder)) {
        if(entry.is_regular_file()) {
            std::string path = entry.path().string();
            imageQueue.push(path);
        }
    }

    std::vector<std::thread> workers;
    for (int i = 0; i < numThreads; ++i) {
        workers.emplace_back(worker, std::ref(imageQueue), std::ref(totalProcess), i);
    }
    
    for (int i=0;i < numThreads; ++i) {
        imageQueue.push("EXIT");
    }

    for (auto&  t : workers) {
        t.join();
    }

    std::cout << "圖片處理完畢" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "==============================" << std::endl;
    std::cout << "總共處理  " << totalProcess << "張圖片\n";
    std::cout << "總共耗時  " << elapsed.count() << "秒\n";
    std::cout << "平均Fps: " << totalProcess / elapsed.count() << std::endl;
    std::cout << "==============================" << std::endl;
    return 0;
}

