//
//  main.cpp
//  opencv
//
//  Created by 심지훈 on 31/03/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include <string>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include "AdvancedLaneDetection.hpp"
#include "Calibration.hpp"
#include "ObjectDetection.hpp"
#include "Utility.hpp"

//    Order of Pipeline
//    Mat trans = detector.transformingView(undis, BIRDEYE_VIEW);
//    Mat sobel = detector.sobelColorThresholding(trans);
//    Mat curve = detector.windowSearch(sobel);
//    Mat temp = detector.transformingView(curve, NORMAL_VIEW);
//    Mat output = detector.drawPolyArea(img);

string savePath = "/Users/shimjihoon/Desktop/programming/study/opencv";

int main(int argc, const char * argv[])
{
    
    string video = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/finalTest/test.mp4";
    string colorPath = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/colors.txt";
    string labelPath = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/mscoco_labels.names";
    string textGraph = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
    string modelWeights = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/detectionModel/frozen_inference_graph.pb";
    
    string outputPath = "/Users/shimjihoon/Desktop/";
    
    
    Size size(960,540);
    
    
    VideoCapture capture(video);
    
    CV_Assert(capture.isOpened());
    
    AdvnacedLaneDetection detector;
    
    double fps = 30.0;
    int delay = cvRound(1000.0 / fps);
    
    Mat frame;
    Mat originalImg;
    Mat detected;
    
    loadColors(colorPath);
    loadNameOfClasses(labelPath);
    VideoWriter sobel_vid;
//    detected_vid.open(outputPath+"detected.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
//    trans_vid.open(outputPath+"trans.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
//    curve_vid.open(outputPath+"curve.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
//    frame_vid.open(outputPath+"frame.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
//    temp_vid.open(outputPath+"temp.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
    sobel_vid.open(outputPath+"sobel.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, size);
    
    
    while(capture.read(frame)){
        if(waitKey(delay) == 0) break;
        
        resize(frame, frame, size);
        originalImg = frame.clone();
    
        Mat trans = detector.transformingView(frame, BIRDEYE_VIEW);
        Mat sobel = detector.sobelColorThresholding(trans);
        
        rectangle(sobel, Point(300,350), Point(600,540), Scalar(0),-1);
        rectangle(originalImg, Point(300,350), Point(600,540), Scalar(0),-1);
        
        Mat curve = detector.windowSearch(sobel);
        Mat temp = detector.transformingView(curve, NORMAL_VIEW);
        Mat output = detector.drawPolyArea(frame);
        detected = detect(output, textGraph, modelWeights);
    
        
//        imshow("detected", detected);
//        imshow("trans", trans);
//        imshow("curve", curve);
//        imshow("frame", frame);
//        imshow("temp", temp);
//        imshow("sobel", sobel);
        
//        detected_vid.write(detected);
//        trans_vid.write(trans);
//        curve_vid.write(curve);
//        frame_vid.write(frame);
//        temp_vid.write(temp);
        sobel_vid.write(sobel);
    }
}
