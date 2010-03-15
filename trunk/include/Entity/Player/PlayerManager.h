/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "..\..\..\include\Entity\EntityManager.h"
#include <Ogre.h>

namespace WyvernsAssault
{
	/**
	Class used to manage all the enemies
	*/
	class PlayerManager : public EntityManager
	{
	public:
		PlayerManager();
		~PlayerManager();

		void initialize(Ogre::String name, Ogre::String mesh, Ogre::SceneManager* levelSceneManager, Ogre::Vector3 position);
		void finalize();

		Ogre::SceneNode* GetPlayerSceneNode() const {return playerSceneNode;}

		void setPosition(Ogre::Vector3 position);
		void setRotation(Ogre::Radian x, Ogre::Radian y, Ogre::Radian z);
		Ogre::Vector3 getPosition(){return playerSceneNode->getPosition();} 


	private:
		void initializeVariables();

		Ogre::SceneManager *mSceneMgr;
		Ogre::Entity* playerMesh;
		Ogre::SceneNode* playerSceneNode;
	};
}

#endif // __PLAYER_MANAGER_H__