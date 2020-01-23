//
//  Utility.hpp
//  opencv
//
//  Created by 심지훈 on 02/10/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp

#include "detectionHeader.h"

void mouseOnClick(int event, int x, int y, int flags, void* userdata);
void draw_histo(Mat hist, Mat &hist_img,int channel, Size size);


#endif /* Utility_hpp */
