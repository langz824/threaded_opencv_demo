# 刮痕的檢測

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行刮痕檢測，使用高斯模糊將雜訊抑制
，再使用邊緣檢測方法，將bounding rect 以xml紀錄，並計算效能指標（FPS）。

##  編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp -o scratch_detect `pkg-config --cflags --libs opencv4` -std=c++17 -pthread
```

##  執行方式

將圖片放入 `image/` 資料夾後執行：

```bash
./scratch_detect
```

##  輸出說明

- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS

---
