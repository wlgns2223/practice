//
//  AdvancedLaneDetection.cpp
//  opencv
//
//  Created by 심지훈 on 17/07/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "AdvancedLaneDetection.hpp"

void AdvnacedLaneDetection::changeSize(Mat input) {
    
    if(input.size() != this->defaultSize)
        resize(input, input, this->defaultSize);
    
}

Mat AdvnacedLaneDetection::loadImgAndResize(String path, int flag) {
    
    
    Mat temp = imread(path,flag);
    changeSize(temp);
    return temp;
}

AdvnacedLaneDetection::AdvnacedLaneDetection(String path, int flag) {
    img = loadImgAndResize(path,flag);
}

Mat AdvnacedLaneDetection::getImg(){
    return img;
}


Mat AdvnacedLaneDetection::transformingView(Mat input, const int flag){
    //Change perspective from driver view to bird eye view
    
    //Param
    //Input : Mat type Image
    
    //Change     
    changeSize(input);
    
    
    Size warpSize(input.size());
    Mat output(warpSize, input.type());
    Mat transformationMatrix;
    
    switch(flag){
        case BIRDEYE_VIEW:
            // Get Transformation Matrix
            transformationMatrix = getPerspectiveTransform(srcPts, destPts);
            //Warping perspective
            warpPerspective(input, output, transformationMatrix, warpSize, INTER_LINEAR);
            break;
            
        case NORMAL_VIEW:
            transformationMatrix = getPerspectiveTransform(destPts, srcPts);
            warpPerspective(input, output, transformationMatrix, warpSize,INTER_LINEAR);
            break;
        default:
            cerr<<"ERROR: FLAG ERROR\n";
            break;
    }
    
    return output;
    
    
    
}


Mat AdvnacedLaneDetection::sobelThresholding(Mat input, string dir){
    // Sobel Direction (x) and Thresholding
    
    
    // Params
    // Input : L Channel of HLS Img
    // Dir : Sobel Thresholding Direction . {X, Y}
    
    //Output
    //A Mat Img processed by Sobel direction thresholding between
    //range sobelMinThreshold to sobelMaxThreshold
    
    
    Mat sobel;
    Mat sxbinary = Mat::zeros(input.size(), input.type());
    
    // Sobel X Gradient Thresholding
    if(dir == "x")
        Sobel(input, sobel, CV_64F, 1, 0);
    if(dir == "y")
        Sobel(input, sobel, CV_64F, 0, 1);
    
    convertScaleAbs(sobel, sobel);
    
    //Range based thresholding.
    //pixels between sobelMinThreshold and sobelMaxThreshold set to 255, otherwise 0
    inRange(sobel, Scalar(sobelMinThreshold), Scalar(sobelMaxThreshold), sxbinary);
    
    return sxbinary;
        
}

Mat AdvnacedLaneDetection::colorThresholding(Mat input){
    // Color Channel Thresholding
    
    //Param
    //Input : S Channel of HLS Img.
    
    //Output
    //A Mat img processed by Color Thresholding Between
    //colorMinThreshold To colorMaxThreshold
    
    
    Mat s_binary = Mat::zeros(input.size(), input.type());
    
    //Range based thresholding.
    //pixels between colorMinThreshold and colorMaxThreshold set to 255, otherwise 0
    inRange(input, Scalar(colorMinThreshold), Scalar(colorMaxThreshold), s_binary);
    return s_binary;
    
}

Mat AdvnacedLaneDetection::sobelColorThresholding(Mat input){
    // Bitwise operation of sobel threshold img and color threshold img
    
    // Param
    // input :  Mat BGR Color Img
    
    // output
    // A Mat img with bitwise operation of
    // sobel threshold img and color threshold img
    
    changeSize(input);
        
    Mat sxbinary;
    Mat s_binary;
    Mat output;
    Mat splitedColor[3];
    
//    //Convert Color from BGR to HLS channel
//    cvtColor(input, input, COLOR_BGR2HLS);
    
    //Split BGR Color spaces into B, G, R and store them in splitedColor array
    split(input, splitedColor);
    
    Mat imgForSobel = splitedColor[1].clone();
    Mat imgForColor = splitedColor[2].clone();
    
    //Apply sobelThresholding to G channel Image
    sxbinary = sobelThresholding(imgForSobel);
    
    //Apply colorThresholding to R channel Image
    s_binary = colorThresholding(imgForColor);
    
    //Bitwise operation to sum sobelThresholding and colorThresholding Images
    bitwise_and(sxbinary, s_binary, output);
    
    return output;
    
}


