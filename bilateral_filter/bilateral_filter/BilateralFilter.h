//
//  BilateralFilter.h
//  bilateral_filter
//
//  Created by xyz on 17/1/7.
//  Copyright (c) 2017年 xyz. All rights reserved.
//

#ifndef __bilateral_filter__BilateralFilter__
#define __bilateral_filter__BilateralFilter__

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class BilateralFilter {
private:
    // sigmaD是高斯距离权重的标准差， sigmaC是颜色相似度权重的标准差
    float sigmaD, sigmaC;
    // 模糊半径
    int r;
    // 高斯距离权重表
    float **dw;
    // 颜色相似度权重表
    float *cw;
private:
    void buildDisTable();
    void buildColorTable();
    Mat filterGrayImage(const Mat& image);
    Mat filterColorImage(const Mat& image);
public:
    BilateralFilter(int radius = 3, float sigmaD = 1.0f, float sigmaC = 1.0f);
    Mat filter(const Mat& image);
    Mat gaussianFilter(const Mat &image);
    Mat similarityFilter(const Mat &image);
    
    
};

#endif /* defined(__bilateral_filter__BilateralFilter__) */
