# rolled-in scale的檢測

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行針對這類圖形的斑塊框選
，最後將bounding box畫在原圖上，並計算效能指標（FPS）。

##  編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp scale_detector.cpp -o scale_detect `pkg-config --cflags --libs opencv4` -std=c++17 -pthread

```

##  執行方式
![image](https://github.com/langz824/threaded_opencv_demo/blob/scale_detector/scale.png)
將圖片放入 `image/` 資料夾後執行：

```bash
./scale_detect
```
##  方法介紹
- 透過**CLAHE 對比增強**提升局部亮度細節，改善光照不均問題

- 使用**高斯模糊**去除雜訊，強化目標邊緣穩定性

- 採用**Otsu 自動閾值法**決定初步門檻值，並手動下調20以避免過亮區域判斷為瑕疵

- 結合**形態學Opening（2×2）與Closing（3×3）** 去除雜點並填補斷裂區域

- 利用輪廓偵測與篩選條件（面積、長寬比、緊密度）濾除非瑕疵區域，面積 > 50 且 < 1000 排除過小雜點與過大背景，長寬比 < 3（排除狹長形），緊密度（compactness）< 6（排除邊緣過度抖動或破碎）

- 將有效缺陷以紅色矩形框標示於原圖上
##  輸出說明
![image](https://github.com/langz824/threaded_opencv_demo/blob/scale_detector/scale_output.png)
- image_output/ 為在原圖上畫框標示出檢測結果
- debug_output/ 顯示圖片在畫bounding box 之前利用threshold劃分出的目標區域
- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS

[圖片展示](<https://drive.google.com/drive/folders/1JLs98PNOchkLnGknqp4t-tphvkQ02PBz?hl=zh-TW>)