Mat AdvnacedLaneDetection::windowSearch(Mat input){
    //find out pixels in left and right windows
    
    //Param
    //input : sobelColorthreshold Mat type image
    
    // Reduce Image (Size(640, 360) to 1 x 360 and sum row elements
    Mat bottom_half_y = input.rowRange(input.size().height / 2, input.size().height );
    Mat hist;
    reduce(bottom_half_y, hist, 0, REDUCE_SUM, CV_32FC1);
    
    
    int midPoint = (int)hist.cols / 2;
    Point leftx_base, rightx_base;
    Mat bottomLeft = hist.colRange(0, midPoint );
    Mat bottomRight = hist.colRange(midPoint , hist.cols);

    //Find out minimum value (most less summed pixels)
    minMaxLoc(bottomLeft, nullptr, nullptr, nullptr, &leftx_base);
    minMaxLoc(bottomRight, nullptr, nullptr, nullptr, &rightx_base);
    
    
    Point leftx_current = leftx_base;
    rightx_base.x += midPoint;
    Point rightx_current = rightx_base;
    
    vector<Point> good_left_idx, good_right_idx;
    Scalar left_mean, right_mean;
    
    // Sum 1d channel image and make 3d channel image
    Mat temp[3] = {input, input, input};
    Mat out;
    cv::merge(temp, 3, out);
    
    int windowHeight = (int)input.size().height / nwindows;
    vector<Point> nonzero;
    //Find out Nonzero pixels and store its position (Point class) to vector
    findNonZero(input, nonzero);
    
    for(int window = 0; window < this->nwindows; ++window){
        //Determine rectangular shape window size
        int win_y_low = input.size().height - ((window + 1 ) * windowHeight);
        int win_y_high = input.size().height - ( window * windowHeight);
        
        int win_xleft_low = leftx_current.x - margin;
        int win_xleft_high = leftx_current.x + margin;
        
        int win_xright_low = rightx_current.x - margin;
        int win_xright_high = rightx_current.x + margin;
        
        rectangle(out, Point(win_xleft_low, win_y_low), Point(win_xleft_high, win_y_high), colors->getColor("green"));
        rectangle(out, Point(win_xright_low, win_y_low), Point(win_xright_high, win_y_high), colors->getColor("green"));
        
        //Index to calculate mean points
        vector<Point>leftIdx, rightIdx;
        
        //find out pixels in window and store in vector
        for(auto _nonzero : nonzero){
            if((_nonzero.y >= win_y_low) && (_nonzero.y < win_y_high) && (_nonzero.x >= win_xleft_low)
               && ( _nonzero.x < win_xleft_high)){
                leftIdx.push_back(_nonzero);        // Index to calculate mean points for window box
                good_left_idx.push_back(_nonzero);  // Index to draw best-fit curve , send it to 'drawCurvelien' as param
            }
            
            if((_nonzero.y >= win_y_low) && (_nonzero.y < win_y_high) && (_nonzero.x >= win_xright_low)
               && ( _nonzero.x < win_xright_high)){
                rightIdx.push_back(_nonzero);
                good_right_idx.push_back(_nonzero);
            }
        }
        left_mean = cv::mean(leftIdx);
        right_mean = cv::mean(rightIdx);
        
        //Change x point position
        if( left_mean != Scalar(0,0,0,0))
            leftx_current = Point(left_mean[0], left_mean[1] );
        
        if( right_mean != Scalar(0,0,0,0) )
            rightx_current = Point(right_mean[0], right_mean[1]);
    }
    
    //draw best-fit curve , polynomial fitting 
    drawCurveline(out, good_left_idx, good_right_idx);
    
    return out;
}

