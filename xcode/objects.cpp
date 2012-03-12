//
//  objects.cpp
//  threeDstuff
//
//  Created by Colin Honigman on 3/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "objects.h"
#include "cinder/Vector.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;

objects::objects(){
    ballLoc = Vec3f(4, 15, 5);
    rodLoc = Vec3f(-3, 15, -10);
    rodLocEnd = Vec3f(-3, 0, -10);
    ballColor =  Vec3f(0.0f, 1.0f, 1.0f);
    rodColor =  Vec3f(1.0f, 0, 1.0f);
    ballHit = false;
    rodHit = false;
    ballThrow = false;
    xDir = 0;
    yDir = 0;
    zDir = 0;
}

void objects::draw(){
    ball();
    rod();
}

void objects::update(){
    
    if(ballThrow == true){
        bounceBall();
    }
    
    
}

void objects::bounceBall(){
    
    ballLoc.x += ballDir.x;

        ballLoc.y += ballDir.y;
        ballLoc.z += ballDir.z;

    
    if(ballLoc.x > 100 || ballLoc.x < -100){
        ballDir.x = -ballDir.x;
    }

    if(ballLoc.y > 100 || ballLoc.y < 0){
        ballDir.y = -ballDir.y;
    }

    if(ballLoc.z > 50 || ballLoc.z < -100){
        ballDir.z = -ballDir.z;
    }
    
}

void objects::hitTest(ci::Vec3f handR,ci::Vec3f handL ){
    
    
    if(handR.x < ballLoc.x+2.0f && handR.x > ballLoc.x-2.0f && handR.y < ballLoc.y+2.0f && handR.y > ballLoc.y-2.0f &&  handR.z < ballLoc.z + 2.0f && handR.z > ballLoc.z - 2.0f){
        float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
        
        //std::cout << handDist << std::endl;
        
        if(handDist > 1) {
            ballHit = true;
            ballColor.x = 1.0f;
            ballColor.y = 1.0f;
            ballColor.z = 0.0f;
        } else {
            ballHit = false;
            ballColor.x = 0.0f;
            ballColor.y = 1.0f;
            ballColor.z = 1.0f;
        }
        
    }
    
    if(ballHit == true && ballThrow == false){
        ballLoc = handR;
        if(zzBall.z < handR.z+2 && zzBall.y-2 > handR.y){
            ballThrow = true;
            ballHit = false;
            ballDir = Vec3f((ballLoc.x - zzBall.x), (ballLoc.y - zzBall.y), (ballLoc.z - zzBall.z));
            std::cout << "Ball Thrown" << ballDir << std::endl;
        }
    }
    else if( ballHit == true && ballThrow == true){
        ballThrow = false;
        
    }
    
    if(handL.x < rodLoc.x+1.0f && handL.x > rodLoc.x-1.0f && handL.y < rodLoc.y && handL.y > rodLocEnd.y &&  handL.z < rodLoc.z + 1.0f && handL.z > rodLoc.z - 1.0f){
        float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
        
       // std::cout << handDist << std::endl;
        
        if(handDist > 1) {
            rodHit = true;
            rodColor.x = 0.0f;
            rodColor.y = 1.0f;
            rodColor.z = 0.0f;
        } else {
            rodHit = false;
            rodColor.x = 1.0f;
            rodColor.y = 0.0f;
            rodColor.z = 1.0f;
        }
        
    }
    
    if(rodHit == true){
        rodLoc = handL;
        rodLocEnd.x = handL.x;
        rodLocEnd.y = 0;
        rodLocEnd.z = handL.z;
        
        for(float i = 0; i < 2 ; i+=.1){
            gl::color( Color(rodColor.x, rodColor.y, rodColor.z));
            gl::drawLine(Vec3f(rodLoc.x+i, rodLoc.y, rodLoc.z), Vec3f(rodLoc.x+i, rodLoc.y, rodLoc.z));
        }
    }
    
    zzBall = zBall;
    zBall = handR;
    
    
}


void objects::ball(){
    ci::gl::color( Color(ballColor.x, ballColor.y, ballColor.z) );
    gl::drawSphere(ballLoc, 2.0f, 12);
    
}

void objects::rod(){
    ci::gl::color( Color(rodColor.x, rodColor.y, rodColor.z) );
    gl::drawLine(rodLoc, rodLocEnd);
    if(rodHit == true){
        gl::drawLine(Vec3f(rodLoc.x, rodLoc.y+5, rodLoc.z), rodLocEnd);
        for(float i = 0; i < 2 ; i+=.1){
            gl::color( Color(rodColor.x, rodColor.y, rodColor.z));
            gl::drawLine(Vec3f(rodLoc.x+i, rodLoc.y, rodLoc.z), Vec3f(rodLoc.x+i, rodLoc.y, rodLoc.z));
        }
        
                     
        
    }
    
}

