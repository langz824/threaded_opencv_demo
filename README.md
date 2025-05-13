# FPS Benchmark (Multithreaded)

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行高斯模糊處理，並計算效能指標（FPS）。

## 編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp -o main `pkg-config --cflags --libs opencv4` -std=c++17 -pthread
```

##  執行方式

將圖片放入 `image/` 資料夾後執行：

```bash
./main
```

##  輸出說明

- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS

---

##  2025/05/09 僅讀入高斯模糊後就刪掉


總共處理  400張圖片
總共耗時  4.64961秒
平均Fps: 86.0287


##  刮傷類瑕疵

總共處理  240張圖片
總共耗時  0.0680839秒
平均Fps: 3525.06
