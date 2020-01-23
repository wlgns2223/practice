//
//  Calibration.hpp
//  opencv
//
//  Created by 심지훈 on 11/08/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#ifndef Calibration_hpp
#define Calibration_hpp
#include "detectionHeader.h"

class Calibration {
    
private:
    int numBoards = 15;
    int numCornersHor = 9;
    int numCornersVer = 6;
    int numSquares = numCornersHor * numCornersVer;
    Size boardSize = Size(numCornersHor, numCornersVer);
    const Size size = Size(640,360);
    Mat intrinsic = Mat(3,3, CV_32FC1);
    Mat distCoeffs;
    
    
public:
    
    Calibration(Mat intrinsic, Mat distCoeffs);
    Calibration();
    
    void cameraCalibration(String chessBoardPath);
    void saveCameraMatrix(String filePath);
    void loadCameraMatrix(String filePath, String name);
    Mat getUndistortedImg(Mat input);
};


#endif /* Calibration_hpp */

