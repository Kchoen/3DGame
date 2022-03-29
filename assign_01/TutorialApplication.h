//
// 3D Game Programming
// NCTU
// Instructor: SAI-KEUNG WONG
//
/*!
\brief 3D Game Programming
\n
My Name: Kuo Chung En
\n
My ID: 0716011
\n
My Email: kj0716011kj.cs07@nycu.edu.tw
\n Date: 2021/10/02

This is an assignment of 3D Game Programming
*/

////////////////////////////////////////
// You can delete or add some functions to do the assignment.
////////////////////////////////////////

#ifndef __BasicTutorial_00_h_
#define __BasicTutorial_00_h_

#include "BaseApplication.h"

class BasicTutorial_00 : public BaseApplication
{
public:
	BasicTutorial_00(void);
	/*!
	\brief 
	Create two SceneManager

	and store them into the **mSceneMgrArr** for following use

	*/
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createScene(void);
	
	
    /*!
	\brief
	Fuction handling framewise routine.(Penguin Animation)

	Every frame, go check if some events happened.

	In this function, the animation of two penguins' rotation is implemented here.
	Checking the bool **moving** whether true(start animation)

	By previous settings, **the SmallPenguin is childNode of BigPenguin**, and **watching each other**.

	We can simply use 
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
	mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(-PenguinV));
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Making BigPenguin rotate, and SmallPenguin will follow the BigPenguin


	@param bool moving : Control the RotatingAnimation starting or not
	@param float PenguinV : Speed of Penguins' Rotation
	@param float rotateRadius : Total Sum of Penguin's Rotation(Control the clockwise)
	*/
    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    //
    // Add your own stuff.
    //
protected:
	/*!
	\brief 

	Create a viewport for the entire screen by the first camera.
	~~~.cpp
	Ogre::Viewport* vp = mWindow->addViewport(Camera1,0.0, 0.0, 0.0, 1.0, 1.0);
	~~~

	and set the screen's background color to blue
	~~~.cpp
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	~~~

	@param viewport vp : Establish the port from camera to rendering window

	*/
	void createViewport_00(void);
	/*!
	\brief 

	Create a viewport for the 1/4 screen by the second camera at left upper corner.
	~~~.cpp
	Ogre::Viewport* vp = mWindow->addViewport(Camera2,1, 0, 0, 0.25, .25);
	~~~

	set the screen's background color to blue
	~~~.cpp
	vp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	~~~

	and turn off the Overlay
	~~~.cpp
	vp->setOverlaysEnabled(false);
	~~~

	@param viewport vp : Establish the port from camera to rendering window

	*/
	void createViewport_01(void);
	//
	/*!
	\brief 

	S1mply create the first camera 

	and setup its **Position** and **Direction**

	*/
	void createCamera_00();
	//
	/*!
	\brief 

	S1mply create the second camera 

	and setup its **Position** and **Direction**

	*/
	void createCamera_01();
	//
	/*!
	\brief 
	Setup the MainScene of the Program

	Control the first SceneManager, and build entities and SceneNodes.. upon it.

	###At the beginning, early settings setup

	~~~.cpp
	mSceneMgr = mSceneMgrArr[0];
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	~~~

	###Starting entities building


	The **ground** built by given fuction
	~~~.cpp
	MeshManager::getSingleton().createPlane()
	~~~

	and each entity can be attached to SceneNode by
	~~~.cpp
	SceneNode->attachObject(Entity);
	~~~

	The **Big Penguin** SceneNode built under **SceneMgr** and
	**Small Penguin** build under **Big Penguin**

	Because of this relationship, the **Small Penguin** will maintain the relative position with **Big Penguin**

	-> by 1 yaw function controlling **BigPenguin**, the animation is finished.(No need of Calculation)

	Very useful relationship for the later animation.
	~~~.cpp
	SceneNode *BigPenguinNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	SceneNode *SmallPenguinNode = BigPenguinNode->createChildSceneNode();
	~~~
	and setup their **Position** and **DIrection**

	###Finish building Entities and Nodes

	The same prosedure, finish establish **cubes** and  **lights**

	~~~.cpp
	snode->attachObject(ent);
	Light1Node->attachObject(Light1);
	...
	~~~

	*/
	void createScene_00();
	/*!
	\brief 
	Setup the SecondScene of the Program

	Control the second SceneManager, and build entities and SceneNodes.. upon it.

	The buildup is similar to createScene_00()


	Early settings setup **Light, Shadow**

	~~~.cpp
	mSceneMgr = mSceneMgrArr[1];
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	~~~
	Setup **ground, light, cube**
	~~~.cpp
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	Light1Node->attachObject(Light1);
	snode->attachObject(ent);
	~~~
	

	*/
	void createScene_01();
	/*!
	\brief 
	Fuction handles KeyPress event

	###For the 1~5 KeyEvent

	simply set Camera position and direction

	~~~.cpp
	mCameraMan->getCamera()
            ->setPosition(Vector3(19.94,	822.63,	30.79));
    mCameraMan->getCamera()
            ->setDirection(Vector3(0.00,	-0.99,	-0.11));
	~~~

	###For m,n KeyEvent

	simply removeViewport by
	~~~.cpp
	mWindow->removeViewport(mViewportArr[0]->getZOrder());
	mWindow->removeViewport(mViewportArr[1]->getZOrder());
	~~~
	
	and follow previous createViewport_00() and createViewport_01()
	
	add the viewport back.
	~~~.cpp
	Ogre::Viewport* vp = mWindow->addViewport(
        c_ptr,
        0,
        0.0,
        0,
        1,
        1
        );
	vp = mWindow->addViewport(
        c_ptr,
        1,
        0.0,
        0,
        0.45,
        0.3
        );
	~~~

	###For the p KeyEvent
	add a bool to check if it is the first click
	
	no->only control the PenguinAnimation by
	~~~.cpp
	if (moving == false)
			moving = true;
	else
			moving = false;
	~~~

	yes->set the relative position of **BIGPENGUIN** and **SMALLPENGUIN** first
	, set the **first bool** to false
	, and control the PenguinAnimation
	~~~.cpp
	mSceneMgr->getEntity("BigPenguin")->getParentSceneNode()->yaw(Ogre::Degree(90));
	mSceneMgr->getEntity("SmallPenguin")->getParentSceneNode()->setPosition(0,-10,140);
	mSceneMgr->getEntity("SmallPenguin")->getParentSceneNode()->yaw(Ogre::Degree(-90));
	first = false;
	~~~

	@param bool first : Check whether it is the first click
	@param bool moving : Control the PenguinAnimation

	*/
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );
    //
    // Add your own stuff.
    //
protected:
    Ogre::Viewport* mViewportArr[8];
	Ogre::Camera* mCameraArr[8];
	Ogre::SceneManager* mSceneMgrArr[8];
	OgreBites::SdkCameraMan* mCameraManArr[8];
	
	
    //
    // Add your own stuff.
    //
};


#endif // #ifndef __BasicTutorial_00_h_