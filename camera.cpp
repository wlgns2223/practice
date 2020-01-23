    int numBoards = 15;
    int numCornersHor = 9;
    int numCornersVer = 6;
    

    vector<Mat> images;
    for(int num = 6; num<=20; ++num) {
        Mat temp = imread(chessboard + to_string(num) + ".jpg");
        resize(temp, temp, Size(640, 360));
        images.push_back(temp);
    }

    int numSquares = numCornersHor * numCornersVer;
    Size board_sz(numCornersHor, numCornersVer);

    vector<vector<Point3f>> object_points;
    vector<vector<Point2f>> image_points;
    vector<Point2f> corners;
    int success = 0;

    Mat image;
    Mat gray_image;

    vector<Point3f> obj;
    for(int j=0; j<numSquares; ++j)
        obj.push_back(Point3f(j / numCornersHor, j % numCornersHor, 0.0f));
    
    
    vector<Mat>::iterator pos = images.begin();
    image = *pos;
    while(success < numBoards){
        if(pos == images.end()) break;
        
        
        cvtColor(image, gray_image, COLOR_BGR2GRAY);
        bool found = findChessboardCorners(image, board_sz, corners);
        if(found)
            drawChessboardCorners(gray_image, board_sz, corners, found);
        
//        imshow("img", gray_image);
        pos++;
        
        if(found != 0){
            image_points.push_back(corners);
            object_points.push_back(obj);
            
            cout<<"Snap Stored!\n";
            success++;
        }
    }
    Mat intrinsic = Mat(3,3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;
    
    calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
    
    Mat undistorted;
    undistort(image, undistorted, intrinsic, distCoeffs);





    
    cv::FileStorage fs(xmlPath+"intrinsics.xml", FileStorage::WRITE);
    fs<<"image_width"<<image.size().width<<"image_height"<<image.size().height
      <<"camera_matrix"<<intrinsic<<"distortion_coefficients"<<distCoeffs;
    fs.release();
    
    fs.open(xmlPath+"intrinsics.xml", FileStorage::READ);
    cout<<"\nimage width"<<(int)fs["image_width"];
    cout<<"\nimage height"<<(int)fs["image_height"];
    
    Mat intrinsic_loaded, distortion_coeffs_loaded;
    fs["camera_matrix"]>>intrinsic_loaded;
    fs["distortion_coefficients"]>>distortion_coeffs_loaded;
    
    Mat map1, map2;
    initUndistortRectifyMap(intrinsic_loaded, distortion_coeffs_loaded,
                            cv::Mat(), intrinsic_loaded, image.size(),
                            CV_16SC2, map1, map2);
    
    Mat img0 = imread(filePath1);
    resize(img0, img0, Size(640,360));
    Mat kkk;
    
    
    remap(img0, kkk, map1, map2, cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar());
    
    imshow("img", kkk);
    imshow("undistorted", undistorted);
    waitKey();
    
    
