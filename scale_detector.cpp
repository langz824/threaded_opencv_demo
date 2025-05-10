#include "scale_detector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <filesystem>
#include <iostream>

std::mutex print_mutex;
namespace fs = std::filesystem;

bool is_valid_defect(const std::vector<cv::Point>& contour) {
    cv::Rect rect = cv::boundingRect(contour);
    float aspect_ratio = static_cast<float>(rect.width) / rect.height; // 過寬或過扁排除
    float area = cv::contourArea(contour); // 太小雜點 & 太大背景排除
    float perimeter = cv::arcLength(contour, true); // 周長
    float compactness = (perimeter * perimeter) / (4 * CV_PI * area); // 越接近 1 越像圓形
    /*return (
        area > 30 &&                  // 去除雜點
        area < 1000 &&               // 過大框是背景誤判
        aspect_ratio < 5.0f &&       // 防止框成一條長線
        compactness < 10.0f            // 防止亂七八糟的抖動邊緣
    );*/
    return (
        area > 50 &&                  // 去除雜點
        area < 1000 &&               // 過大框是背景誤判
        aspect_ratio < 3.0f &&       // 防止框成一條長線
        compactness < 6.0f            // 防止亂七八糟的抖動邊緣
    );

}

void process_image_for_scale(const std::string& path, int thread_id, int count) {
    
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

    // 灰階 + Clahe等化 + 模糊 + 邊緣
    cv::Mat gray, enhanced, blurred, binary;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    if (gray.empty()) {
        std::cerr << "Thread " << thread_id << " gray conversion failed: " << path << std::endl;
        return;
    }
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4.0);
    clahe->apply(gray, enhanced);

    cv::GaussianBlur(enhanced, blurred, cv::Size(9, 9), 2.5); 
    // cv::threshold(blurred, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU); 由otsu去根據灰階分布決定thresh反而會導致被打光的部分被強調
    // cv::threshold(blurred, binary, 80, 255, cv::THRESH_BINARY_INV); 固定threshold注定失敗
    /* 更慘
    cv::adaptiveThreshold(blurred, binary, 255,
                        cv::ADAPTIVE_THRESH_MEAN_C,  // 或 GAUSSIAN_C
                        cv::THRESH_BINARY_INV,
                        25,   // blockSize，區域大小（奇數，越大越平滑）
                        5);   // C，局部平均後減掉的值（越大越嚴格）
    */

    /*
    double otsu_thresh = cv::threshold(blurred, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    cv::threshold(blurred, binary, otsu_thresh +10, 255, cv::THRESH_BINARY_INV);  // +10 → 調高門檻
    */

    double otsu_thresh = cv::threshold(blurred, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    double final_thresh = std::max(otsu_thresh - 20, 80.0);  // 設定最小下限
    cv::threshold(blurred, binary, final_thresh, 255, cv::THRESH_BINARY_INV);
    cv::Mat morph;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    //cv::morphologyEx(binary, morph, cv::MORPH_CLOSE, kernel);
    // 由Close 改為 Open+Close Morphology 連結仍導致「小斑點被合併進雜區」
    cv::Mat open_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::Mat close_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(binary, morph, cv::MORPH_OPEN, open_kernel);
    cv::morphologyEx(morph, morph, cv::MORPH_CLOSE, close_kernel);
    // 找輪廓並框起來
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morph, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    
    for (const auto& contour : contours) {
        if (is_valid_defect(contour)) {
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

    fs::path debug_dir = output_dir.parent_path() / "debug_output";
    if (!fs::exists(debug_dir)) {
        fs::create_directories(debug_dir);
    }
    fs::path debug_morph_path = debug_dir / (input_path.stem().string() + ".jpg");
    cv::imwrite(debug_morph_path.string(), morph);
}