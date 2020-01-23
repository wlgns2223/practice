//
//  ObjectDetection.cpp
//  opencv
//
//  Created by 심지훈 on 02/10/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "ObjectDetection.hpp"

vector<string> classes;
vector<Scalar> colors;

float confThreshold = 0.6;
float maskThreshold = 0.3;

//Object Detection Path
string classesFile = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/mscoco_labels.names";
string colorsFile = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/colors.txt";
string textGraph = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/mask_rcnn_inception_v2_coco_2018_01_28.pbtxt";
string modelWeights = "/Users/shimjihoon/Desktop/programming/study/opencv/MaskRCNN/detectionModel/frozen_inference_graph.pb";


// Runs the forward pass to get output from the output layers
vector<String> outNames(2);
vector<Mat> outs;


void loadNameOfClasses(string classesFilePath){
    //Load names of classes
    string line;
    ifstream ifs(classesFilePath.c_str());
    while(getline(ifs,line)) classes.push_back(line);
}

void loadColors(string colorFilePath){
    //Load the colors
    string line;
    ifstream colorFptr(colorFilePath.c_str());
    while(getline(colorFptr, line)){
        char* pEnd;
        double r,g,b;
        r = strtod(line.c_str(), &pEnd);
        g = strtod(pEnd, NULL);
        b = strtod(pEnd, NULL);
        Scalar color = Scalar(r,g,b, 255.0);
        colors.push_back(color);
    }
}

// Draw the predicted bounding box, colorize and show the mask on the image
void drawBox(Mat& frame, int classId, float conf, Rect box, Mat& objectMask)
{
    //Draw a rectangle displaying the bounding box
    rectangle(frame, Point(box.x, box.y), Point(box.x+box.width, box.y+box.height), Scalar(255, 178, 50), 3);
    
    //Get the label for the class name and its confidence
    string label = format("%.2f", conf);
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ":" + label;
    }
    
    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    box.y = max(box.y, labelSize.height);
    rectangle(frame, Point(box.x, box.y - round(1.5*labelSize.height)), Point(box.x + round(1.5*labelSize.width), box.y + baseLine), Scalar(255, 255, 255), FILLED);
    putText(frame, label, Point(box.x, box.y), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);

    Scalar color = colors[classId%colors.size()];
    
    // Resize the mask, threshold, color and apply it on the image
    resize(objectMask, objectMask, Size(box.width, box.height));
    Mat mask = (objectMask > maskThreshold);
    Mat coloredRoi = (0.3 * color + 0.7 * frame(box));
    coloredRoi.convertTo(coloredRoi, CV_8UC3);

    // Draw the contours on the image
    vector<Mat> contours;
    Mat hierarchy;
    mask.convertTo(mask, CV_8U);
    findContours(mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    drawContours(coloredRoi, contours, -1, color, 5, LINE_8, hierarchy, 100);
    coloredRoi.copyTo(frame(box), mask);

}

void postprocess(Mat& frame, const vector<Mat>& outs)
{
    Mat outDetections = outs[0];
    Mat outMasks = outs[1];
    
    // Output size of masks is NxCxHxW where
    // N - number of detected boxes
    // C - number of classes (excluding background)
    // HxW - segmentation shape
    
    const int numDetections = outDetections.size[2];
    const int numClasses = outMasks.size[1];
    
    outDetections = outDetections.reshape(1, outDetections.total() / 7);
    for (int i = 0; i < numDetections; ++i)
    {
        float score = outDetections.at<float>(i, 2);
        if (score > confThreshold)
        {
            // Extract the bounding box
            int classId = static_cast<int>(outDetections.at<float>(i, 1));
            int left = static_cast<int>(frame.cols * outDetections.at<float>(i, 3));
            int top = static_cast<int>(frame.rows * outDetections.at<float>(i, 4));
            int right = static_cast<int>(frame.cols * outDetections.at<float>(i, 5));
            int bottom = static_cast<int>(frame.rows * outDetections.at<float>(i, 6));
            
            left = max(0, min(left, frame.cols - 1));
            top = max(0, min(top, frame.rows - 1));
            right = max(0, min(right, frame.cols - 1));
            bottom = max(0, min(bottom, frame.rows - 1));
            Rect box = Rect(left, top, right - left + 1, bottom - top + 1);
            
            // Extract the mask for the object
            Mat objectMask(outMasks.size[2], outMasks.size[3],CV_32F, outMasks.ptr<float>(i,classId));
            
            // Draw bounding box, colorize and show the mask on the image
            drawBox(frame, classId, score, box, objectMask);
            
        }
    }
}



Mat detect(Mat& input, string textGraph, string modelWeights){
    
    Net net = readNetFromTensorflow(modelWeights, textGraph);
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);
    
    Mat blob;
        
    // Stop the program if reached end of video
    if (input.empty()) {
        cout << "Done processing !!!" << endl;
        waitKey(3000);
    }
    // Create a 4D blob from a frame.
    blobFromImage(input, blob, 1.0, Size(input.cols, input.rows), Scalar(), true, false);
    //blobFromImage(frame, blob);
        
    //Sets the input to the network
    net.setInput(blob);

    // Runs the forward pass to get output from the output layers
    outNames[0] = "detection_out_final";
    outNames[1] = "detection_masks";
    net.forward(outs, outNames);
        
    // Extract the bounding box and mask for each of the detected objects
    postprocess(input, outs);
    
    return input;
}
