//
//  main.cpp
//  bilateral_filter
//
//  Created by xyz on 17/1/7.
//  Copyright (c) 2017年 xyz. All rights reserved.
//

#include <iostream>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "BilateralFilter.h"

int main(int argc, const char * argv[]) {
    Mat image = imread("lena.jpg");
    
    BilateralFilter filter(5, 1.5, 30);
    Mat blur = filter.filter(image);
    imwrite("blur.jpg", blur);
    Mat gauBlur = filter.gaussianFilter(image);
    imwrite("gau_blur.jpg", gauBlur);
    cout << "finish gaussian blur" << endl;
    Mat simiBlur = filter.similarityFilter(image);
    imwrite("simi_blur.jpg", simiBlur);
    cout << "finish similarity blur" << endl;
    
    return 0;
}
