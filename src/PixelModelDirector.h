//
//  PixelModelDirector.h
//  churchOfRobotronVR
//
//  Created by Zach Archer on 8/9/14.
//
//

#ifndef __churchOfRobotronVR__PixelModelDirector__
#define __churchOfRobotronVR__PixelModelDirector__

#include "cinder/TriMesh.h"
#include "cinder/params/Params.h"
#include "cinder/gl/Vbo.h"
#include "PixelModel.h"

class PixelModelDirector
{

public:
	static PixelModelDirector& getInstance()
	{
		static PixelModelDirector instance;
		return instance;
	}
	
	std::vector<cinder::gl::VboMeshRef> animationMeshesForKey( std::string key );
	
	void init( cinder::params::InterfaceGl* params );
	void update();
	void draw();
	
private:
	// Singleton pattern
	PixelModelDirector(){}
	PixelModelDirector( PixelModelDirector const& );
	void operator=(PixelModelDirector const& );

	static std::vector<cinder::Area> walkAreas( cinder::Area home, int offsetX, BOOL dipHomeFrame );

	void cacheAnimation( cinder::Surface8u allSprites, std::string key, std::vector<cinder::Area> areas );
	std::map< std::string, std::vector<cinder::gl::VboMeshRef> > mAnimations;
	std::vector<PixelModel*> mModels;
	
	cinder::Timer mTimer;
	double mPrevSeconds;
};

#endif /* defined(__churchOfRobotronVR__PixelModelDirector__) */
