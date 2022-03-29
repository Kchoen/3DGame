////////////////////////////////////////
//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/09/20
////////////////////////////////////////
// Student Name: Kuo Chung En
// Student ID: 0716011
// Student Email: kj0716011kj.cs07@nycu.edu.tw
//
////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;
bool moving =false,first=true;
float PenguinV=0,rotateRadius=0;
BasicTutorial_00::BasicTutorial_00(void) {}

void BasicTutorial_00::chooseSceneManager()
{
	mSceneMgrArr[0] = mRoot
		->createSceneManager(ST_GENERIC, "primary");
	mSceneMgrArr[1] = mRoot
		->createSceneManager(ST_GENERIC, "secondary");
}

void BasicTutorial_00::createCamera_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	mCamera = mCameraArr[0] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(120,300,600));
	mCamera->lookAt(Ogre::Vector3(120,0,0));
	mCamera->setNearClipDistance(10);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("SecondCam");
	mCamera->setPosition(Ogre::Vector3(0,350,0.00001));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}



void BasicTutorial_00::createViewport_00(void)
{
	mSceneMgr = mSceneMgrArr[0];
	Camera *Camera1 = mSceneMgr->getCamera("PlayerCam"); 
	Ogre::Viewport* vp = mWindow->addViewport(Camera1,0.0, 0.0, 0.0, 1.0, 1.0);
	mViewportArr[0] = vp;
    vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	Camera1->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


}

void BasicTutorial_00::createViewport_01(void)
{
	
	mSceneMgr = mSceneMgrArr[1];
	Camera *Camera2 = mSceneMgr->getCamera("SecondCam"); 
	Ogre::Viewport* vp = mWindow->addViewport(Camera2,1, 0, 0, 0.25, .25);
	mViewportArr[1] = vp;
    vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	Camera2->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	vp->setOverlaysEnabled(false);
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);


	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 										
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		);
	Entity* groundEntity = mSceneMgr->createEntity("ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	


	Entity *BigPenguin = mSceneMgr->createEntity("BigPenguin", "penguin.mesh");
	SceneNode *BigPenguinNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	BigPenguinNode->attachObject(BigPenguin);
	BigPenguinNode->setPosition(Ogre::Vector3(0, 50, 0));
	BigPenguinNode->scale(Ogre::Vector3(2,3,2));
	Entity *SmallPenguin = mSceneMgr->createEntity("SmallPenguin", "penguin.mesh");
	SceneNode *SmallPenguinNode = BigPenguinNode->createChildSceneNode();
	SmallPenguinNode->attachObject(SmallPenguin);
	SmallPenguinNode->setPosition(Ogre::Vector3(140,-10,0));
	SmallPenguinNode->setScale(0.5,0.33,0.5);
	SmallPenguinNode->yaw(Ogre::Degree(-90));

	int numCubes = 72;
	int L = 255;
	float x=0,y=0,z=125,fx,h,circle_radius,cubeSize,x1,z1,unitF;
	for (int i = 0; i < numCubes; ++i) {
		String name;
		genNameUsingIndex("CircleCube", i, name);
		Entity *ent = mSceneMgr->createEntity(name, "cube.mesh");
		ent->setMaterialName("Examples/SphereMappedRustySteel");
		AxisAlignedBox bb =  ent->getBoundingBox();
		cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *snode = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode();
		snode->attachObject(ent);
		fx = i/(double) (numCubes-1); // in range [0,1]
		h = (1+sin(fx*PI*4))*50; // height
		circle_radius = 100;
		x1 = circle_radius*cos(fx*PI*2);
		z1 = circle_radius*sin(fx*PI*2);
		unitF = 1.0/cubeSize/numCubes*L*0.8;
		snode->scale(unitF, h/cubeSize, unitF);
		snode->setPosition(x1, 50, z1);
		

	}

	for (int i = 0; i < numCubes; ++i) {
		String name;
		genNameUsingIndex("RowCube", i, name);
		Entity *ent = mSceneMgr->createEntity(name, "cube.mesh");
		ent->setMaterialName("Examples/Chrome");
		AxisAlignedBox bb =  ent->getBoundingBox();
		cubeSize = bb.getMaximum().x - bb.getMinimum().x;
		SceneNode *snode = mSceneMgr
			->getRootSceneNode()
			->createChildSceneNode();
		snode->attachObject(ent);
		
		
		fx = 2*i/(double) (numCubes-1); //i from 0 to numCubes-1
		x = fx*L - L/2.0;
		h = (1+cos(fx*3.1415*2.0))*20; // height
		Real unitF = 1.0/cubeSize/numCubes*L*0.8;
		snode->scale(unitF, h/cubeSize, unitF);
		snode->setPosition(x, 20, z);

	}

	Light* Light1 = mSceneMgr->createLight("Light1");
	Light1->setType(Light::LT_POINT);
	SceneNode* Light1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Light1Node->attachObject(Light1);
	Light1->setPosition(Vector3(150, 250, 100)); 
	Light1->setDiffuseColour( 0,1,0 );		
	Light1->setSpecularColour( 1,1,0 );	

	Light* Light2 = mSceneMgr->createLight("Light2");
	Light2->setType(Light::LT_POINT);
	SceneNode* Light2Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Light2Node->attachObject(Light2);
	Light2->setPosition(Vector3(150, 250, 100)); 
	Light2->setDiffuseColour( 0,1,1 );		
	Light2->setSpecularColour( 0,0,1 );	


    
}

