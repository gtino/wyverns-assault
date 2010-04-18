#include "..\..\include\GUI\GuiWidget.h"

using namespace WyvernsAssault;

GuiWidget::GuiWidget() 
: mRectangle2D(0)
{
	//
	// TODO Constructor
	//
	initialize();
}

GuiWidget::~GuiWidget()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

void GuiWidget::initialize()
{
	mRectangle2D = new Ogre::Rectangle2D(true);
}

void GuiWidget::finalize()
{
	if(mRectangle2D)
	{
		delete mRectangle2D;
		mRectangle2D = 0;
	}
}

void GuiWidget::setPosition(Real positionX, Real positionY)
{
	mPosition.x = positionX;
	mPosition.y = positionY;

	update();
}

void GuiWidget::setSize(Real sizeX, Real sizeY)
{
	mSize.width = sizeX;
	mSize.heigth = sizeY;

	update();
}

void GuiWidget::update()
{
	mRectangle2D->setCorners(mPosition.x, mPosition.y, mPosition.x + mSize.width, mPosition.y - mSize.heigth, false);
}

Ogre::Rectangle2D* GuiWidget::getRectangle2D()
{
	return mRectangle2D;
}

void GuiWidget::show()
{

}

void GuiWidget::hide()
{

}
