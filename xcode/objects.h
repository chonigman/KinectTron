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
    void ball(ci::Vec3f mLocation);
    void bounceBall(int whichBall);
    void bounceBall();
    void ballRetrieval(ci::Vec3f head, ci::Vec3f handR, ci::Vec3f handL);
    
    ci::Vec3f ballLoc, rodLoc, rodLocEnd, ballColor, rodColor;
    ci::Vec3f zBall, zzBall, ballDir;
    
    ci::Vec3f b2Loc, b3Loc, b4Loc, b2z, b3z, b4z, b2Dir, b3Dir, b4Dir;
    
    
    bool ballHitR, ballHitL, rodHit, b2Hit, b3Hit, b4Hit;
    bool ballThrow, b2Throw, b3Throw, b4Throw;
    int xDir, yDir, zDir;

    ci::Vec3f ballsLoc[4], ballsDir[4], ballsColor[4], zBallsLocR[4], zBallsLocL[4];
    int ballsHit[4];
    bool ballsThrow[4];
    
};