//
//  objects.h
//  threeDstuff
//
//  Created by Colin Honigman on 3/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Vector.h"
#include <vector>

class objects{
public:
    objects();
    

    void update();
    void draw();
    void hitTest(ci::Vec3f handR, ci::Vec3f handL);
    void ball();
    void rod();
    void bounceBall();
    
    ci::Vec3f ballLoc, rodLoc, rodLocEnd, ballColor, rodColor;
    ci::Vec3f zBall, zzBall, ballDir;
    
    bool ballHit, rodHit;
    bool ballThrow;
    int xDir, yDir, zDir;

    
    
};