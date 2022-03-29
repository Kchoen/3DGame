//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
// Date: 2019/10/28
//
#include "TutorialApplication.h"
#include "BasicTools.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace Ogre;

const float PI = 3.141592654;
int Index = 0;
#define mMoveDirection_NONE 0
#define mMoveDirection_DOWN (1 << 0)
#define mMoveDirection_UP   (1 << 1)
#define mMoveDirection_LEFT (1 << 2)
#define mMoveDirection_RIGHT (1<<3)


BasicTutorial_00::BasicTutorial_00(void)
    : 
mMoveDirection(mMoveDirection_NONE)
{}

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
	mCamera->setNearClipDistance(5);
	mCameraManArr[0] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}

void BasicTutorial_00::createCamera_01(void)
{
	mSceneMgr = mSceneMgrArr[1];
	mCamera = mCameraArr[1] = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,350,0.0));
	mCamera->lookAt(Ogre::Vector3(0.00001,0,0));
	mCamera->setNearClipDistance(5);
	mCameraManArr[1] = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller

}



void BasicTutorial_00::createViewport_00(void)
{
	mCamera = mCameraArr[0];
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0.0,1.0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    mViewportArr[0] = vp;
}

void BasicTutorial_00::createViewport_01(void)
{
}

void BasicTutorial_00::resolveCollision(
    SceneNode *nodeA, SceneNode *nodeB,
    float rA, float rB, float wA, float wB)
{
    Vector3 posA = nodeA->getPosition();
    Vector3 posB = nodeB->getPosition();
    float R = rA + rB;
	float W = wA + wB;
	Sphere sph1(posA - Vector3(0,posA.y,0),rA);
	Sphere sph2(posB - Vector3(0,posB.y,0),rB);
	Vector3 movingVec = sph2.getCenter() - sph1.getCenter();
	if(movingVec.isZeroLength())	movingVec = Vector3::UNIT_X;
    if(movingVec.length() < R){
		float distanceA = wB / W;
		float distanceB = wA / W;
		movingVec.normalise();
		Vector3 destA = distanceA * -movingVec;
		Vector3 destB = distanceB * movingVec;
		nodeA->setPosition(posA + destA);
		nodeB->setPosition(posB + destB);
	}
}

void BasicTutorial_00::resolveCollisionLargeSphere()
{
    float smallR = 15; // small sphere radius

    float largeR = 1.0/0.15*smallR; // large sphere radius
	SceneNode *nodeL = mLargeSphereSceneNode;
	for (int i = 0; i < mNumSpheres; ++i)
	{
		SceneNode *nodeS = mSceneNode[i];
		resolveCollision(nodeL, nodeS, largeR, smallR, 1,0);
	}
	for (int i = 0; i < 80; ++i)
	{
		SceneNode *nodeB = barrel[i];
		resolveCollision(nodeL, nodeB, largeR, 28, 0,1);
	}
}

// perform collision handling for all pairs
void BasicTutorial_00::resolveCollisionSmallSpheres()
{
    float ri = 15; // sphere radius
    float rj = 15; // sphere radius
    for (int i = 0; i < mNumSpheres; ++i)
	{
		SceneNode* node1 = mSceneNode[i];
		for (int j = i+1; j < mNumSpheres; ++j) {
			SceneNode* node2 = mSceneNode[j];	
			resolveCollision(node1, node2, ri, rj, 1,1);
        }
		for (int k = 0; k < 80; ++k)
		{
			SceneNode *nodeB = barrel[k];
			resolveCollision(node1, nodeB, ri, 28, 0,1);
		}
    }
}

void BasicTutorial_00::resolveCollision()
{
    int num = 10;
    for (int i = 0; i < num;++i) {
        resolveCollisionSmallSpheres();
        resolveCollisionLargeSphere();
    }
}

// reset positions of all small spheres
void BasicTutorial_00::reset()
{
	mLargeSphereSceneNode->setPosition(0,0,0);
    for (int i = 0; i < mNumSpheres; ++i ) {
        mSceneNode[i]->setPosition(rand()%801-400,0,rand()%801-400);
    }
	resolveCollision();
}

