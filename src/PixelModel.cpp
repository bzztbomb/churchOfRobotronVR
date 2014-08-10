//
//  PixelModel.cpp
//  churchOfRobotronVR
//
//  Created by Brian Richardson on 1/22/14.
//
//

#include "PixelModel.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include "churchUtil.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

void PixelModel::init(cinder::params::InterfaceGl* params)
{
  mScale = Vec3f(0.46f, 0.46f, 0.46f);
	mRotationRads = 0.0f;
  mPosition = Vec3f(0.0f, 14.0f, -1.27f);
  params->addSeparator(mParamPrefix);
  params->addParam(mParamPrefix + ": Position", &mPosition);
  params->addParam(mParamPrefix + ": Animate", &mAnimate);
  params->addParam(mParamPrefix + ": Scale", &mScale);

  mCurrFrame = 0;
  mAnimate = true;
	
	mAlwaysFaceAltar = true;
}

void PixelModel::loadFrames( const std::vector<cinder::Area>& frames )
{
  Surface8u allSprites = loadImage(loadAsset("robotron_sprites_complete.png"));
	
  for (auto f : frames)
  {
    Surface8u s = allSprites.clone(f);
    mFrames.push_back(VboMesh::create(cor::spriteToMesh(s)));
  }
	
	// Reset mTimer
	mTimer = new Timer(true);	// new Timer object, and start automatically.
}

void PixelModel::setFPS( float inFPS ) {
	mFPS = inFPS;
}


void PixelModel::update()
{
  if (!mFrames.size())
    return;
  
  int totalFrames = mTimer.getSeconds() * mFPS;
  mCurrFrame = totalFrames % mFrames.size();
}

void PixelModel::draw()
{
	if( !mAnimate ) return;
	
	if( mAlwaysFaceAltar ) {
		mRotationRads = atan2f( mPosition.y, mPosition.x ) + M_PI*0.5f;
	}
	
  gl::disable(GL_TEXTURE);
  gl::disable(GL_TEXTURE_2D);
  gl::color(Color::white());
  gl::pushMatrices();
  gl::translate(mPosition);
  gl::scale(mScale);
	gl::rotate(Vec3f( 0, 0, mRotationRads*(180/M_PI) ));
  gl::draw(mFrames[mCurrFrame]);
  gl::popMatrices();
}
