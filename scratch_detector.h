#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <mutex>

extern std::mutex print_mutex;
void process_image_for_scratch(const std::string& path, int thread_id, int count);