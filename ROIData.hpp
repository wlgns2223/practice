//
//  ROIData.hpp
//  opencv
//
//  Created by 심지훈 on 2020/01/23.
//  Copyright © 2020 Shim. All rights reserved.
//

#ifndef ROIData_hpp
#define ROIData_hpp
#include "detectionHeader.h"

class ROIData
{
public:
    void setSrcPts(vector<Point2f> &pts);
    vector<Point2f> getSrcPts();
    
    void setDestPts(vector<Point2f> &pts);
    vector<Point2f> getDestPts();
    
private:
    bool isEmpty(vector<Point2f> &pts);
    vector<Point2f> srcPts, destPts;
};


#endif /* ROIData_hpp */
