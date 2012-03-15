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
    b2Loc = Vec3f(-8, 5, -2);
    b3Loc = Vec3f(10, 25, 10);
    b4Loc = Vec3f(0, 0, 0);
    rodLoc = Vec3f(-3, 20, 0);
    rodLocEnd = Vec3f(-3, 0, -10);
    ballColor =  Vec3f(0.0f, 1.0f, 1.0f);
    rodColor =  Vec3f(1.0f, 0, 1.0f);
    ballHitR = false;
    ballHitL = false;
    rodHit = false;
    ballThrow = false;
    b2Hit = false;
    b3Hit = false;
    b4Hit = false;
    xDir = 0;
    yDir = 0;
    zDir = 0;
    for(int i = 0; i < 8; i++){
        ballsLoc[i] = Vec3f(-8+(5*i), 5+i*2, 10+i*3); 
        ballsThrow[i] = false;
    }
}

void objects::draw(){
   // ball();
   // rod();
    
    for(int i = 0; i < 8; i++){
        ball(ballsLoc[i]);
    }
}

void objects::update(){
    
    if(ballThrow == true){
        bounceBall();
    }
    
    for(int i = 0; i < 8; i++){
        if(ballsThrow[i] == true){
         //   std::cout << "ball throwing no moving?" << std::endl;
            bounceBall( i );
        }
    }
    
    
}

void objects::hitTest(ci::Vec3f handR, ci::Vec3f handL ){
    
    //****************************first ball ***********************
    //right hand ***************************************************
    if(handR.x < ballLoc.x+2.0f && handR.x > ballLoc.x-2.0f && handR.y < ballLoc.y+2.0f && handR.y > ballLoc.y-2.0f &&  handR.z < ballLoc.z + 2.0f && handR.z > ballLoc.z - 2.0f){
        float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
        
        //std::cout << handDist << std::endl;
        
        if(handDist > 1) {
            ballHitR = true;
            ballColor.x = 1.0f;
            ballColor.y = 1.0f;
            ballColor.z = 0.0f;
        } else {
            ballHitR = false;
            ballColor.x = 0.0f;
            ballColor.y = 1.0f;
            ballColor.z = 1.0f;
        }
        
    }
    //left hand ***************************************************
    else if(handL.x < ballLoc.x+2.0f && handL.x > ballLoc.x-2.0f && handL.y < ballLoc.y+2.0f && handL.y > ballLoc.y-2.0f &&  handL.z < ballLoc.z + 2.0f && handL.z > ballLoc.z - 2.0f){
        float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
        
        //std::cout << handDist << std::endl;
        
        if(handDist > 1) {
            ballHitL = true;
            ballColor.x = 1.0f;
            ballColor.y = 1.0f;
            ballColor.z = 0.0f;
        } else {
            ballHitL = false;
            ballColor.x = 0.0f;
            ballColor.y = 1.0f;
            ballColor.z = 1.0f;
        }
        
    }
    
    //grab ball and check for throw *******************************
    if(ballHitR == true && ballThrow == false){
        ballLoc = handR;
        if(zzBall.z < handR.z+2 || zzBall.y+2 > handR.y){
            ballThrow = true;
            ballHitR = false;
            ballDir = Vec3f((ballLoc.x - zzBall.x), (ballLoc.y - zzBall.y), (ballLoc.z - zzBall.z));
            //std::cout << "Ball Thrown" << ballDir << std::endl;
        }
    }
    else if( ballHitR == true && ballThrow == true){
        ballThrow = false;
    }
    if(ballHitL == true && ballThrow == false){
        ballLoc = handL;
        if(zBall.z < handL.z+2 && zBall.y+2 > handL.y){
            ballThrow = true;
            ballHitL = false;
            ballDir = Vec3f((ballLoc.x - zBall.x), (ballLoc.y - zBall.y), (ballLoc.z - zBall.z));
            //std::cout << "Ball Thrown" << ballDir << std::endl;
        }
    }
    else if( ballHitL == true && ballThrow == true){
        ballThrow = false;
    }
    
    
    zzBall = handR;
    zBall = handL;
    
    
    //**********************************Balls 2 3 and 4 *************************************
    //***************************************************************************************    
    for(int i = 0; i < 8; i++){
        if(handR.x < ballsLoc[i].x+2.0f && handR.x > ballsLoc[i].x-2.0f && handR.y < ballsLoc[i].y+2.0f && handR.y > ballsLoc[i].y-2.0f && handR.z < ballsLoc[i].z + 2.0f && handR.z > ballsLoc[i].z - 2.0f){
            
            float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
            if(handDist > 1) {
                ballsHit[i] = 1;
            } else {
                ballsHit[i] = 0;
            }            
        }
        
        else if(handL.x < ballsLoc[i].x+2.0f && handL.x > ballsLoc[i].x-2.0f && handL.y < ballsLoc[i].y+2.0f && handL.y > ballsLoc[i].y-2.0f && handL.z < ballsLoc[i].z + 2.0f && handL.z > ballsLoc[i].z - 2.0f){
            
            float handDist = abs(handR.x - handL.x) + abs(handR.y - handL.y);
            if(handDist > 1) {
                ballsHit[i] = 2;
            } else {
                ballsHit[i] = 0;
            }            
        }
        
        if(ballsHit[i] == 1 && ballsThrow[i] == false){
            ballsLoc[i] = handR;
            if(zBallsLocR[i].z > handR.z+2 || zBallsLocR[i].y+2 > handR.y){
                ballsThrow[i] = true;
                ballsHit[i] = 0;
                ballsDir[i] = Vec3f((ballsLoc[i].x - zBallsLocR[i].x), (ballsLoc[i].y - zBallsLocR[i].y), (ballsLoc[i].z - zBallsLocR[i].z));
               // std::cout << "Ball Thrown "<< i << " " << ballsDir[i] << std::endl;
            }
            zBallsLocR[i] = handR;
        }
        else if(ballsHit[i] == 2 && ballsThrow[i] == false){
            ballsLoc[i] = handL;
            if(zBallsLocL[i].z > handL.z+2 || zBallsLocL[i].y+2 > handL.y){
                ballsThrow[i] = true;
                ballsHit[i] = 0;
                ballsDir[i] = Vec3f((ballsLoc[i].x - zBallsLocL[i].x), (ballsLoc[i].y - zBallsLocL[i].y), (ballsLoc[i].z - zBallsLocL[i].z));
               // std::cout << "Ball Thrown "<< i << " " << ballsDir[i] << std::endl;
            }
            zBallsLocL[i] = handL;
        }        
        
        if( ballsHit[i] == 1 && ballsThrow[i] == true){
            ballsThrow[i] = false;
           // zBallsLocR[i] = handR;
        }
        else if( ballsHit[i] == 2 && ballsThrow[i] == true){
            ballsThrow[i] = false;
           // zBallsLocL[i] = handL;
        }
    }
    
    
    
    //*********************************** ROD ***********************************************
    //***************************************************************************************
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
    
    
    
    
}


