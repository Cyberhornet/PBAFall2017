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
: mTerrainGroup(0),
mTerrainGlobals(0),
mInfoLabel(0)
{
	numberOfNodes = 100;
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

void TutorialApplication::destroyScene()
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	bool ret = BaseApplication::frameRenderingQueued(fe);

	for (int i = 0; i < numberOfNodes;i++)
	{
      
		Vector3 pos= nodes[i]->getPosition();
		pos.x = pos.x - (rand() % 4);
		pos.y = pos.y - (rand() % 4);
		pos.z = pos.z - (rand() % 4);
		nodes[i]->setPosition(pos);
		//nodes[i]->translate()
	}



	return ret;
}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain1.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

void TutorialApplication::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	bool exists =
		Ogre::ResourceGroupManager::getSingleton().resourceExists(
			mTerrainGroup->getResourceGroup(),
			filename);
	if (exists)
		mTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		mTerrainGroup->defineTerrain(x, y, &img);

		mTerrainsImported = true;
	}
}

void TutorialApplication::initBlendMaps(Ogre::Terrain* terrain)
{
	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

	float* pBlend0 = blendMap0->getBlendPointer();
	float* pBlend1 = blendMap1->getBlendPointer();

	for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	{
		for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
		{
			Ogre::Real tx, ty;

			blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
			Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
			Ogre::Real val = (height - minHeight0) / fadeDist0;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend0++ = val;

			val = (height - minHeight1) / fadeDist1;
			val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
			*pBlend1++ = val;
		}
	}

	blendMap0->dirty();
	blendMap1->dirty();
	blendMap0->update();
	blendMap1->update();
}

void TutorialApplication::configureTerrainDefaults(Ogre::Light* light)
{
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(3000);
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
	Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = 6000.0;
	importData.inputScale = 600;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;
	importData.layerList.resize(3);
	importData.layerList[0].worldSize = 100;
	importData.layerList[0].textureNames.push_back(
		"dirt_grayrocky_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back(
		"dirt_grayrocky_normalheight.dds");
	importData.layerList[1].worldSize = 30;
	importData.layerList[1].textureNames.push_back(
		"grass_green-01_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back(
		"grass_green-01_normalheight.dds");
	importData.layerList[2].worldSize = 200;
	importData.layerList[2].textureNames.push_back(
		"growth_weirdfungus-03_diffusespecular.dds");
	importData.layerList[2].textureNames.push_back(
		"growth_weirdfungus-03_normalheight.dds");
}
void TutorialApplication::createFrameListener()
{
	BaseApplication::createFrameListener();
	mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TerrainInfo", "", 350);
	if (mTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
		mInfoLabel->show();

		if (mTerrainsImported)
			mInfoLabel->setCaption("Building terrain...");
		else
			mInfoLabel->setCaption("Updating terrain...");
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(mInfoLabel);
		mInfoLabel->hide();

		if (mTerrainsImported)
		{
			mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}

}

//void TutorialApplication::createCamera()
//{
//	mCamera = mSceneMgr->createCamera("PlayerCam");
//	//mCamera->setPosition(Ogre::Vector3(150, 700, 900));
//	////mCamera->setPosition(Ogre::Vector3(150, 10, 900));
//	//mCamera->lookAt(Ogre::Vector3(150, 0, 0));
//	//mCamera->setNearClipDistance(5);
//	
//
//	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
//	mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
//	mCamera->setNearClipDistance(0.1);
//
//	bool infiniteClip =
//		mRoot->getRenderSystem()->getCapabilities()->hasCapability(
//			Ogre::RSC_INFINITE_FAR_PLANE);
//
//	if (infiniteClip)
//		mCamera->setFarClipDistance(0);
//	else
//		mCamera->setFarClipDistance(50000);
//	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
//}

//void TutorialApplication::createViewports()
//{
//	Viewport* vp = mWindow->addViewport(mCamera);
//	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
//	mCamera->setAspectRatio(
//		Ogre::Real(vp->getActualWidth()) /
//		Ogre::Real(vp->getActualHeight()));
//
//}
//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// Create your scene here :)
	//////Sky
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	/////Camera

	mCamera->setPosition(Ogre::Vector3(1283,700, 1916));
	mCamera->lookAt(Ogre::Vector3(1263, 550, 1160));
	mCamera->setNearClipDistance(0.1);

	bool infiniteClip =
		mRoot->getRenderSystem()->getCapabilities()->hasCapability(
			Ogre::RSC_INFINITE_FAR_PLANE);

	if (infiniteClip)
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(50000);

	//Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.8, .8, .8));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
	lightdir.normalise();

	Ogre::Light* light = mSceneMgr->createLight("TestLight");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(lightdir);
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));


	
	

	


	Real y = 0;
	for (int i = 0; i < numberOfNodes; i++) {
		entities.push_back(mSceneMgr->createEntity("sphere.mesh"));

		
		nodes.push_back(mSceneMgr->getRootSceneNode()->createChildSceneNode());


		nodes[i]->attachObject(entities[i]);

		nodes[i]->scale(.1, .1, .1);
		
		if (i % 10 == 0) {
			y = y + 30;
		}
		nodes[i]->setPosition(30+((i%10) * 30), 700, y);
	}
	

	
	//Plane plane(Ogre::Vector3::UNIT_Y, 0);

	//MeshManager::getSingleton().createPlane(
	//	"ground",
	//	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//	plane,
	//	1500, 1500, 20, 20,
	//	true,
	//	1, 5, 5,
	//	Ogre::Vector3::UNIT_Z);

	//

	//Entity* groundEntity = mSceneMgr->createEntity("ground");
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	//groundEntity->setCastShadows(false);
	//groundEntity->setMaterialName("Examples/Rockwall");

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
		mSceneMgr,
		Ogre::Terrain::ALIGN_X_Z,
		513, 6000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	configureTerrainDefaults(light);
	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);

	mTerrainGroup->loadAllTerrains(true);
	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();

		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}
	mTerrainGroup->freeTemporaryResources();
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
