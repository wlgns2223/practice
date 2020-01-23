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

template <typename T>
class ROIData
{
public:
    void setSrcPts(vector<T> &pts);
    vector<T> getSrcPts();
    
    void setDestPts(vector<T> &pts);
    vector<T> getDestPts();
    
private:
    bool isEmpty(vector<T> &pts);
    vector<T> srcPts, destPts;
};


#endif /* ROIData_hpp */
