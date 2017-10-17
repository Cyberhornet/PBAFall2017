/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_
#include<Terrain\OgreTerrain.h>
#include<Terrain\OgreTerrainGroup.h>
#include "BaseApplication.h"

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);

	

protected:
	virtual void createScene(void);
//	virtual void createCamera(void);
  //  virtual void createViewports(void);
	virtual void createFrameListener();
	virtual void destroyScene();
	virtual bool frameRenderingQueued(const FrameEvent& fe);

private:
	void defineTerrain(long x, long y);
	void initBlendMaps(Terrain* terrain);
	void configureTerrainDefaults(Light* light);
	void TutorialApplication::getMinimumPoint(int&minX, int&minY, int&minZ, int delta,int pDelta, Vector3 pos);
	bool mTerrainsImported;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	OgreBites::Label* mInfoLabel;
	std::vector<Entity*> entities;
	std::vector<SceneNode*> nodes;
	int numberOfNodes;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
