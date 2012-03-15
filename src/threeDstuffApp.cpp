#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Rand.h"
#include "cinder/TriMesh.h"
#include "Resources.h"
#include "cinder/gl/gl.h"

#include "OscListener.h"
#include "OscSender.h"

#include "objects.h"

#include <vector>
#include <utility>
#include <math.h>

//#include "cinder/gl/Light.h"

using namespace ci;
using namespace ci::app;
using namespace std;

GLfloat no_mat[]			= { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[]		= { 0.6, 0.3, 0.4, 1.0 };
GLfloat mat_diffuse[]		= { 0.3, 0.5, 0.8, 1.0 };
GLfloat mat_specular[]		= { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_emission[]		= { 0.0, 0.1, 0.3, 0.0 };

GLfloat mat_shininess[]		= { 128.0 };
GLfloat no_shininess[]		= { 0.0 };

class threeDstuffApp : public AppBasic {
public:
    void prepareSettings( Settings *settings);
	void setup();
	void update();
	void draw();
    
    void drawGrid(float size=100.0f, float step= 20.0f);
    void drawGridY(float size = 100.0f, float step= 20.0f);
    void drawCone(float size = 80.0f, float step = 20.0f);
    
    bool performPicking( Vec3f *pickedpoint, Vec3f *pickedNormal );
    
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void resize( ResizeEvent event );
    
    void wallHit(Vec3f ball);
    
    void fadeOut();
    
    void oscUpdate();
    
    Vec3f vec;
    Vec3f col;
    //bodyparts
    Vec3f rH, lH, rE, lE, rS, lS, rF, lF, head;
    Vec3f rHz, lHz;
    //objects
    Vec3f o1, o1z, o2, o2z;
    
    float oc1r, oc1g, oc1b;
    float al, al2, al3, al4, al5;
    
    bool o1Hit;
    bool o2Hit;
    osc::Listener listener;
    osc::Sender sender;
    std::string host;
    int port;
    float positionX;
    
    objects mObjects;
    
    Vec2f mMousePos;
    float mDirectional;
    
    
protected:
    //transformations (translate, rotate, scale) of the model
    
    //our camera
    MayaCamUI mMayaCam;
    
    Matrix44f mTransform;
    
    //keep track of mouse
    // Vec2i mMousePos;
    TriMesh mesh;
    double mTime;
    
    GLfloat light_position[4];
    
    float shoulderDiff, shoulderDiffz, shoulderDiffzz;
    
    
};

void threeDstuffApp::prepareSettings( Settings *settings){
    //settings->setWindowSize(1280, 800);
    settings->enableSecondaryDisplayBlanking( false );
    
}

void threeDstuffApp::setup()
{
    host = "127.0.0.1";
    port = 13000;
    sender.setup(host, port);
    mTime = getElapsedSeconds();
    mTransform.setToIdentity();
    
    CameraPersp cam;
    cam.setEyePoint( Vec3f(0.0f, 10.0f, 20.0f) );
    cam.setCenterOfInterestPoint( Vec3f(0.0f, 0.0f, 0.0f) );
    cam.setPerspective( 80.0f, getWindowAspectRatio(), 1.0f, 1000.0f );
    mMayaCam.setCurrentCam( cam );
    
    listener.setup(12000);
    
    // mLight.Light(2, 1);
    mDirectional = -1.0f;
    al = al2 = al3 = al4 = .2;
    // gl::enableDepthWrite();
    //	gl::enableDepthRead();
    //	gl::enableAlphaBlending();
    
}

void threeDstuffApp::update()
{
    //  double elapsed = getElapsedSeconds() - mTime;
    //  mTime = getElapsedSeconds();
    
    mTransform.setToIdentity(); 
    oscUpdate();
    
    mObjects.hitTest(rH, lH);
    
    light_position[0] = head.x;
    light_position[1] = head.y;
    light_position[2] = head.z - 10;
    light_position[4] = 0;
    
    mObjects.update();
    //wallHit(mObjects.ballLoc);
    
    for(int i = 0; i < 8; i++){
        wallHit(mObjects.ballsLoc[i]);
    }
    fadeOut();
    
    mObjects.ballRetrieval(head, rH, lH);
}


//*******************************************************************************************
//                      RECEIVE OSC JOINT POSITIONS
//*******************************************************************************************

void threeDstuffApp::oscUpdate(){
    while (listener.hasWaitingMessages()) {
		osc::Message message;
		listener.getNextMessage(&message);
		
        if(message.getAddress() == "/righthand"){
            rH.x = message.getArgAsFloat(0)/50;
            rH.y = message.getArgAsFloat(1)/75 + 15;
            rH.z = (message.getArgAsFloat(2)/50)-20;
            
            // console() << rH.x << " " << rH.y << " " << rH.z << std::endl;
        }
        else if(message.getAddress() =="/lefthand"){
            lH.x = message.getArgAsFloat(0)/50;
            lH.y = message.getArgAsFloat(1)/75 + 15;
            lH.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() == "/rightElbow"){
            rE.x = message.getArgAsFloat(0)/50;
            rE.y = message.getArgAsFloat(1)/75 + 15;
            rE.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() == "/leftElbow"){
            lE.x = message.getArgAsFloat(0)/50;
            lE.y = message.getArgAsFloat(1)/75 + 15;
            lE.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() =="/rightShoulder"){
            rS.x = message.getArgAsFloat(0)/50;
            rS.y = message.getArgAsFloat(1)/75 + 15;
            rS.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() =="/leftShoulder"){
            lS.x = message.getArgAsFloat(0)/50;
            lS.y = message.getArgAsFloat(1)/75 + 15;
            lS.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() == "/rightFoot"){
            rF.x = message.getArgAsFloat(0)/50;
            rF.y = message.getArgAsFloat(1)/75+15;
            rF.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() == "/leftFoot"){
            lF.x = message.getArgAsFloat(0)/50;
            lF.y = message.getArgAsFloat(1)/75+15;
            lF.z = (message.getArgAsFloat(2)/50)-20;
        }
        else if(message.getAddress() == "/head"){
            head.x = message.getArgAsFloat(0)/50;
            head.y = message.getArgAsFloat(1)/75+15;
            head.z = (message.getArgAsFloat(2)/50)-20;
        }
        
    }
    
    
}

void threeDstuffApp::draw()
{
    // cout << rS.z - lS.z << std::endl;
    // clear out the window with white
	gl::clear( Colorf( 0.0f, 0.0f, 0.0f ) ); 
    ci::ColorA color( al2, al3, al4, al);
    //for(int x = head.x-5; x <head.x+5; x+=5){
    //for(int z = head.y-20; z < head.y+20; z+=10){
   // gl::pushMatrices();
    glEnable( GL_LIGHTING);
    glEnable( GL_LIGHT0);
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // light_position[0] = 0;
   // light_position[1] = 20;
   // light_position[2] = 70;
   // light_position[3] = 0;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color );
    //glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    //glMaterialfv(GL_FRONT, GL_AMBIENT, color);
   // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, color);
   //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    //glLightfv(GL_FRONT, GL_SPOT_CUTOFF, light_position);
    
    
    //glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    //  glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );   
    // }
    //}
    
    
    //setup the camera
    gl::pushMatrices();
    gl::setMatrices( mMayaCam.getCamera() );
    
    //enable the depth bufer (after all, we are doing 3D)
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    //draw the grid on the floor
    drawGrid();
     drawGridY();
    //drawCone();
    gl::color( Colorf(1.0f, 0.0f, 0.0f) );
    //gl::drawCube( Vec3f::zero(), Vec3f( 2.0f, 2.0f, 2.0f ) );
    
    gl::color( Color(1, 0, 0) );
    
    gl::drawCube( rH, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( lH, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( rE, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( lE, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( rS, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( lS, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( rF, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( lF, Vec3f(1.0f, 1.0f, 1.0f));
    gl::drawCube( head, Vec3f(1.0f, 1.0f, 1.0f));
        gl::drawLine( rH, rE);
    gl::drawLine(rE, rS);
    gl::drawLine(rS, lS);
    gl::drawLine(lS, lE);
    gl::drawLine(lE, lH);
    
    
	//gl::drawStrokedCube(Vec3f::zero(), Vec3f( 6.0f, 6.0f, 6.0f ));
    
	// the coordinates of the bounding box are in object space, not world space,
	// so if the model was translated, rotated or scaled, the bounding box would not
	// reflect that.
	//AxisAlignedBox3f worldBounds = Vec3f(0.1, 0.1, 0.1), Vec3f(1, 1, 1);
    
	// draw this transformed box in cyan
	gl::color( Color(oc1r, oc1g, oc1b) );
    
    //  gl::drawSphere( o1, 2.0f, 12);
    //gl::drawLine( 
	//gl::drawStrokedCube( Vec3f::zero(), Vec3f( 4.0f, 4.0f, 4.0f ));
    
    
    mObjects.draw();
    
    shoulderDiff = (rS.z - lS.z);
    shoulderDiffzz = (shoulderDiff + shoulderDiffz)*.5;
    // gl::color( Color(0, 1, 0) );
    /* ********* Camera Follows User *********************
    CameraPersp cam;
    cam.setEyePoint( Vec3f(head.x, head.y, head.z + 30) );
    cam.setCenterOfInterestPoint( Vec3f(shoulderDiff*4, head.y, shoulderDiff));
    cam.setPerspective( 80.0f, getWindowAspectRatio(), 1.0f, 1000.0f );
    mMayaCam.setCurrentCam( cam );
    */
    CameraPersp cam;
    cam.setEyePoint( Vec3f(0, 20, 100) );
    cam.setCenterOfInterestPoint( Vec3f(0, 20, -100));
    cam.setPerspective( 80.0f, getWindowAspectRatio(), 1.0f, 1000.0f );
    mMayaCam.setCurrentCam( cam );
    
    rHz = rH;
    
    //std::cout << shoulderDiff << " "<< rS.z << " " << lS.z << std::endl;
    shoulderDiffzz = shoulderDiffz;
    shoulderDiffz = shoulderDiff;
    
    
}
//*****************************************************************************
//                      WALL HIT SEND OSC
//*****************************************************************************


void threeDstuffApp::wallHit(Vec3f ball){
    if(ball.y <= 0){
        al = 1;
        osc::Message msg;
        msg.setAddress("/ballFloor");
        msg.setRemoteEndpoint(host, port);
        msg.addFloatArg(mObjects.ballLoc.x);
        msg.addFloatArg(mObjects.ballLoc.y);
        msg.addFloatArg(mObjects.ballLoc.z);
        sender.sendMessage(msg);
    }
    else if(ball.z <= -100){
        al2 = 1;
        al = 1;
        osc::Message msg;
        msg.setAddress("/frontWall");
        msg.setRemoteEndpoint(host, port);
        msg.addFloatArg(mObjects.ballLoc.x);
        msg.addFloatArg(mObjects.ballLoc.y);
        msg.addFloatArg(mObjects.ballLoc.z);
        sender.sendMessage(msg);
    }
    else if(ball.x <= -100){
        al3 = 1; 
        al = 1;
        osc::Message msg;
        msg.setAddress("/leftWall");
        msg.setRemoteEndpoint(host, port);
        msg.addFloatArg(mObjects.ballLoc.x);
        msg.addFloatArg(mObjects.ballLoc.y);
        msg.addFloatArg(mObjects.ballLoc.z);
        sender.sendMessage(msg);
    }
    else if(ball.x >= 100){
        al4 = 1;  
        al = 1;
        osc::Message msg;
        msg.setAddress("/rightWall");
        msg.setRemoteEndpoint(host, port);
        msg.addFloatArg(mObjects.ballLoc.x);
        msg.addFloatArg(mObjects.ballLoc.y);
        msg.addFloatArg(mObjects.ballLoc.z);
        sender.sendMessage(msg);
    }
    
    else if(ball.y >= 100){
        al4 = 1; 
        al2 = 1;
        al = 1;
        osc::Message msg;
        msg.setAddress("/ballCeiling");
        msg.setRemoteEndpoint(host, port);
        msg.addFloatArg(mObjects.ballLoc.x);
        msg.addFloatArg(mObjects.ballLoc.y);
        msg.addFloatArg(mObjects.ballLoc.z);
        sender.sendMessage(msg);
    }
    
    
}

//************************************************************************************
//                        FADE OUT LIGHT
//************************************************************************************

void threeDstuffApp::fadeOut(){
    if(al <= 1 && al > .2){
        al = al * .9; 
       // std::cout<<al<<std::endl
    }
    if(al2 <= 1 && al2 > .4) al2 = al2 * .9;
    if(al3 <= 1 && al3 > .4) al3 = al3 * .9;
    if(al4 <= 1 && al4 > .4) al4 = al4 * .9;
    
}
//************************************************************************************
//                 DRAW GRIDS
//************************************************************************************

void threeDstuffApp::drawGrid(float size, float step)
{
    
    gl::enableAlphaBlending(false);
    gl::color(1.0f, 1.0f, 1.0f, al);
    for(float i=-size; i<=size; i+=step) {
        gl::drawLine(Vec3f(i, 0.0f, -size), Vec3f(i, 0.0f, size) );
        gl::drawLine(Vec3f(-size, 0.0f, i), Vec3f(size, 0.0f, i) );
    }
    for(float i=-size; i<=size; i+=step) {
        gl::drawLine(Vec3f(i, 100.0f, -size), Vec3f(i, 100.0f, size) );
        gl::drawLine(Vec3f(-size, 100.0f, i), Vec3f(size, 100.0f, i) );
    }
    
    
    
}


void threeDstuffApp::drawGridY(float size, float step){
    gl::color( Colorf(0.2f, 0.2f, 0.2f) );
    for(float i=0; i<=size; i+=step) {
        for(float j=-size; j<=size; j+=step) {
            gl::drawLine(Vec3f(-size, i, size), Vec3f(size, i, size) );
            gl::drawLine(Vec3f(j, 0, size), Vec3f(j, size, size) );
        }
    }
    gl::color(1.0f, 0, 0, al2 );
    for(float i=0; i<=size; i+=step) {
        for(float j=-size; j<=size; j+=step) {
            gl::drawLine(Vec3f(-size, i, -size), Vec3f(size, i, -size) );
            gl::drawLine(Vec3f(j, 0, -size), Vec3f(j, size, -size) );
            gl::drawLine(Vec3f(-size, .01, -size), Vec3f(size, .01, -size));
        }
    }
    gl::color(0, 1.0f, 0, al3 );
    for(float i=0; i<=size; i+=step) {
        for(float j=-size; j<=size; j+=step) {
            gl::drawLine(Vec3f(-size, i, size), Vec3f(-size, i, -size) );
            gl::drawLine(Vec3f(-size, 0, j), Vec3f(-size, size, j) );
            gl::drawLine(Vec3f(-100, 0, -99.9), Vec3f(-size, size, -99.9));
            gl::drawLine(Vec3f(-size, .01, size), Vec3f(-size, .01, -size));
        }
    }
    gl::color(0, 0, 1.0f, al4 );
    for(float i=0; i<=size; i+=step) {
        for(float j=-size; j<=size; j+=step) {
            gl::drawLine(Vec3f(size, i, size), Vec3f(size, i, -size) );
            gl::drawLine(Vec3f(size, 0, j), Vec3f(size, size, j) );
            gl::drawLine(Vec3f(size, 0, -99.9), Vec3f(size, size, -99.9));
            gl::drawLine(Vec3f(size, .01, -size), Vec3f(size, .01, size));
            
        }
    }
    
    
}


void threeDstuffApp::mouseMove( MouseEvent event )
{
	// keep track of the mouse
	//mMousePos = event.getPos();
	mMousePos.x = event.getX() - getWindowWidth() * 0.5f;
	mMousePos.y = getWindowHeight() * 0.5f - event.getY();
    
}

void threeDstuffApp::mouseDown( MouseEvent event )
{	
	// let the camera handle the interaction
	mMayaCam.mouseDown( event.getPos() );
}

void threeDstuffApp::mouseDrag( MouseEvent event )
{
	// keep track of the mouse
	mMousePos = event.getPos();
    
	// let the camera handle the interaction
	mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void threeDstuffApp::resize( ResizeEvent event )
{
	// adjust aspect ratio
	CameraPersp cam = mMayaCam.getCamera();
	cam.setAspectRatio( getWindowAspectRatio() );
	mMayaCam.setCurrentCam( cam );
}

bool threeDstuffApp::performPicking( Vec3f *pickedPoint, Vec3f *pickedNormal )
{
	// get our camera 
	CameraPersp cam = mMayaCam.getCamera();
    
	// generate a ray from the camera into our world
	float u = mMousePos.x / (float) getWindowWidth();
	float v = mMousePos.y / (float) getWindowHeight();
	// because OpenGL and Cinder use a coordinate system
	// where (0, 0) is in the LOWERleft corner, we have to flip the v-coordinate
	Ray ray = cam.generateRay(u , 1.0f - v, cam.getAspectRatio() );
    
	// get the bounding box of the model, for fast intersection testing
	//AxisAlignedBox3f objectBounds = Vec3f(1, 1, 1);
    
	// draw this untransformed box in yellow
	gl::color( Color(1, 1, 0) );
	gl::drawStrokedCube(Vec3f::zero(), Vec3f( 6.0f, 6.0f, 6.0f ));
    
	// the coordinates of the bounding box are in object space, not world space,
	// so if the model was translated, rotated or scaled, the bounding box would not
	// reflect that.
	//AxisAlignedBox3f worldBounds = Vec3f(0.1, 0.1, 0.1), Vec3f(1, 1, 1);
    
	// draw this transformed box in cyan
	gl::color( Color(0, 1, 1) );
	gl::drawStrokedCube( Vec3f::zero(), Vec3f( 4.0f, 4.0f, 4.0f ));
    
	// set initial distance to something far, far away
	float result = 1.0e6f;
    
	// fast detection first - test against the bounding box itself
	//if( ! worldBounds.intersects(ray) )
	//	return false;
    
	// traverse triangle list and find the picked triangle
	//size_t polycount = mMesh.getNumTriangles();
	float distance = 0.0f;
	/*
     for(size_t i=0;i<5;++i)
     {
     Vec3f v0, v1, v2;
     // get a single triangle from the mesh
     //mMesh.getTriangleVertices(i, &v0, &v1, &v2);
     
     // transform triangle to world space
     v0 = mTransform.transformPointAffine(v0);
     v1 = mTransform.transformPointAffine(v1);
     v2 = mTransform.transformPointAffine(v2);
     
     // test to see if the ray intersects with this triangle
     if( ray.calcTriangleIntersection(v0, v1, v2, &distance) ) {
     // set our result to this if its closer than any intersection we've had so far
     if( distance < result ) {
     result = distance;
     // assuming this is the closest triangle, we'll set our normal
     // while we've got all the points handy
     *pickedNormal = ( v1 - v0 ).cross( v2 - v0 ).normalized();
     }
     }
     }
     */
    
	// did we have a hit?
	if( distance > 0 ) {
		*pickedPoint = ray.calcPosition( result );
		return true;
	}
	else
		return false;
}


CINDER_APP_BASIC( threeDstuffApp, RendererGl )