// create all spheres
// "Examples/red"
// "Examples/green"
// "Examples/blue"
// "Examples/yellow"
void BasicTutorial_00::createSpace()
{
    String name_en;
    String name_sn;
	//SmallShpere
    mNumSpheres = 300;
    for (int i = 0; i < 500; ++i ) {
		genNameUsingIndex("R1", Index, name_en);
		genNameUsingIndex("S1", Index, name_sn);
		mEntity[Index] = mSceneMgr
			->createEntity( name_en, "sphere.mesh" );

		mSceneNode[Index] = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mSceneNode[Index]->attachObject(mEntity[Index]);

    switch(rand()%3) {
    case 0:
        mEntity[Index]->setMaterialName("Examples/red");
        break;
    case 1:
		mEntity[Index]->setMaterialName("Examples/green");
        break;
    case 2:
		mEntity[Index]->setMaterialName("Examples/blue");
        break;
    }
    //scale the small spheres
    mSceneNode[Index]->scale(0.15, 0.15, 0.15);
	mSceneNode[Index]->setPosition(rand()%801-400,0,rand()%801-400);
	Index++;
    }
	for(int i =0;i<mNumSpheres;i++){
			mSceneNode[i]->setVisible(true);
		}
		for(int i =mNumSpheres;i<500;i++){
			mSceneNode[i]->setVisible(false);
		}
	//BigShpere
	mLargeSphereEntity = mSceneMgr
		->createEntity( "LargeSphere", "sphere.mesh" );
	mLargeSphereEntity->setMaterialName("Examples/yellow");
	mLargeSphereSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLargeSphereSceneNode->attachObject(mLargeSphereEntity);
	mLargeSphereSceneNode->setPosition(0,0,0);
	//barrel
	int curIndex=Index;
	//wall left
    for (int i = 0; i < 20; ++i) {
		genNameUsingIndex("R1", Index, name_en);
        genNameUsingIndex("S1", Index, name_sn);
        Entity *ent = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
        barrel[Index-curIndex] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn); 
        barrel[Index-curIndex]->scale(10.0, 10.0, 10.0);
        barrel[Index-curIndex]->attachObject(ent);
        barrel[Index-curIndex]->setPosition(-600.0+2*30*i,10,-600);
		Index++;
    }
	//wall front
	for (int i = 0; i < 20; ++i) {
		genNameUsingIndex("R1", Index, name_en);
        genNameUsingIndex("S1", Index, name_sn);
        Entity *ent = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
        barrel[Index-curIndex] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn); 
        barrel[Index-curIndex]->scale(10.0, 10.0, 10.0);
        barrel[Index-curIndex]->attachObject(ent);
        barrel[Index-curIndex]->setPosition(600.0,10,-600+2*30*i);
		Index++;
    }
	//wall right
	for (int i = 0; i < 20; ++i) {
		genNameUsingIndex("R1", Index, name_en);
        genNameUsingIndex("S1", Index, name_sn);
        Entity *ent = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
        barrel[Index-curIndex] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn); 
        barrel[Index-curIndex]->scale(10.0, 10.0, 10.0);
        barrel[Index-curIndex]->attachObject(ent);
        barrel[Index-curIndex]->setPosition(600.0-2*30*i,10,600);
		Index++;
    }
	//wall back
	for (int i = 0; i < 20; ++i) {
		genNameUsingIndex("R1", Index, name_en);
        genNameUsingIndex("S1", Index, name_sn);
        Entity *ent = mSceneMgr->createEntity( name_en, "Barrel.mesh" );
        barrel[Index-curIndex] = mSceneMgr->getRootSceneNode()->createChildSceneNode(name_sn); 
        barrel[Index-curIndex]->scale(10.0, 10.0, 10.0);
        barrel[Index-curIndex]->attachObject(ent);
        barrel[Index-curIndex]->setPosition(-600.0,10,600-2*30*i);
		Index++;
    }
    
}

