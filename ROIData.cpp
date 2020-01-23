//
//  ROIData.cpp
//  opencv
//
//  Created by 심지훈 on 2020/01/23.
//  Copyright © 2020 Shim. All rights reserved.
//

#include "ROIData.hpp"

template <typename T>
void ROIData<T>::setSrcPts(vector<T> &pts)
{
    this->srcPts = pts;
}

template <typename T>
void ROIData<T>::setDestPts(vector<T> &pts)
{
    this->destPts = pts;
}

template <typename T>
bool ROIData<T>::isEmpty(vector<T> &pts)
{
    return pts.empty();
}

template <typename T>
vector<T> ROIData<T>::getSrcPts()
{
    if(isEmpty(srcPts)){
        cerr<<"Empty Point has returned\n";
        return vector<T>();
    }
    return destPts;
    
}

template <typename T>
vector<T> ROIData<T>::getDestPts()
{
    if(isEmpty(destPts)){
        cerr<<"Empty Point has returned\n";
        return vector<T>();
    }
    return destPts;
}

