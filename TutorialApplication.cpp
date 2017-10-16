/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
	  |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}
void TutorialApplication::createCamera()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(150, 700, 900));
	//mCamera->setPosition(Ogre::Vector3(150, 10, 900));
	mCamera->lookAt(Ogre::Vector3(150, 0, 0));
	mCamera->setNearClipDistance(5);
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);

}

void TutorialApplication::createViewports()
{
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

}
//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// Create your scene here :)
	
	//Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.8, .8, .8));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	/*Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);*/
	
	

	
	//Ogre::Entity* ogreEntity1 = new Entity[5];
	std::vector<Entity*> entities;
	std::vector<SceneNode*> nodes;
	Real y = 0;
	for (int i = 0; i < 100; i++) {
		entities.push_back(mSceneMgr->createEntity("sphere.mesh"));
		//Ogre::Entity* ogreEntity =  mSceneMgr->createEntity("sphere.mesh");

		nodes.push_back(mSceneMgr->getRootSceneNode()->createChildSceneNode());
		//Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

		nodes[i]->attachObject(entities[i]);
		//ogreNode->attachObject(ogreEntity);
		nodes[i]->scale(.1, .1, .1);
		
		if (i % 10 == 0) {
			y = y + 30;
		}
		nodes[i]->setPosition(30+((i%10) * 30), 10, y);
	}
	
	
	Plane plane(Ogre::Vector3::UNIT_Y, 0);

	MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		1500, 1500, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);

	

	Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/Rockwall");

	

}
//---------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
	{
		// Create application object
		TutorialApplication app;

		try
		{
			app.go();
		}
		catch(Ogre::Exception& e)
		{
			MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
