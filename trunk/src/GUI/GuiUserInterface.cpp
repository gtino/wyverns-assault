#include "..\..\include\GUI\GuiUserInterface.h"

using namespace WyvernsAssault;

GuiUserInterface::GuiUserInterface(Real aspectRatio, int screenId, GuiWidgetId widgetId) :
GuiWidget()
{
	char uiName[20];
	char uiChild[40];

	this->mWidgetId = widgetId;
	this->mKills = 0;
	this->mPoints = 0;
	this->mLifePercent = 100;
	this->mSpecialPercent = 100;
	
	OverlayManager& overlayManager = OverlayManager::getSingleton();

	// Create overlay container from template
	sprintf(uiName, "Widget_%i_%i", screenId, mWidgetId);
	mUi = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/UI", "Panel", uiName));
	// Height depends on viewport aspect ratio
	mUi->setHeight(mUi->getHeight()*aspectRatio);
	
	// Initialize bars
	sprintf(uiChild, "Widget_%i_%i/GUI/UI/LifeBar", screenId, mWidgetId);
	mLifeBar = (PanelOverlayElement*)mUi->getChild(uiChild);
	mLifeBar->setHeight(mLifeBar->getHeight()*aspectRatio);
	mLifeBar->setTop(mLifeBar->getTop()*aspectRatio);
	// Save intial bar width and u2 values
	this->mLifeWidth = mLifeBar->getWidth();
	Real u1, v1, u2, v2;
	mLifeBar->getUV(u1, v1, u2, v2);
	this->mLifeU2 = u2;

	sprintf(uiChild, "Widget_%i_%i/GUI/UI/SpecialBar", screenId, mWidgetId);
	mSpecialBar = (PanelOverlayElement*)mUi->getChild(uiChild);
	mSpecialBar->setHeight(mSpecialBar->getHeight()*aspectRatio);
	mSpecialBar->setTop(mSpecialBar->getTop()*aspectRatio);
	// Save intial bar width and u2 values
	this->mSpecialWidth = mSpecialBar->getWidth();
	mSpecialBar->getUV(u1, v1, u2, v2);
	this->mSpecialU2 = u2;

	// Initialize text areas
	sprintf(uiChild, "Widget_%i_%i/GUI/UI/Kills", screenId, mWidgetId);
	mTextKills = (TextAreaOverlayElement*)mUi->getChild(uiChild);
	mTextKills->setTop(mTextKills->getTop()*aspectRatio);
	mTextKills->setCharHeight(mTextKills->getCharHeight()*aspectRatio);
	// Set kills value
	sprintf(uiChild, "%.0f", mKills);
	mTextKills->setCaption(uiChild);

	sprintf(uiChild, "Widget_%i_%i/GUI/UI/Points", screenId, mWidgetId);
	mTextPoints = (TextAreaOverlayElement*)mUi->getChild(uiChild);
	mTextPoints->setTop(mTextPoints->getTop()*aspectRatio);	
	mTextPoints->setCharHeight(mTextPoints->getCharHeight()*aspectRatio);
	// Set points value
	sprintf(uiChild, "%.0f", mPoints);
	mTextPoints->setCaption(uiChild);

	// Create an overlay, and add the panel
	mOverlay = overlayManager.create(uiName);
	mOverlay->add2D(mUi);
}

GuiUserInterface::~GuiUserInterface()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiUserInterface::show()
{
	mOverlay->show();
}

void GuiUserInterface::hide()
{
	mOverlay->hide();
}

/** Life and Special bars set */
void GuiUserInterface::setLifeBar(Real value)
{
	if (value < 101 && value > 0)
	{
		Real factor = value / 100.0;
		Real u1, v1, u2, v2;

		mLifePercent = value;
		mLifeBar->setWidth(mLifeWidth * factor);
		mLifeBar->getUV(u1, v1, u2, v2);		
		mLifeBar->setUV(u1, v1, mLifeU2*factor, v2);
	}
}

void GuiUserInterface::setSpecialBar(Real value)
{
	if (value < 101 && value > 0)
	{
		Real factor = value / 100.0;
		Real u1, v1, u2, v2;

		mSpecialPercent = value;
		mSpecialBar->setWidth(mSpecialWidth * factor);
		mSpecialBar->getUV(u1, v1, u2, v2);
		mSpecialBar->setUV(u1, v1, mSpecialU2*factor, v2);
	}
}

/** Kills and Points counter set */
void GuiUserInterface::setTextKills(Real value)
{
	char uiText[20];

	mKills = value;
	sprintf(uiText, "%.0f", mKills);
	mTextKills->setCaption(uiText);
}

void GuiUserInterface::setTextPoints(Real value)
{
	char uiText[20];

	mPoints = value;
	sprintf(uiText, "%.0f", mPoints);
	mTextPoints->setCaption(uiText);
}