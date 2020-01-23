//
//  LaneDetection.hpp
//  opencv
//
//  Created by 심지훈 on 22/05/2019.
//  Copyright © 2019 Shim. All rights reserved.
//
#include <opencv2/opencv.hpp>


#ifndef LaneDetection_hpp
#define LaneDetection_hpp
using namespace cv;
using namespace std;



void draw_houghLine(Mat src, Mat& dst, vector<Vec4i> lanes);
void onMouse(int event, int x, int y, int flags, void* param);
void draw_trapezoid(Mat& dst, Point* pt, const Scalar& color );

Mat region_of_interest(Mat img, const Point* pt, int nPoints, const Scalar& color);
vector< vector<Vec4i> > lineSeperator(vector<Vec4i> lines, Mat img_edges );
vector<Point> regression(vector< vector<Vec4i>> left_right_lines, Mat input_img);
void plotLane(Mat inputImage, vector<Point> lane);


#endif /* LaneDetection_hpp */