void BasicTutorial_00::createScene_00(void) 
{
	mSceneMgr = mSceneMgrArr[0];
	//mSceneMgr->setAmbientLight( ColourValue( 0.25, 0.25, 0.25 ) ); 

	mSceneMgr->setAmbientLight( ColourValue( 0.5, 0.5, 0.5 ) ); 
	//mSceneMgr->setAmbientLight( ColourValue( 1, 1, 1 ) );  
	mSceneMgr->setShadowTechnique(
		SHADOWTYPE_STENCIL_ADDITIVE); 

	Light *light;
	light = mSceneMgr->createLight("Light1"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(150, 250, 100)); 
	light->setDiffuseColour(0.3, 0.3, 0.3);		
	light->setSpecularColour(0.5, 0.5, 0.5);	

	light = mSceneMgr->createLight("Light2"); 
	light->setType(Light::LT_POINT); 
	light->setPosition(Vector3(-150, 300, 250)); 
	light->setDiffuseColour(0.25, 0.25, 0.25);		
	light->setSpecularColour(0.35, 0.35, 0.35);	

	//
	Plane plane(Vector3::UNIT_Y, 0); 
	MeshManager::getSingleton().createPlane(
		"ground", 						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		plane, 
		1500,1500, 	// width, height
		20,20, 		// x- and y-segments
		true, 		// normal
		1, 			// num texture sets
		5,5, 		// x- and y-tiles
		Vector3::UNIT_Z	// upward vector
		); 

	Entity *ent = mSceneMgr->createEntity(
		"GroundEntity", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	//ent->setMaterialName("Examples/Rockwall");


	mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode()
		->attachObject(ent); 

    ent = mSceneMgr->createEntity(
		"Background", "ground"); 
	//ent->setMaterialName("Examples/BeachStones");
	ent->setMaterialName("Examples/Rockwall");

    //Radian angle((45/180.0)*3.141592654);
    Radian angle(3.141952654/2.0);

    Vector3 axis(1.0, 0.0, 0.0);
    mSceneMgr
		->getRootSceneNode()
		->createChildSceneNode(
            Vector3(0.0, 0.0, -750.0),
            Quaternion( angle, axis))
		->attachObject(ent);

    createSpace();

    ///////////////////////
        // add your own stuff
        ///////////////////////
    
    resolveCollision();
}

void BasicTutorial_00::createScene_01(void) 
{

}

void BasicTutorial_00::createViewports(void)
{
	createViewport_00();
	createViewport_01();
}

void BasicTutorial_00::createCamera(void) {
	createCamera_00();
	createCamera_01();
	mCameraMan = mCameraManArr[0];
	//mCameraMan = mCameraManArr[1];
}

void BasicTutorial_00::createScene( void ) {
	createScene_00();
	createScene_01();
	//mSceneMgr = mSceneMgrArr[0]; // active SceneManager
	mSceneMgr = mSceneMgrArr[1]; // active SceneManager
    //
    mCamera = mCameraArr[0];
    //mCamera = mCameraArr[1];
    //
    mCameraMan->getCamera()
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

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
            ->setPosition(Vector3(-22.30,	409.24,	816.74));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.02,	-0.23,	-0.97));

    }

    if (arg.key == OIS::KC_2 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(-824.52,	468.58,	68.45));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.94,	-0.31,	-0.11));

        //-1463.00	606.45	-513.24
        //0.88	-0.47	0.10
    }


    if (arg.key == OIS::KC_3 ) {
        mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,	822.63,	30.79));
        mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));
        //19.94	822.63	30.79
        //0.00	-0.99	-0.11
    }

	if (arg.key == OIS::KC_U ) {
		if(Udir==false)
			mVz += -2;
			Udir = true;
		
    }
	if (arg.key == OIS::KC_J ) {
		if(Jdir==false)
			mVz += 2;
			Jdir = true;
    }
	if (arg.key == OIS::KC_H ) {
		if(Hdir==false)
			mVx += -2;
			Hdir = true;
    }
	if (arg.key == OIS::KC_K ) {
		if(Kdir==false)
			mVx += 2;
			Kdir = true;
    }

    if (arg.key == OIS::KC_B ) {
		reset();
    }
	if (arg.key == OIS::KC_7 ) {
		mNumSpheres = 100;
		for(int i =0;i<mNumSpheres;i++){
			mSceneNode[i]->setVisible(true);
		}
		for(int i =mNumSpheres;i<500;i++){
			mSceneNode[i]->setVisible(false);
		}
		reset();
    }
	if (arg.key == OIS::KC_8 ) {
		mNumSpheres = 200;
		for(int i =0;i<mNumSpheres;i++){
			mSceneNode[i]->setVisible(true);
		}
		for(int i =mNumSpheres;i<500;i++){
			mSceneNode[i]->setVisible(false);
		}
		reset();
    }
	if (arg.key == OIS::KC_9 ) {
		mNumSpheres = 300;
		for(int i =0;i<mNumSpheres;i++){
			mSceneNode[i]->setVisible(true);
		}
		for(int i =mNumSpheres;i<500;i++){
			mSceneNode[i]->setVisible(false);
		}
		reset();
    }
	if (arg.key == OIS::KC_0 ) {
		mNumSpheres = 500;
		for(int i =0;i<mNumSpheres;i++){
			mSceneNode[i]->setVisible(true);
		}
		reset();
    }


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

	if (arg.key == OIS::KC_U ) {
		mVz -= -2;
		Udir = false;
    }
	if (arg.key == OIS::KC_J ) {
		mVz -= 2;
		Jdir = false;
    }
	if (arg.key == OIS::KC_H ) {
		mVx -= -2;
		Hdir = false;
    }
	if (arg.key == OIS::KC_K ) {
		mVx -= 2;
		Kdir = false;
    }


    BaseApplication::keyReleased(arg);

    return flg;
}

bool BasicTutorial_00::frameStarted(const Ogre::FrameEvent& evt)
{
	bool flg = Ogre::FrameListener::frameStarted(evt);
    //
    Vector3 mdir;
    if (mMoveDirection & mMoveDirection_UP ) {
        mdir += Vector3(0.0, 0.0, -1.0);
    }
	Vector3 pos = mLargeSphereSceneNode->getPosition();
	pos[0] = pos[0]+mVx;
	pos[2] = pos[2]+mVz;
	mLargeSphereSceneNode->setPosition(pos);
    ///////////////////////
    // add your own stuff
    ///////////////////////

    resolveCollision();
    //
    return flg;
}

int main(int argc, char *argv[]) {
	BasicTutorial_00 app;
	app.go();  
	return 0;
}
