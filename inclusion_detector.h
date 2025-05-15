#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <mutex>

extern std::mutex print_mutex;

void process_image_for_inclusion(const std::string& path, int thread_id, int count);