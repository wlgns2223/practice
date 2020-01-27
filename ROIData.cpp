//
//  ROIData.cpp
//  opencv
//
//  Created by 심지훈 on 2020/01/23.
//  Copyright © 2020 Shim. All rights reserved.
//

#include "ROIData.hpp"

void ROIData::setSrcPts(vector<Point2f> &pts)
{
    this->srcPts = pts;
}

void ROIData::setDestPts(vector<Point2f> &pts)
{
    this->destPts = pts;
}

bool ROIData::isEmpty(vector<Point2f> &pts)
{
    return pts.empty();
}

vector<Point2f> ROIData::getSrcPts()
{
    if(isEmpty(srcPts)){
        cerr<<"Empty Point has returned\n";
        return vector<Point2f>();
    }
    return destPts;
    
}

vector<Point2f> ROIData::getDestPts()
{
    if(isEmpty(destPts)){
        cerr<<"Empty Point has returned\n";
        return vector<Point2f>();
    }
    return destPts;
}