void BasicTutorial_00::createScene_01(void) 
{
	mSceneMgr = mSceneMgrArr[1];
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3,0.3));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 										
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		);
	Entity* groundEntity = mSceneMgr->createEntity("ground");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	
	Light* Light1 = mSceneMgr->createLight("Light1");
	Light1->setType(Light::LT_POINT);
	SceneNode* Light1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Light1Node->attachObject(Light1);
	Light1->setPosition(Vector3(100, 150, 250)); 
	Light1->setDiffuseColour( 0,0,1 );		
	Light1->setSpecularColour( 0,0,1 );	


	Entity *ent = mSceneMgr->createEntity("cube", "cube.mesh");
	ent->setMaterialName("Examples/green");
	SceneNode *snode = mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode();
	snode->attachObject(ent);
	snode->setScale(.5,.3,.7);
	snode->setPosition(50,30,0);

}

void BasicTutorial_00::createViewports(void)
{
    //Do not modify
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
    //Do not modify
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
    //Do not modify
	createScene_00();
	createScene_01();
	mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	//mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
//
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyPressed( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyPressed ***\n";
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    
    if (arg.key == OIS::KC_C ) {
        
        //How to clear ss?
        ss.str("");
        ss.clear();
        
        //stringstream ss; // Second way

        // Third way?
        //=============

        // How to get camerea position?
        //-----------------------------
        //This is incorrect.
        //Vector3 pos = mCamera->getPosition();
        //-----------------------------
        Vector3 pos = mCameraMan->getCamera()->getPosition(); //Correct
        ss << std::fixed << std::setprecision(2) 
            << "CameraPosition:" 
            << pos.x << "\t" 
            << pos.y << "\t" 
            << pos.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
        ss.str("");
        ss.clear();
        Vector3 dir = mCameraMan->getCamera()->getDirection();
        ss << std::fixed << std::setprecision(2) 
            << "CameraDirection:" 
            << dir.x << "\t" 
            << dir.y << "\t" 
            << dir.z << "\n";
        Ogre::LogManager::getSingletonPtr()
            ->logMessage( ss.str() );
        //
    }

    if (arg.key == OIS::KC_1 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(98.14,	450.69,	964.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.01,	-0.30,	-0.95));

        //98.14	450.69	964.20
        //-0.01	-0.30	-0.95
    }

    if (arg.key == OIS::KC_2 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1463.00	,606.45	,-513.24));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.88,	-0.47	,0.10));

        // add your own stuff
        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }

    if (arg.key == OIS::KC_3 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(-1356.16,	634.32,	-964.51));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.71,	-0.44,	0.55));


        // add your own stuff
        //-1356.16	634.32	-964.51
        //0.71	-0.44	0.55
    }

    if (arg.key == OIS::KC_4 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(40.39,	155.23,	251.20));
        mCameraMan->getCamera()
            ->setDirection(Vector3(-0.02,	-0.41,	-0.91));

         // add your own stuff
        //40.39	155.23	251.20
        //-0.02	-0.41	-0.91
    }

    if (arg.key == OIS::KC_5 ) {
		mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,	822.63,	30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));

        // add your own stuff
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

    if (arg.key == OIS::KC_M ) {
		mWindow->removeViewport(mViewportArr[0]->getZOrder());
		mWindow->removeViewport(mViewportArr[1]->getZOrder());

        Camera *c_ptr = mCameraArr[1];
	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr,
        0,
        0.0,
        0,
        1,
        1
        );
	vp->setOverlaysEnabled(false);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[1] = vp;     




		c_ptr = mCameraArr[0];
		vp = mWindow->addViewport(
        c_ptr,
        1,
        0.0,
        0,
        0.45,
        0.3
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,1,0));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;       // make sure to save the new pointer

	  

    
    }

    if (arg.key == OIS::KC_N ) {
        mWindow->removeViewport(mViewportArr[0]->getZOrder());
		mWindow->removeViewport(mViewportArr[1]->getZOrder());

        Camera *c_ptr = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr,
        0,
        0.0,
        0,
        1,
        1
        );
	vp->setBackgroundColour(Ogre::ColourValue(0,1,0));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[0] = vp;     




		c_ptr = mCameraArr[1];
		vp = mWindow->addViewport(
        c_ptr,
        1,
        0.55,
        0.3,
        0.45,
        0.3
        );
		vp->setOverlaysEnabled(false);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	c_ptr->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
   //delete mViewportArr[0];    // program crashes
    mViewportArr[1] = vp;       // make sure to save the new pointer

	  
    }
	/*if (arg.key == OIS::KC_B ){
		Ogre::Vector3 pos = mCameraManArr[1]->getCamera()->getPosition();
		std::cout<<"NOW CAMERA POS::"<<pos<<".\n";
		mCameraManArr[1]->getCamera()->setPosition(Ogre::Vector3(pos[0],pos[1],pos[2]+1));
		std::cout<<"NOW CAMERA DIRECTION::"<<mCameraManArr[1]->getCamera()->getDirection()<<".\n";
		mCameraManArr[1]->getCamera()->lookAt(Ogre::Vector3(0,0,0));
		std::cout<<"THE CUBE POS:: "<<mSceneMgrArr[1]->getEntity("cube")->getParentSceneNode()->getPosition()<<",\n";
	}
	if (arg.key == OIS::KC_S ){
		Ogre::Vector3 pos = mCameraManArr[1]->getCamera()->getPosition();
		std::cout<<"NOW ZPOS::"<<pos[2]-1<<".\n";
		mCameraManArr[1]->getCamera()->setPosition(Ogre::Vector3(pos[0],pos[1],pos[2]-1));
	}*/
	if (arg.key == OIS::KC_P ) {
		mSceneMgr = mSceneMgrArr[0];
		if(first){
			mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(90));
			mSceneMgr->getEntity("SmallPenguin")->getParentSceneNode()->setPosition(0,-10,140);
			mSceneMgr->getEntity("SmallPenguin")->getParentSceneNode()->yaw(Ogre::Degree(-90));
			first = false;
		}


		if (moving == false)
			moving = true;
		else
			moving = false;
	
	
	}



    // Do not delete this line
    BaseApplication::keyPressed(arg);

    return flg;
}

