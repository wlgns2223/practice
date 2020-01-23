//
//  Calibration.cpp
//  opencv
//
//  Created by 심지훈 on 11/08/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "Calibration.hpp"

Calibration::Calibration(Mat intrinsic, Mat distCoeffs):intrinsic(intrinsic), distCoeffs(distCoeffs)
{
    if(this->intrinsic.empty() || this->distCoeffs.empty())
        cout<<"ERROR : Camera Matrix is empty\n";
}

Calibration::Calibration() { }

void Calibration::cameraCalibration(String chessBoardPath){
    
    vector<Mat> chessBoardImages;
    for(int num = 1; num<=15; ++num) {
        Mat temp = imread(chessBoardPath + to_string(num) + ".jpg");
        resize(temp, temp, Size(640, 360));
        chessBoardImages.push_back(temp);
    }
    
    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;
    vector<Point2f> corners;
    int success = 0;
    
    Mat image;
    Mat gray_image;
    
    vector<Point3f> obj;
    for(int j=0; j<numSquares; ++j)
        obj.push_back(Point3f(j / numCornersHor, j % numCornersHor, 0.0f));
    
    
    vector<Mat>::iterator pos = chessBoardImages.begin();
    image = *pos;
    while(success < numBoards){
        if(pos == chessBoardImages.end()) break;
        
        
        cvtColor(image, gray_image, COLOR_BGR2GRAY);
        bool found = findChessboardCorners(image, this->boardSize, corners);
        if(found)
            drawChessboardCorners(gray_image, this->boardSize, corners, found);
        
        
        pos++;
        
        if(found != 0){
            image_points.push_back(corners);
            object_points.push_back(obj);
            
            cout<<"Snap Stored!\n";
            success++;
        }
    }
    
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;
    
    calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
    
    if(!intrinsic.empty())
        cout<<"Intrinsic Done\n";
    if(!distCoeffs.empty())
        cout<<"DistCoeffs Done\n";
    
    
}

void Calibration::saveCameraMatrix(String filePath){
    cv::FileStorage fs(filePath+"cameraMatrix.xml", FileStorage::WRITE);
    fs<<"camera_matrix"<<intrinsic<<"distortion_coefficients"<<distCoeffs;
    fs.release();
    
}

void Calibration::loadCameraMatrix(String filePath, String name){
    cv::FileStorage fs(filePath+name, FileStorage::READ);
    fs["camera_matrix"]>>this->intrinsic;
    fs["distortion_coefficients"]>>this->distCoeffs;
    fs.release();
}

Mat Calibration::getUndistortedImg(Mat input){
    
    Mat img;
    if(input.size() != this->size )
        resize(input, input, this->size);

    undistort(input, img, this->intrinsic, this->distCoeffs);
    return img;
}
