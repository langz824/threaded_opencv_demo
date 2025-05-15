# patches的檢測

本專案使用 C++ 搭配 OpenCV 與多執行緒對 `image/` 資料夾中的圖片進行針對這類圖形的斑塊框選
，最後將bounding box畫在原圖上，並計算效能指標（FPS）。

##  編譯方式

需先安裝 OpenCV（4.x）：

```bash
g++ main.cpp patches_detector.cpp -o patches_detect `pkg-config --cflags --libs opencv4` -std=c++17 -pthread


```

##  執行方式
![image](https://github.com/langz824/threaded_opencv_demo/blob/patches_detector/patches.png)
將圖片放入 `image/` 資料夾後執行：

```bash
./patches_detect
```
##  方法介紹
- 透過**CLAHE 對比增強**提升局部亮度和紋理細節，改善光照不均問題

- 使用**高斯模糊**去除雜訊，強化目標邊緣穩定性

- 採用**Otsu 自動閾值法**並搭配調整後閾值進行二值化，分離亮區暗區

- 結合**形態學Closing**去除小孔洞並填補斷裂區域

- 利用輪廓偵測與篩選條件（面積、長寬比、緊密度）濾除非瑕疵區域 

- 將有效缺陷以紅色矩形框標示於原圖上
##  輸出說明

![image](https://github.com/langz824/threaded_opencv_demo/blob/patches_detector/patches.output.png)
- image_output/ 為在原圖上畫框標示出檢測結果
- debug_output/ 顯示圖片在畫bounding box 之前利用threshold劃分出的目標區域
- 每張圖會顯示處理它的執行緒 ID
- 統計處理總張數、總耗時與平均 FPS


##  需要改善問題
- 若同張圖發生光暗強度有分布差異，會把瑕疵連同整個暗區也都當作瑕疵
- 斑塊站比較大，用長方形框選區愈容易發生框到一大片不是瑕疵的區域
- 雖存在以上問題，但該方法框選區域還是都有包含瑕疵，只是bounding box 會多框到比較多不屬於瑕疵範圍的問題存在

![image](https://github.com/langz824/threaded_opencv_demo/blob/patches_detector/patches_mask.png)