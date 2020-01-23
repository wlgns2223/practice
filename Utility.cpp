//
//  Utility.cpp
//  opencv
//
//  Created by 심지훈 on 02/10/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "Utility.hpp"

void mouseOnClick(int event, int x, int y, int flags, void* userdata){
    if(event == EVENT_LBUTTONDOWN)
        cout<<"( "<<x<<", "<<y<<" )\n";
}

void draw_histo(Mat hist, Mat &hist_img,int channel, Size size){
    hist_img = Mat(size, channel, Scalar(255));
    float bin = (float)hist_img.cols / hist.rows;
    normalize(hist, hist, 0 ,hist_img.rows, NORM_MINMAX);
    
    for(int i=0; i<hist.rows; ++i){
        float start_x = i * bin;
        float end_x = (i+1) * bin;
        Point2f pt1(start_x,0);
        Point2f pt2(end_x, hist.at<float>(i));
        
        if(pt2.y > 0)
            rectangle(hist_img,pt1, pt2, Scalar(0), -1);
    }
    
    flip(hist_img, hist_img, 0);
}