void AdvnacedLaneDetection::drawCurveline(Mat input, vector<Point> leftPt, vector<Point> rightPt){
    //Draw left and right curve fitting to lanes in windows
    
    //Param
    //Input   : Mat type image
    //leftPt  : best-fit pixels that exist in windows of left lane
    //rightPt : best-fit pixels that exist in windows of right lane
    
    //Result
    //Draw a best-fit line of pixels in left or right window
    
    //Store X,Y Points of left and right pixels in window To vector
    vector<float> leftx, lefty, rightx, righty;
    for(auto _good_left : leftPt){
        leftx.push_back(_good_left.x);
        lefty.push_back(_good_left.y);
    }
    
    for(auto _good_right : rightPt){
        rightx.push_back(_good_right.x);
        righty.push_back(_good_right.y);
    }
    
    int leftRow = (int)leftx.size();
    Mat leftxMat(leftRow, 1 , CV_32FC1, leftx.data());
    Mat leftyMat(leftRow, 1, CV_32FC1, lefty.data());
    Mat leftDst(3,1, CV_32FC1 );
    
    // find out coefficients of polynomial equiation
    // OpenCV 2.x ver funciton, now deprecated
    // https://github.com/stonier/opencv2/blob/master/modules/contrib/src/polyfit.cpp
    polyfit(leftyMat, leftxMat, leftDst, 2);
    
    int rightRow = (int)rightx.size();
    Mat rightxMat(rightRow,1, CV_32FC1, rightx.data());
    Mat rightyMat(rightRow, 1, CV_32FC1, righty.data());
    Mat rightDst(3,1, CV_32FC1 );
    polyfit(rightyMat, rightxMat, rightDst, 2);
    
    // Order of Coefficients
    // f(x) = leftCoef[2] * x * x + leftCoef[1] * x + leftCoef[0]
    float leftCoef[3] = {leftDst.at<float>(2,0), leftDst.at<float>(1,0), leftDst.at<float>(0,0)};
    float rightCoef[3] = {rightDst.at<float>(2,0), rightDst.at<float>(1, 0), rightDst.at<float>(0,0) };
    float leftFitx,rightFitx;
    
    
    
    // Calculate x pixel position based on Y ( input Image Height ) and coefficients
    for(int i=0; i<input.size().height; ++i){
        
        leftFitx = leftCoef[0] * i * i + leftCoef[1] * i + leftCoef[2];
        rightFitx = rightCoef[0] * i * i + rightCoef[1] * i + rightCoef[2];
        leftFitPt.push_back(Point(leftFitx, i));
        rightFitPt.push_back(Point(rightFitx, i));
        
        //Left Curve + margin and - margin
        leftFit_windowLine1.push_back(Point(leftFitx- margin, i));
        leftFit_windowLine2.push_back(Point(leftFitx+ margin, i));
        
        //Right Curve + margin and - margin
        rightFit_windowLine1.push_back(Point(rightFitx - margin, i));
        rightFit_windowLine2.push_back(Point(rightFitx + margin, i));
        
    }
    
    
    
    //Draw Window Curves
    drawWindowLine(input);
    
    
}

void AdvnacedLaneDetection::drawWindowLine(Mat input){
    
    //Draw best-fit curves
    polylines(input, leftFitPt, false, colors->getColor("yellow"));
    polylines(input, rightFitPt, false, colors->getColor("yellow"));
    
    //Draw fit curves + margins , - margin
    polylines(input, leftFit_windowLine1, false, colors->getColor("yellow"));
    polylines(input, leftFit_windowLine2, false, colors->getColor("yellow"));
    polylines(input, rightFit_windowLine1, false, colors->getColor("yellow"));
    polylines(input, rightFit_windowLine2, false, colors->getColor("yellow"));
    
    
    Mat windowCurve = Mat::zeros(input.size(), input.type());
    vector<Point> left = leftFit_windowLine1;
    vector<Point> right = rightFit_windowLine1;
    reverse(leftFit_windowLine2.begin(), leftFit_windowLine2.end());
    reverse(rightFit_windowLine2.begin(), rightFit_windowLine2.end());
    left.insert(left.end(), leftFit_windowLine2.begin(), leftFit_windowLine2.end());
    right.insert(right.end(), rightFit_windowLine2.begin(), rightFit_windowLine2.end());
    fillConvexPoly(windowCurve, left, colors->getColor("green"));
    fillConvexPoly(windowCurve, right, colors->getColor("green"));
    addWeighted(input, 1, windowCurve, 0.3, 0, input);
    
    
}

