#include "scratch_detector.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>

std::mutex print_mutex;
namespace fs = std::filesystem;

void process_image_for_scratch(const std::string& path, int thread_id, int count) {

    fs::path input_path(path);
    fs::path output_dir = input_path.parent_path().parent_path() /
    (input_path.parent_path().stem().string() + "_output");
    if (!fs::exists(output_dir)) {
        fs::create_directories(output_dir);
    }

    cv::Mat img = cv::imread(path);
    if (img.empty()) {
        std::cerr << "Thread " << thread_id << " failed to read " << path << std::endl;
        return;
    }
    fs::path output_path = output_dir / input_path.filename();

    // 灰階 + 模糊 + 邊緣
    cv::Mat gray, blurred, edges, binary;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);
    // cv::Canny(blurred, edges, 50, 150);

    double otsu_thresh = cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::Canny(gray, edges, otsu_thresh * 0.5, otsu_thresh);  // 更動態的邊緣偵測

    // 找輪廓並框起來
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {
        if (cv::contourArea(contour) > 100) {
            cv::rectangle(img, cv::boundingRect(contour), cv::Scalar(0, 0, 255), 2);
        }
    }
    // std::cout << "Thread " << thread_id << " \nprocessed: " << path << std::endl;
    // 避免在cout過程打架
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "[Thread " << thread_id << "] #" << count << ": " << path << std::endl;
    }

    // imwrite 但檢查能不能成功
    if (!cv::imwrite(output_path.string(), img)) {
    std::cerr << "Thread " << thread_id << " failed to write: " << output_path << std::endl;
    }
}