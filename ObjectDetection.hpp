//
//  ObjectDetection.hpp
//  opencv
//
//  Created by 심지훈 on 02/10/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#ifndef ObjectDetection_hpp
#define ObjectDetection_hpp

#include "detectionHeader.h"

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <iostream>

void loadNameOfClasses(string classesFilePath);
void loadColors(string colorFilePath);

void drawBox(Mat& frame, int classId, float conf,Rect box, Mat& objectMask);
void postprocess(Mat& frame, const vector<Mat>& outs);


Mat detect(Mat& input, string textGraph, string modelWeights);

#endif /* ObjectDetection_hpp */
