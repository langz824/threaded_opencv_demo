# 夾雜物 inclusion 的檢測

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行針對這類瑕疵的目標框選
，最後將bounding box畫在原圖上，並計算效能指標（FPS）。

##  編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp inclusion_detector.cpp -o inclusion_detect `pkg-config --cflags --libs opencv4` -std=c++17 -pthread


```

##  執行方式
![image](https://github.com/langz824/threaded_opencv_demo/blob/inclusion_detector/inclusion.png)
將圖片放入 `image/` 資料夾後執行：

```bash
./inclusion_detect
```
##  方法介紹
- 透過**CLAHE 對比增強**提升局部亮度細節，改善某些目標物顏色較淡問題

- 使用**高斯模糊**去除雜訊，減少整體雜訊干擾

- 採用**Adaptive threshold** 進行局部閾值來降低光照不均造成的false positive(雜訊誤判為目標)

- 結合**形態學Opening與Closing**去除雜點並填補斷裂區域

- 利用輪廓偵測與篩選條件（面積、長寬比、緊密度）濾除非瑕疵區域

- 將有效缺陷以紅色矩形框標示於原圖上
##  輸出說明
![image](https://github.com/langz824/threaded_opencv_demo/blob/inclusion_detector/inclusion_output.png)
- image_output/ 為在原圖上畫框標示出檢測結果
- debug_output/ 顯示圖片在畫bounding box 之前利用threshold劃分出的目標區域
- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS

[output-google雲端](<https://drive.google.com/drive/folders/14qnqhehRLgPCXoHlXTpeIv2SV17BuaAg?hl=zh-TW>)