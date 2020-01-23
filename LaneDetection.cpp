//
//  LaneDetection.cpp
//  opencv
//
//  Created by 심지훈 on 22/05/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "LaneDetection.hpp"

bool right_flag = false;
bool left_flag = false;
double img_center;

void onMouse(int event, int x, int y, int flags, void* param)
{
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            cout<<"(x, y ) = ("<<x<<","<<y<<")"<<endl;
            break;
    }
}

void draw_trapezoid(Mat& dst, Point* pt, const Scalar& color )
{
    if(dst.channels() == 1) {
        cvtColor(dst, dst, COLOR_GRAY2BGR);
    }
    
    
    line(dst, pt[0], pt[1], color);
    line(dst, pt[1], pt[2], color);
    line(dst, pt[2], pt[3], color);
    line(dst, pt[3], pt[0], color);
    
}

Mat region_of_interest(Mat img, const Point* pt, int nPoints, const Scalar& color)
{
    Mat output;
    Mat mask(img.rows, img.cols, CV_8U,Scalar(0));
    fillConvexPoly(mask, pt, 4, color);
    bitwise_and(img, mask, output);
    return output;
}


vector<std::vector<cv::Vec4i> > lineSeperator(std::vector<cv::Vec4i> lines, cv::Mat img_edges)
{
    vector<std::vector<cv::Vec4i> > output(2);
    size_t j = 0;
    Point ini;
    Point fini;
    double slope_thresh = 0.3;
    vector<double> slopes;
    vector<cv::Vec4i> selected_lines;
    vector<cv::Vec4i> right_lines, left_lines;
    
    // Calculate the slope of all the detected lines
    for (auto i : lines) {
        ini = Point(i[0], i[1]);
        fini = Point(i[2], i[3]);
        
        // Basic algebra: slope = (y1 - y0)/(x1 - x0)
        double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y))/(static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);
        
        // If the slope is too horizontal, discard the line
        // If not, save them  and their respective slope
        if (std::abs(slope) > slope_thresh) {
            slopes.push_back(slope);
            selected_lines.push_back(i);
        }
    }
    
    // Split the lines into right and left lines
    img_center = static_cast<double>((img_edges.cols / 2));
    while (j < selected_lines.size()) {
        ini = Point(selected_lines[j][0], selected_lines[j][1]);
        fini = Point(selected_lines[j][2], selected_lines[j][3]);
        
        // Condition to classify line as left side or right side
        if (slopes[j] > 0 && fini.x > img_center && ini.x > img_center) {
            right_lines.push_back(selected_lines[j]);
            right_flag = true;
        } else if (slopes[j] < 0 && fini.x < img_center && ini.x < img_center) {
            left_lines.push_back(selected_lines[j]);
            left_flag = true;
        }
        j++;
    }
    
    output[0] = right_lines;
    output[1] = left_lines;
    
    return output;
}


std::vector<cv::Point> regression(std::vector<std::vector<cv::Vec4i> > left_right_lines, cv::Mat inputImage) {
    vector<cv::Point> output(4);
    Point ini;
    Point fini;
    Point ini2;
    Point fini2;
    Vec4d right_line;
    Vec4d left_line;
    vector<cv::Point> right_pts;
    vector<cv::Point> left_pts;
    double right_m=0, left_m=0;
    Point right_b, left_b;
    
    
    // If right lines are being detected, fit a line using all the init and final points of the lines
    if (right_flag == true) {
        for (auto i : left_right_lines[0]) {
            ini = cv::Point(i[0], i[1]);
            fini = cv::Point(i[2], i[3]);
            right_pts.push_back(ini);
            right_pts.push_back(fini);
            
        }
        
        if (right_pts.size() > 0) {
            // The right line is formed here
            cv::fitLine(right_pts, right_line, DIST_L2, 0, 0.01, 0.01);
            right_m = right_line[1] / right_line[0];
            right_b = cv::Point(right_line[2], right_line[3]);
        }
        
    } else {
        cout<<"No right lane"<<endl;
    }
    // If left lines are being detected, fit a line using all the init and final points of the lines
    if (left_flag == true) {
        for (auto j : left_right_lines[1]) {
            ini2 = cv::Point(j[0], j[1]);
            fini2 = cv::Point(j[2], j[3]);
            left_pts.push_back(ini2);
            left_pts.push_back(fini2);
            
        
        }
        
        if (left_pts.size() > 0) {
            // The left line is formed here
            cv::fitLine(left_pts, left_line, DIST_L2, 0, 0.01, 0.01);
            left_m = left_line[1] / left_line[0];
            left_b = cv::Point(left_line[2], left_line[3]);
        }
    } else {
        cout<<"NO left lane"<<endl;
    }
    // One the slope and offset points have been obtained, apply the line equation to obtain the line points
    int ini_y = inputImage.rows ;
    int fin_y = 360;
    
    double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
    double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;
    

    double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
    double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
    
    cout<<endl;
    cout<<"right_ini_x = "<<right_ini_x<<endl;
    cout<<"right_fin_x = "<<right_fin_x<<endl;
    cout<<"left_ini_x = "<<left_ini_x<<endl;
    cout<<"left_fin_x = "<<left_fin_x<<endl;
    
    
    
    output[0] = cv::Point(right_ini_x, ini_y);
    output[1] = cv::Point(right_fin_x, fin_y);
    output[2] = cv::Point(left_ini_x, ini_y);
    output[3] = cv::Point(left_fin_x, fin_y);
    
    
    
    return output;
}


void plotLane(Mat inputImage, vector<Point> lane)
{
    vector<Point> poly_points;
    Mat output;
    inputImage.copyTo(output);
    
    poly_points.push_back(lane[2]);
    poly_points.push_back(lane[0]);
    poly_points.push_back(lane[1]);
    poly_points.push_back(lane[3]);
    

    fillConvexPoly(output, poly_points, Scalar(0,255,0));
    addWeighted(output, 0.3, inputImage, 1.0, -0.3, inputImage);
    
    line(inputImage, lane[0], lane[1], Scalar(0,0,255),2, LINE_AA);
    line(inputImage, lane[2], lane[3], Scalar(0,0,255),2, LINE_AA);
    
    imshow("image", inputImage);

}
