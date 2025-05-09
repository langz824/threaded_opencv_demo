#include <opencv2/opencv.hpp>
#include <safe_queue.h>
#include <string>
#include <iostream>

#include <thread>


void worker(SafeQueue<std::string>& imageQueue) {
    std::string path;

    while (true) {
        if(!imageQueue.pop(path)) break;
        if(path == "EXIT") break;

        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Fail to read image: " << path << std::endl;
            continue;
        }

        cv::Mat result;
        cv::GaussianBlur(img, result, cv::Size(5,5), 1.5);

        std::cout << "Thread " << std::this_thread::get_id()
                  << "Processed : " << path << std::endl;

    }
    
    std::cout << "Thread " << std::this_thread::get_id() << "exiting. \n";

}