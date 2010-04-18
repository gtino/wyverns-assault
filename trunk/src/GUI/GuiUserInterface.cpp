#include "..\..\include\GUI\GuiUserInterface.h"

using namespace WyvernsAssault;

GuiUserInterface::GuiUserInterface(Real aspectRatio, GuiWidgetId widgetId) :
GuiWidget()
{
	char uiName[20];
	char uiText[20];
	this->mWidgetId = widgetId;
	this->mKills = 0;
	this->mPoints = 0;

	OverlayManager& overlayManager = OverlayManager::getSingleton();
	
	// Create overlay container from template
	sprintf(uiName, "UI_%d", mWidgetId);
	OverlayContainer* ui = static_cast<OverlayContainer*>(overlayManager.createOverlayElementFromTemplate("GUI/UI", "Panel", uiName));	
	
	// Height depends on viewport aspect ratio
	ui->setHeight(ui->getHeight()*aspectRatio);

	// Initialize text areas
	sprintf(uiText, "UI_%d/Kills", mWidgetId);
	mTextKills = (TextAreaOverlayElement*)ui->getChild(uiText);
	mTextKills->setTop(mTextKills->getTop()*aspectRatio);
	mTextKills->setCharHeight(mTextKills->getCharHeight()*aspectRatio);
	// Set kills value
	sprintf(uiText, "%.0f", mKills);
	mTextKills->setCaption(uiText);

	sprintf(uiText, "UI_%d/Points", mWidgetId);
	mTextPoints = (TextAreaOverlayElement*)ui->getChild(uiText);
	mTextPoints->setTop(mTextPoints->getTop()*aspectRatio);	
	mTextPoints->setCharHeight(mTextPoints->getCharHeight()*aspectRatio);
	// Set points value
	sprintf(uiText, "%.0f", mPoints);
	mTextPoints->setCaption(uiText);

	// Create an overlay, and add the panel
	mOverlay = overlayManager.create(uiName);
	mOverlay->add2D(ui);
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

void GuiUserInterface::setTextKills(int value)
{
	char uiText[20];

	mKills = value;
	sprintf(uiText, "%.0f", mKills);
	mTextKills->setCaption(uiText);
}
void GuiUserInterface::setTextPoints(int value)
{
	char uiText[20];

	mPoints = value;
	sprintf(uiText, "%.0f", mPoints);
	mTextPoints->setCaption(uiText);
}