//
//  AdvancedLaneDetection.hpp
//  opencv
//
//  Created by 심지훈 on 17/07/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#ifndef AdvancedLaneDetection_hpp
#define AdvancedLaneDetection_hpp
#include "detectionHeader.h"
#include "ROIData.hpp"
#include "ColorData.hpp"

#define BIRDEYE_VIEW 0
#define NORMAL_VIEW  1

class AdvnacedLaneDetection {
    
private:
    Mat img;
    
    // Refactoring -- Data
    vector<Point2f> srcPts = {
        Point2f(110,540), Point2f(430,330),
        Point2f(530,330), Point2f(890,540)
    };

    vector<Point2f> destPts = {
        Point2f(110,540), Point2f(110, 0),
        Point2f(800,0), Point2f(800,540)
    };
    
//    Scalar GREEN  = Scalar(0,255,0);
//    Scalar BLUE   = Scalar(255,0,0);
//    Scalar RED    = Scalar(0,0,255);
//    Scalar YELLOW = Scalar(0,255,255);
    
    ColorData* colors;
    ROIData* ROI;
    
    
    

    vector<Point> leftFitPt, rightFitPt;
    vector<Point> leftFit_windowLine1,leftFit_windowLine2;
    vector<Point> rightFit_windowLine1, rightFit_windowLine2;
    
    //Original Sample Size Size(640, 360)
    const Size defaultSize = Size(960,540);
    
    
    
    const int sobelMinThreshold = 30;
    const int sobelMaxThreshold = 255;
    
    const int colorMinThreshold = 100;
    const int colorMaxThreshold = 255;
    
    
    const int nwindows = 9;
    const int margin = 50;
    
    
    // Refactoring -- Editing
    Mat loadImgAndResize(String path, int flag = IMREAD_COLOR);
    void changeSize(Mat input);
    
    
    
    void drawCurveline(Mat input, vector<Point> leftPt, vector<Point> rightPt);
    void drawWindowLine(Mat input);
    Mat sobelThresholding(Mat input, string dir = "x");
    Mat colorThresholding(Mat input);
    void clearFitPtVec();

public:
    AdvnacedLaneDetection();
    AdvnacedLaneDetection(ROIData* ROI, ColorData* colorData);
    AdvnacedLaneDetection(String path, int flag = IMREAD_COLOR);
    
    Mat getImg();
    
    //Change view perspective from normal to bird eye view
    Mat transformingView(Mat input, const int flag);
    
    //apply sobel edge and thresholding
    //Refactoring -- will split function into two
    Mat sobelColorThresholding(Mat input);
    
    //find out pixels(Point) in windows and draw left and right curve fitting to lanes
    Mat windowSearch(Mat input);
    
    // Refactoring -- Editing
    Mat drawPolyArea(Mat input);
    
    Mat detectionPipeline(Mat input, Mat img);
        
};

void polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order);
#endif /* AdvancedLaneDetection_hpp */
