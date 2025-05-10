# 刮痕的檢測

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行刮痕檢測，使用高斯模糊將雜訊抑制
，再以Otsu來給定threshold來使用邊緣檢測方法(Canny)，將bounding rect 畫在原圖上，並計算效能指標（FPS）。

##  編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp scratch_detector.cpp -o scratch_detect `pkg-config --cflags --libs opencv4` -std=c++17 -pthread

```

##  執行方式

將圖片放入 `image/` 資料夾後執行：

```bash
./scratch_detect
```

##  輸出說明
![image](https://github.com/langz824/threaded_opencv_demo/blob/scratch_detect_bench/output.png)
- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS

---
## 2025/05/09 前處理 + Canny + 標註圖片

總共處理  400張圖片
總共耗時  4.75581秒
平均Fps: 84.1077


## 刮傷類瑕疵

總共處理  240張圖片
總共耗時  0.0418177秒
平均Fps: 5739.2