void objects::ball(){
    ci::gl::color( Color(ballColor.x, ballColor.y, ballColor.z) );
    gl::drawSphere(ballLoc, 2.0f, 12);
    
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
    
    if(ballLoc.z > 100 || ballLoc.z < -100){
        ballDir.z = -ballDir.z;
    }
    
}


void objects::ball(ci::Vec3f mLocation){
    ci::gl::color( 1.0f, 1.0f, 1.0f);
    gl::drawSphere(mLocation, 2.0f, 12);
}

void objects::bounceBall(int whichBall){
    
    ballsLoc[whichBall].x += ballsDir[whichBall].x;
    ballsLoc[whichBall].y += ballsDir[whichBall].y;
    ballsLoc[whichBall].z += ballsDir[whichBall].z;
    
    
    if(ballsLoc[whichBall].x > 100 || ballsLoc[whichBall].x < -100){
        ballsDir[whichBall].x = -ballsDir[whichBall].x;
    }
    
    if(ballsLoc[whichBall].y > 100 || ballsLoc[whichBall].y < 0){
        ballsDir[whichBall].y = -ballsDir[whichBall].y;
    }
    
    if(ballsLoc[whichBall].z > 100 || ballsLoc[whichBall].z < -100){
        ballsDir[whichBall].z = -ballsDir[whichBall].z;
    }
}


//*******************BALL RETRIEVAL METHOD*********************
//*************************************************************
void objects::ballRetrieval(ci::Vec3f head, ci::Vec3f handR, ci::Vec3f handL){
    if(ballLoc.z > head.z && handR.y > head.y ){
        ballThrow = false;
        ballHitR = true;
    }
    else if(ballLoc.z > head.z && handL.y > head.y ){
        ballThrow = false;
        ballHitL = true;
    }
    
    for(int i = 0; i < 8; i ++){
        if(ballsLoc[i].z > head.z && handR.y > head.y && handL.y > head.y){
            ballsThrow[i] = false;
            if(handR.y>handL.y) ballsHit[i] = 1;
            else ballsHit[i] = 2;
        }
        
        //if(ballsLoc[i].z > head.z && handL.y > head.y){
        //    ballsThrow[i] = false;
        //    ballsHit[i] = 2;
       // }
        
    }
    
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

