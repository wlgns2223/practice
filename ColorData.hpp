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

enum Colorlist{
    GREEN,
    BLUE,
    RED,
    YELLOW
};

template <typename T>
class ColorData
{
public:
    void setColor(T color);
    T getColor();
    
private:
    Colorlist colorName;
    Scalar rgb;
    
};

#endif /* ColorData_hpp */
