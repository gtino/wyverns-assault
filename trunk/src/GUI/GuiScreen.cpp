#include "..\..\include\GUI\GuiScreen.h"

using namespace WyvernsAssault;

GuiScreen::GuiScreen(Ogre::SceneManager* sceneManager, GuiScreenId id, const Ogre::String& name)
: mGuiScreenName("")
{
	//
	// TODO Constructor
	//
	mSceneManager = sceneManager;
	mGuiScreenId = id;
	mGuiScreenName = name;
	mBackgroundNode = NULL;
}

GuiScreen::~GuiScreen()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiScreen::addWidget(GuiWidget* widget, GuiWidgetId widgetId)
{
	mWidgetMap[widgetId] = widget;

	//// Attach background to the scene
	char widgetNode[40];
	sprintf(widgetNode, "Widget_%i_%i", mGuiScreenId, widgetId);

	SceneNode* n = mSceneManager->getRootSceneNode()->createChildSceneNode(widgetNode);
	n->attachObject(widget->getRectangle2D());

	widget->show();
}

GuiWidget* GuiScreen::getWidget(GuiWidgetId widgetId)
{
	return mWidgetMap[widgetId];
}

void GuiScreen::removeWidget(GuiWidgetId widgetId)
{

	char widgetNode[40];
	sprintf(widgetNode, "Widget_%i_%i", mGuiScreenId, widgetId);

	mSceneManager->destroySceneNode(widgetNode);

}

void GuiScreen::removeAllWidgets()
{
	WidgetMapIterator it;
	for ( it=mWidgetMap.begin() ; it != mWidgetMap.end(); it++ )
	{
		this->removeWidget((*it).first);
	}
	mWidgetMap.clear();
}

void GuiScreen::removeGui()
{
	this->removeAllWidgets();
	if(mBackgroundNode)
		mSceneManager->destroySceneNode(mBackgroundNode);
}

GuiWidget* GuiScreen::nextWidget(GuiWidgetId widgetId)
{	
	// TODO
	if (!mWidgetMap[widgetId++])
		return (mWidgetMap[widgetId]);
	else 
		return (mWidgetMap[widgetId++]);
}

GuiWidget* GuiScreen::previousWidget(GuiWidgetId widgetId)
{
	// TODO
	if (widgetId>0)
		widgetId--;
	return (mWidgetMap[widgetId]);
}

void GuiScreen::setBackground(GuiBackground* background)
{
	//// Attach background to the scene
	mBackgroundNode = mSceneManager->getRootSceneNode()->createChildSceneNode(mGuiScreenName+"_Background");
	mBackgroundNode->attachObject(background->getRectangle2D());
}

void GuiScreen::show()
{
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	WidgetMapIterator it;
	char name[40];
	for ( it=mWidgetMap.begin() ; it != mWidgetMap.end(); it++ )
	{
		sprintf(name, "Widget_%i_%i", mGuiScreenId, (*it).first);
		SceneNode* n = mSceneManager->getSceneNode(name);
		n->setVisible(true);

		sprintf(name, "UI_%d", (*it).first);
		if(overlayManager.getByName(name))
			overlayManager.getByName(name)->show();
	}
	if(mBackgroundNode)
		mBackgroundNode->setVisible(true);	
}

void GuiScreen::hide()
{
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	WidgetMapIterator it;
	char name[40];
	for ( it=mWidgetMap.begin() ; it != mWidgetMap.end(); it++ )
	{
		sprintf(name, "Widget_%i_%i", mGuiScreenId, (*it).first);
		SceneNode* n = mSceneManager->getSceneNode(name);
		n->setVisible(false);

		sprintf(name, "UI_%d", (*it).first);
		if(overlayManager.getByName(name))
			overlayManager.getByName(name)->hide();
	}
	if(mBackgroundNode)
		mBackgroundNode->setVisible(false);
}

// -----------------------------------------
//
// Keyboard listeners
//
/** Buffered input - keyboard key clicked */
bool GuiScreen::keyClicked(const OIS::KeyEvent& e)
{	 
	return true;
}

/** Buffered input - keyboard key clicked */
bool GuiScreen::keyPressed(const OIS::KeyEvent& e)
{
	return true;
}

/** Buffered input - keyboard key clicked */
bool GuiScreen::keyReleased(const OIS::KeyEvent& e)
{
	return true;
}

//
// MouseListener
//
/** Buffered input - mouse moved */
bool GuiScreen::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}

/** Buffered input - mouse button pressed */
bool GuiScreen::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID)
{
	return true;
}

/** Buffered input - mouse button released */
bool GuiScreen::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID)
{
	return true;
}

//
// JoyStickListener
//
/** Buffered input - joystick button pressed */
bool GuiScreen::buttonPressed(const OIS::JoyStickEvent &evt, int index)
{
	return true;
}

/** Buffered input - joystick button released */
bool GuiScreen::buttonReleased(const OIS::JoyStickEvent &evt, int index)
{
	return true;
}

/** Buffered input - axis pad moved */
bool GuiScreen::axisMoved(const OIS::JoyStickEvent &evt, int index)
{
	return true;
}

/** Buffered input - pov moved */
bool GuiScreen::povMoved(const OIS::JoyStickEvent &evt, int index)
{
	return true;
}