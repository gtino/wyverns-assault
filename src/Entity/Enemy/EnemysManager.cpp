#include "..\..\..\include\Entity\Enemy\EnemysManager.h"

using namespace WyvernsAssault;

EnemysManager::EnemysManager()
{

}

EnemysManager::~EnemysManager()
{

}

void EnemysManager::finalize()
{
	this->unLoad();
}

void EnemysManager::unLoad()
{
	this->list.clear();
}

EnemyList* EnemysManager::getList()
{
	return &this->list;
}

/* TODO
*/
void EnemysManager::processEnemyIA()
{
	
}