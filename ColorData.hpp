//
//  ColorData.hpp
//  opencv
//
//  Created by 심지훈 on 2020/01/23.
//  Copyright © 2020 Shim. All rights reserved.
//

#ifndef ColorData_hpp
#define ColorData_hpp
#include "detectionHeader.h"

class ColorData
{
public:
    void setColor(string colorName, Scalar colorValue);
    Scalar getColor(string desiredColor);
    ColorData();
    
private:
    map<string, Scalar> colorList;
};

#endif /* ColorData_hpp */
