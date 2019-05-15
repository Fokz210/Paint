#include "../Include/WinManager.h"
#include <iostream>

sf::SlidingWnd::SlidingWnd (float x, float y, float size, Color fill) :
	RectWnd (x, y, size, size, fill),
	holdPoint ()
{
	shape_.setOrigin (size / 2, size / 2);
}

bool sf::SlidingWnd::OnClick (Event::MouseButtonEvent event)
{
	mouseHold = true;

	return true;
}

bool sf::SlidingWnd::OnRelease (Event::MouseButtonEvent event)
{
	if (mouseHold)
	{
		mouseHold = false;
		return true;
	}
	else
		return false;
}

bool sf::SlidingWnd::OnMouseMove (Event::MouseMoveEvent event)
{
	if (mouseHold)
		shape_.setPosition (event.x, event.y);
	return true;
}

sf::Vector2<float> sf::SlidingWnd::getPos ()
{
	return shape_.getPosition();
}
