/*
TecnoFreak Animation System
http://sourceforge.net/projects/tecnofreakanima/

Copyright (c) 2008 Pau Novau Lebrato

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef __TECNOFREAK__OGRE_ANIMATION_SYSTEM__H__
#define __TECNOFREAK__OGRE_ANIMATION_SYSTEM__H__

#include "../Common.h"
#include "Nodes/OutputNode.h"
#include "Nodes/AnimationNode.h"
#include "ExampleAnimationSystem.h"

namespace Ogre
{
	class Entity;
	class AnimationStateSet;
}

#include <vector>

namespace tecnofreak
{

namespace ogre
{

class TecnoFreakDllExport AnimationSystem
	: public ExampleAnimationSystem
{
public:
	AnimationSystem( Ogre::Entity* entity );
	AnimationSystem( Ogre::AnimationStateSet* animationStateSet );
	virtual ~AnimationSystem();

	virtual void loadAnimationTree( const std::string& animationTreeOgreResourceName, const std::string& ogreResourceResourceGroupName );
	virtual void loadAnimationInfo( const std::string& animationInfoOgreResourceName, const std::string& ogreResourceResourceGroupName );

	using ExampleAnimationSystem::loadAnimationTree;
	using ExampleAnimationSystem::loadAnimationInfo;

protected:

	virtual void createAnimations();

	Ogre::AnimationStateSet* getAnimationStateSet();
	const Ogre::AnimationStateSet* getAnimationStateSet() const;

private:
	Ogre::AnimationStateSet* m_animations;

};

}

}

#endif
