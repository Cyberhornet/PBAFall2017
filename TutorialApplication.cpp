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

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// Create your scene here :)
	mCamera->setPosition(Ogre::Vector3(150, 700, 900));
	mCamera->lookAt(Ogre::Vector3(150, 0, 0));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	
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
		nodes[i]->setPosition(30+((i%10) * 30), 0, y);
	}
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	
	Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Plane plane2(Vector3::UNIT_X, 0);
	MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		500, 1000, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);

	MeshManager::getSingleton().createPlane(
		"wall",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane2,
		500, 1000, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);

	Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/Rockwall");

	Entity* groundEntity1 = mSceneMgr->createEntity("wall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity1);
	groundEntity1->setCastShadows(false);
	groundEntity1->setMaterialName("Examples/Rockwall");

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
