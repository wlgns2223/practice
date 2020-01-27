//
//  ColorData.cpp
//  opencv
//
//  Created by 심지훈 on 2020/01/23.
//  Copyright © 2020 Shim. All rights reserved.
//

#include "ColorData.hpp"

void ColorData::setColor(string colorName, Scalar colorValue){
    colorList.insert(make_pair(colorName, colorValue));
    
}

Scalar ColorData::getColor(string desiredColor){
    return colorList.find(desiredColor)->second;
    
    
}

ColorData::ColorData()
{
    colorList.insert(make_pair("GREEN", Scalar(0,255,0)));
    colorList.insert(make_pair("BLUE", Scalar(255,0,0)));
    colorList.insert(make_pair("RED", Scalar(0,0,255)));
    colorList.insert(make_pair("YELLOW", Scalar(0,255,255)));
    
}