Mat AdvnacedLaneDetection::drawPolyArea(Mat input) {
    
    Mat area = Mat::zeros(input.size(), input.type());
    Mat transformedArea;
    Mat output;
    vector<Point> polyAreaPts = leftFitPt;
    reverse(rightFitPt.begin(), rightFitPt.end());
    polyAreaPts.insert(polyAreaPts.end(), rightFitPt.begin(), rightFitPt.end());
    fillConvexPoly(area, polyAreaPts, colors->getColor("green"));
    transformedArea =  transformingView(area, NORMAL_VIEW);
    addWeighted(input, 1, transformedArea, 0.3, 0, output);
    
    // Clear Class Member Point Vector data
    clearFitPtVec();
    
    return output;
    
}

void AdvnacedLaneDetection::clearFitPtVec(){
    leftFitPt.clear();
    rightFitPt.clear();
    
    leftFit_windowLine1.clear();
    leftFit_windowLine2.clear();
    
    rightFit_windowLine1.clear();
    rightFit_windowLine2.clear();
}

Mat AdvnacedLaneDetection::detectionPipeline(Mat input, Mat img)
{
    Mat trans = transformingView(input, BIRDEYE_VIEW);
    Mat sobel = sobelColorThresholding(trans);
    Mat curve = windowSearch(sobel);
    Mat temp = transformingView(curve, NORMAL_VIEW);
    Mat output = drawPolyArea(img);
    
    return output;
}

void polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order)
{
    /*M///////////////////////////////////////////////////////////////////////////////////////
     //
     //  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
     //
     //  By downloading, copying, installing or using the software you agree to this license.
     //  If you do not agree to this license, do not download, install,
     //  copy or use the software.
     //
     //
     //                           License Agreement
     //                For Open Source Computer Vision Library
     //
     // Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
     // Copyright (C) 2009, Willow Garage Inc., all rights reserved.
     // Third party copyrights are property of their respective owners.
     //
     // Redistribution and use in source and binary forms, with or without modification,
     // are permitted provided that the following conditions are met:
     //
     //   * Redistribution's of source code must retain the above copyright notice,
     //     this list of conditions and the following disclaimer.
     //
     //   * Redistribution's in binary form must reproduce the above copyright notice,
     //     this list of conditions and the following disclaimer in the documentation
     //     and/or other materials provided with the distribution.
     //
     //   * The name of the copyright holders may not be used to endorse or promote products
     //     derived from this software without specific prior written permission.
     //
     // This software is provided by the copyright holders and contributors "as is" and
     // any express or implied warranties, including, but not limited to, the implied
     // warranties of merchantability and fitness for a particular purpose are disclaimed.
     // In no event shall the Intel Corporation or contributors be liable for any direct,
     // indirect, incidental, special, exemplary, or consequential damages
     // (including, but not limited to, procurement of substitute goods or services;
     // loss of use, data, or profits; or business interruption) however caused
     // and on any theory of liability, whether in contract, strict liability,
     // or tort (including negligence or otherwise) arising in any way out of
     // the use of this software, even if advised of the possibility of such damage.
     //
     //M*/
    
    // This original code was written by
    //  Onkar Raut
    //  Graduate Student,
    //  University of North Carolina at Charlotte

    CV_Assert((src_x.rows>0)&&(src_y.rows>0)&&(src_x.cols==1)&&(src_y.cols==1)
              &&(dst.cols==1)&&(dst.rows==(order+1))&&(order>=1));
    Mat X;
    X = Mat::zeros(src_x.rows, order+1,CV_32FC1);
    Mat copy;
    for(int i = 0; i <=order;i++)
    {
        copy = src_x.clone();
        pow(copy,i,copy);
        Mat M1 = X.col(i);
        copy.col(0).copyTo(M1);
    }
    Mat X_t, X_inv;
    transpose(X,X_t);
    Mat temp = X_t*X;
    Mat temp2;
    invert (temp,temp2);
    Mat temp3 = temp2 * X_t;
    Mat W = temp3 * src_y;
    W.copyTo(dst);
}

