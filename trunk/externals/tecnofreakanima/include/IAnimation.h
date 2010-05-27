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

#ifndef __TECNOFREAK__I_ANIMATION__H__
#define __TECNOFREAK__I_ANIMATION__H__

#include <string>

#include "Common.h"

namespace tecnofreak
{

class TecnoFreakDllExport IAnimation
{
public:

		/**
		Get the animation name.

		@return Animation name.
		*/
	virtual const std::string& getName() const = 0;


		/**
		Check if the animation is enabled.

		@return True if the animation is enabled, false if it isn't.
		*/
	virtual bool getEnabled() const = 0;

		/**
		Enable or disable an animation.

		@param enabled Parameter indicating whether to enable or disable the animation.
		*/
	virtual void setEnabled( const bool enabled ) = 0;


		/**
		Get the amount by which this animation should be blended with others.

		An animation will only be blended if it's enabled.

		Values should be between 0 and 1. A value of 0 means no effect, 1 means full effect.

		@return Weight of the animation.
		*/
	virtual float getWeight() const = 0;

		/**
		Assign a weight by which the animation should be blended.

		An animation will only be blended if it's enabled.

		Values should be between 0 and 1. A value of 0 means no effect, 1 means full effect.

		Changing the weight of an animation may alter its enabled value.

		@param weight Weight to assign to the animation.
		*/
	virtual void setWeight( const float weight ) = 0;

		/**
		Get the total duration of an animation in seconds when speed is 1.

		@return Duration of the animation in seconds
		*/
	virtual float getLength() const = 0;

		/**
		Check if this animation is a looping animation.

		@return True if the animation is set to loop, false if it isn't.
		*/
	virtual bool getLoop() const = 0;

		/*
		Set the looping value of an animation.

		@param loop Looping value of the animation.
		*/
	virtual void setLoop( const bool loop ) = 0;

		/**
		Get the speed at which this animation is running relative to the 'real' animation
		speed.

		Examples:
		A value of 0 means completely stopped.
		A value of 0.5 means half speed.
		A value of 1 means normal speed.
		A value of 2 means double speed.
		A negative value will make it go backwards
		etc

		@return Speed of the animation.
		*/
	virtual float getSpeed() const = 0;

		/**
		Set the speed of the animation relative to the 'real' animation speed.

		Examples:
		A value of 0 means completely stopped.
		A value of 0.5 means half speed.
		A value of 1 means normal speed.
		A value of 2 means double speed.
		A negative value will make it go backwards
		etc

		@param speed Speed of the animation.
		*/
	virtual void setSpeed( const float speed ) = 0;

		/**
		Resets the time position of the animation.
		*/
	virtual void reset() = 0;

		/**
		Get the current time position of the animation in seconds.

		@return The current time position of the animation in seconds.
		*/
	virtual float getTimePosition() const = 0;

		/**
		Get the current relative time position of the animation.
		It is a value between 0 and 1.
		0 -> start of the animation.
		1 -> end of the animation.

		@return The current relative time position of the animation.
		*/
	virtual float getRelativeTimePosition() const = 0;

		/**
		Update the animation.

		@param elapsedSeconds Seconds that have passed since the last update.
		*/
	virtual void update( const float elapsedSeconds ) = 0;

	virtual ~IAnimation() {}
};

}

#endif