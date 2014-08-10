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
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;

inline cinder::Vec3f lerpVec3( Vec3f a, Vec3f b, float p ) {
	return Vec3f( lerp(a.x,b.x,p), lerp(a.y,b.y,p), lerp(a.z,b.z,p) );
}

void PixelModel::init(cinder::params::InterfaceGl* params)
{
  mScale = Vec3f(0.46f, 0.46f, 0.46f);
	mRotationRads = 0.0f;
  mPosition = Vec3f(0.0f, 14.0f, -1.27f);
  params->addSeparator(mParamPrefix);
  params->addParam(mParamPrefix + ": Position", &mPosition);
  params->addParam(mParamPrefix + ": Scale", &mScale);

  mCurrFrame = 0;
}

void PixelModel::clearMovements() {
	mMovements.resize( 0 );
}

void PixelModel::appendMovement( ModelMovement* movement )
{
	mMovements.push_back( movement );
}

void PixelModel::setAnimation(std::vector<cinder::gl::VboMeshRef> inFrames)
{
	mFrames = inFrames;
	mAnimElapsed = 0;
}

void PixelModel::setFPS( float inFPS ) {
	mFPS = inFPS;
}

// Self-calling function. Advances the current movement,
//   and pops it off the front of mMovements if the movement has expired.
void PixelModel::applyMovementElapsed( float elapsed ) {
	if( !mMovements.size() ) return;
	
	ModelMovement* thisMove = mMovements.front();
	thisMove->elapsed += elapsed;
	if( thisMove->elapsed >= thisMove->duration ) {
		float overflow = thisMove->elapsed - thisMove->duration;
		mMovements.pop_front();
		this->applyMovementElapsed( overflow );
	}
}

#pragma mark - per frame

void PixelModel::update( float elapsed/*, PixelModelDirector* director*/ )
{
	// self-calling function which might reduce mMovements to 0 items
	this->applyMovementElapsed( elapsed );

	// sanity checks
	if( !mFrames.size() ) return;
	if( !mMovements.size() ) return;
	
	ModelMovement* move = mMovements.front();
	float progress = move->elapsed / move->duration;
	
	mPosition = lerpVec3( move->prevLoc, move->loc, progress );
	
	if( move->alwaysFaceAltar ) {
		mRotationRads = atan2f( mPosition.y, mPosition.x ) + M_PI*0.5f;
	}
	
	/*
	//
	// Temporary behavior, borrowed from Enforcer
	//
	mSpeed = 0.2f;

	Vec3f diff = mTarget - mPosition;
	if (diff.length() <= mSpeed * 2)
	{
		mTarget.x = randFloat(-13.0f, 13.0f) * 2.0;
		mTarget.y = randFloat(-24.0f, 5.0f) * 2.0;
		mTarget.z = randFloat(-0.95f, 0.48f);
	} else {
		diff.normalize();
		diff *= mSpeed;
		mPosition += diff;
	}
	 */
	
	/*
	// Animation change?
	if( mAnimKey != move->animKey ) {
		mAnimKey = move->animKey;
		mFrames = director->getMeshesWithAnimKey(mAnimKey);
	}
	 */
	
	// Select the correct animation frame (mesh)
	int totalFrames = mAnimElapsed * mFPS;
	mCurrFrame = totalFrames % mFrames.size();
	mAnimElapsed += elapsed;
}

void PixelModel::draw()
{
	// sanity checks
	if( !mFrames.size() ) return;
	if( !mMovements.size() ) return;
		
  gl::disable(GL_TEXTURE);
  gl::disable(GL_TEXTURE_2D);
  gl::color(Color::white());
  gl::pushMatrices();
  gl::translate(mPosition);
  gl::scale(mScale);
	gl::rotate(Vec3f( 0, 0, mRotationRads*(180/M_PI) ));
  gl::draw(mFrames.at(mCurrFrame));
  gl::popMatrices();
}
