//
//  BilateralFilter.cpp
//  bilateral_filter
//
//  Created by xyz on 17/1/7.
//  Copyright (c) 2017年 xyz. All rights reserved.
//

#include "BilateralFilter.h"
#include <cmath>
#include <cassert>

BilateralFilter::BilateralFilter(int radius, float sigmaD, float sigmaC)
: sigmaD(sigmaD), sigmaC(sigmaC), r(radius) {
    this->dw = new float*[2*radius+1];
    this->cw = new float[256];
    for (int i = 0; i < 2*radius+1; i++) {
        this->dw[i] = new float[2*radius+1];
    }
    buildDisTable();
    buildColorTable();
}

void BilateralFilter::buildColorTable() {
    for (int i = 0; i < 256; i++) {
        cw[i] = exp(-0.5*(i*i)/(sigmaC*sigmaC));
    }
}

void BilateralFilter::buildDisTable() {
    for (int i = -r; i <= r; i++) {
        for (int j = -r; j <= r; j++) {
            dw[i+r][j+r] = exp(-0.5*(i*i+j*j) / (sigmaD*sigmaD));
        }
    }
    
}

Mat BilateralFilter::filterGrayImage(const cv::Mat &image) {
    Mat ret;
    image.copyTo(ret);
    for (int row = r; row < image.rows-r; row++) {
        for (int col = r; col < image.cols-r; col++) {
            float weightSum = 0;
            float pixelSum = 0;
            for (int i = -r; i <= r; i++) {
                for (int j = -r; j <= r; j++) {
                    uchar neighbor = image.at<uchar>(row+i, col+j);
                    uchar cur = image.at<uchar>(row, col);
                    float weight = dw[i+r][j+r] * cw[abs(neighbor-cur)];
                    pixelSum += cur * weight;
                    weightSum += weight;
                }
            }
            ret.at<uchar>(row, col) = (uchar)(image.at<uchar>(row, col) / weightSum);
        }
    }
    return ret;
}

Mat BilateralFilter::filterColorImage(const cv::Mat &image) {
    assert(image.channels() == 3);
    Mat ret;
    image.copyTo(ret);
    for (int row = r; row < image.rows-r; row++) {
        for (int col = r; col < image.cols-r; col++) {
            
            float weightSum[3] = {0,};
            float pixelSum[3] = {0,};
            Vec3b cur = image.at<Vec3b>(row, col);
            for (int i = -r; i <= r; i++) {
                for (int j = -r; j <= r; j++) {
                    // RGB channel
                    Vec3b neighbor = image.at<Vec3b>(row+i, col+j);
                    for (int c = 0; c < image.channels(); c++) {
                        float weight = dw[i+r][j+r] * cw[abs(neighbor[c]-cur[c])];
                        pixelSum[c] += neighbor[c] * weight;
                        weightSum[c] += weight;
                    }
                }
            }
            for (int c = 0; c < image.channels(); c++) {
                cur[c] = pixelSum[c] / weightSum[c];
            }
            ret.at<Vec3b>(row, col) = cur;
            
        }
    }
    return ret;
}



Mat BilateralFilter::filter(const Mat& image) {
    if (image.channels() == 1) {
        cout << "blur gray image" << endl;
        return filterGrayImage(image);
    } else {
        cout << "blur color image" << endl;
        return filterColorImage(image);
    }
}

Mat BilateralFilter::gaussianFilter(const Mat &image) {
    Mat ret;
    image.copyTo(ret);
    for (int row = r; row < image.rows-r; row++) {
        for (int col = r; col < image.cols-r; col++) {
            float weightSum[3] = {0,};
            float pixelSum[3] = {0,};
            Vec3b cur = image.at<Vec3b>(row, col);
            for (int i = -r; i <= r; i++) {
                for (int j = -r; j <= r; j++) {
                    Vec3b neighbor = image.at<Vec3b>(row+i, col+j);
                    for (int c = 0; c < image.channels(); c++) {
                        weightSum[c] = weightSum[c] + dw[i+r][j+r];
                        pixelSum[c] += (neighbor[c] * dw[i+r][j+r]);
                    }
                }
            }
            for (int i = 0; i < 3; i++) {
                cur[i] = pixelSum[i] / weightSum[i];
            }
            ret.at<Vec3b>(row, col) = cur;
        }
    }
    
    return ret;
}


Mat BilateralFilter::similarityFilter(const Mat &image) {
    Mat ret;
    image.copyTo(ret);
    for (int row = r; row < image.rows-r; row++) {
        for (int col = r; col < image.cols-r; col++) {
            float weightSum[3] = {0,};
            float pixelSum[3] = {0,};
            Vec3b cur = image.at<Vec3b>(row, col);
            for (int i = -r; i <= r; i++) {
                for (int j = -r; j <= r; j++) {
                    Vec3b neighbor = image.at<Vec3b>(row+i, col+j);
                    for (int c = 0; c < image.channels(); c++) {
                        pixelSum[c] += neighbor[c] * cw[abs(neighbor[c]-cur[c])];
                        weightSum[c] += cw[abs(neighbor[c]-cur[c])];
                    }
                }
            }
            for (int c = 0; c < image.channels(); c++) {
                cur[c] = pixelSum[c] / weightSum[c];
            }
            ret.at<Vec3b>(row, col) = cur;
        }
    }
    return ret;
}




