#include "..\..\include\GUI\GuiMenu.h"

using namespace WyvernsAssault;

GuiMenu::GuiMenu(Real aspectRatio, int screenId) 
: mSelectedOption(0)
, mOverlay(0)
, mMenu(0)
, mScreenId(0)
{
	char uiName[20];	

	this->mScreenId = screenId;

	OverlayManager& overlayManager = OverlayManager::getSingleton();

	// Create overlay container from template
	sprintf(uiName, "Widget_%d", mScreenId);
	// Splash screen
	if (screenId == 0)		
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/ContinueMenu", "Panel", uiName));
	// Intro screen
	else if (screenId == 1)	
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/ContinueMenu", "Panel", uiName));
	// Main Menu screen
	else if (screenId == 2)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/MainMenu", "Panel", uiName));
	// Ending screen
	else if (screenId == 4)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/GameOverMenu", "Panel", uiName));
	// Game over screen
	else if (screenId == 5)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/EndingMenu", "Panel", uiName));
	// Outro screen
	else if (screenId == 6)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/OutroMenu", "Panel", uiName));	
	// Credits screen
	else if (screenId == 7)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/CreditsMenu", "Panel", uiName));
	// Pause screen
	else if (screenId == 8)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/PauseMenu", "Panel", uiName));
	// Options screen
	else if (screenId == 9)
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/OptionsMenu", "Panel", uiName));
	// Others
	else
		mMenu = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/Menu", "Panel", uiName));	

	// Height depends on viewport aspect ratio
	mMenu->setHeight(mMenu->getHeight()*aspectRatio);

	// Initial selected option
	OverlayContainer::ChildIterator it = mMenu->getChildIterator();
	mCurrent = it.getNext();
	mSelectedOption = 0;

	// Create an overlay, and add the panel
	mOverlay = overlayManager.create(uiName);
	mOverlay->add2D(mMenu);
}

GuiMenu::~GuiMenu()
{
	mScreenId = NULL;
	mOverlay = NULL;
	mMenu = NULL;
	mSelectedOption = NULL;
}

void GuiMenu::show()
{
	mOverlay->show();
}

void GuiMenu::hide()
{
	mOverlay->hide();
}

void GuiMenu::destroyMenu()
{
	OverlayManager& overlayManager = OverlayManager::getSingleton();
	
	Overlay::Overlay2DElementsIterator i = mOverlay->get2DElementsIterator();
	OverlayContainer* container;
	OverlayElement* element;

	while(i.hasMoreElements())
	{
		container = i.getNext();
		OverlayContainer::ChildIterator it = container->getChildIterator();		
		while(it.hasMoreElements())
		{
			element = it.getNext();
			overlayManager.destroyOverlayElement(element);
		}		
		overlayManager.destroyOverlayElement(container);
	}	
	overlayManager.destroy(mOverlay);
}

/** Selection funtions */

void GuiMenu::nextOption()
{
	OverlayElement* element;
	OverlayContainer::ChildIterator it = mMenu->getChildIterator();

	while(it.hasMoreElements())
	{
		element = it.getNext();
		if (mCurrent == element)
		{
			// Deselect current
			element->setColour(ColourValue(1, 1, 1));
			// Select next
			if(it.hasMoreElements())
			{
				element = it.getNext();

				mSelectedOption++;
			}
			// Select first
			else
			{
				OverlayContainer::ChildIterator i = mMenu->getChildIterator();
				element = i.getNext();

				mSelectedOption = 0;
			}			
			element->setColour(ColourValue(1, 0.5, 0));
			// Save current
			mCurrent = element;			
			break;
		}
	}
}

void GuiMenu::previousOption()
{

	OverlayElement* element;
	OverlayElement* previous;	
	OverlayContainer::ChildIterator it = mMenu->getChildIterator();

	previous = it.getNext();

	while(it.hasMoreElements())
	{
		element = it.getNext();
		if(mCurrent == element)
		{
			// Deselect current
			element->setColour(ColourValue(1, 1, 1));
			// Select previous
			previous->setColour(ColourValue(1, 0.5, 0));
			// Save previous
			mCurrent = previous;

			mSelectedOption--;
			break;
		}
		else if(mCurrent == previous)
		{			
			OverlayContainer::ChildIterator i = mMenu->getChildIterator();
			mSelectedOption = -1;
			while(i.hasMoreElements())
			{
				previous = i.getNext();
				mSelectedOption++;
			}
			// Deselect current
			mCurrent->setColour(ColourValue(1, 1, 1));
			// Select last
			previous->setColour(ColourValue(1, 0.5, 0));
			// Save last
			mCurrent = previous;
			break;
		}				
		previous = element;
	}
}

GuiWidgetId GuiMenu::getCurrentOption()
{
	return mSelectedOption;
}
