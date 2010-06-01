#include "..\..\include\Audio\FileLocator.h"

using namespace WyvernsAssault;

Archive* FileLocator::find(String &filename)
{
	ResourceGroup* grp = getResourceGroup("General");
	if (!grp)
		OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Cannot locate a resource group called 'General'", "ResourceGroupManager::openResource");

	OGRE_LOCK_MUTEX(grp->OGRE_AUTO_MUTEX_NAME) // lock group mutex
		ResourceLocationIndex::iterator rit = grp->resourceIndexCaseSensitive.find(filename);
	if (rit != grp->resourceIndexCaseSensitive.end())
	{
		// Found in the index
		Archive *fileArchive = rit->second;
		filename = fileArchive->getName() + "/" + filename;
		return fileArchive;
	}
	return NULL;
}