//
// What is stored in the file for arg.key?
// ASCII code? If no, what is it?
// 
// To find out the answer:
// Go to see the definition of KeyEvent
//
bool BasicTutorial_00::keyReleased( const OIS::KeyEvent &arg )
{
    bool flg = true;
    stringstream ss;
    ss << arg.key;
    String msg;
    ss >> msg;
    msg += ":*** keyReleased ***\n";
    
    Ogre::LogManager::getSingletonPtr()->logMessage( msg );

    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    
	if(moving){
		mSceneMgr = mSceneMgrArr[0];
		if (PenguinV<0.1){
			PenguinV+=0.0002;

		}
		if (rotateRadius<359.8){
			mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(-(PenguinV)));
			rotateRadius+=PenguinV;
		}
		else if(rotateRadius<360){
			mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(-(360-rotateRadius)));
			rotateRadius=360;
			PenguinV=0;
		}
		else if(rotateRadius<719.8){
			mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(PenguinV));
			rotateRadius+=PenguinV;
		}
		else{
			mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree((720-rotateRadius)));
			rotateRadius=0;
			PenguinV=0;
		
		}

	
	}


    return flg;
}
int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}

////////////////////////////////////////
// DO NOT DELETE THIS LINE: 2018/09/20. 3D Game Programming
////////////////////////////////////////