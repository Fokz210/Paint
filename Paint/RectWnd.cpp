#include "../Include/WinManager.h"



sf::RectWnd::RectWnd()
{
}

sf::RectWnd::RectWnd (sf::RectangleShape shape) :
	shape_ (shape),
	isActive (false),
	mouseHold (false)
{
}

sf::RectWnd::RectWnd (float x, float y, float width, float height, sf::Color color) :
	shape_ (),
	isActive (false),
	mouseHold (false)
{
	shape_.setPosition (Vector2f (x, y));
	shape_.setSize (Vector2f (width, height));
	shape_.setFillColor (color);
}

sf::RectWnd::~RectWnd()
{
}

void sf::RectWnd::Activate()
{
	isActive = true;
}

void sf::RectWnd::Deactivate()
{
	isActive = false;
}

bool sf::RectWnd::OnClick(Event::MouseButtonEvent event)
{
	return false;
}

bool sf::RectWnd::OnTextEntered(Event::TextEvent event)
{
	return false;
}

void sf::RectWnd::Draw(sf::RenderWindow * wnd)
{
	if (isActive)
	{
		shape_.setOutlineColor (Color (255 - shape_.getFillColor ().r, 255 - shape_.getFillColor ().g, 255 - shape_.getFillColor ().b, 255));
		shape_.setOutlineThickness (2.0f);
	}
	else
		shape_.setOutlineThickness (0.0f);

	wnd->draw(shape_);
}

bool sf::RectWnd::IsHovered(sf::Vector2f pos)
{
	return pos.x >= shape_.getPosition().x + - shape_.getOrigin().x && pos.x <= shape_.getPosition().x + shape_.getSize().x - shape_.getOrigin().x && pos.y >= shape_.getPosition().y - shape_.getOrigin().y && pos.y <= shape_.getPosition().y + shape_.getSize().y - shape_.getOrigin().x;
}

sf::StandardCursor::TYPE sf::RectWnd::GetCursorType()
{
	return StandardCursor::TYPE::HAND;
}

bool sf::RectWnd::OnKey(Event::KeyEvent event)
{
	mouseHold = true;
	return false;
}

bool sf::RectWnd::OnMouseMove(Event::MouseMoveEvent event)
{
	return false;
}

bool sf::RectWnd::OnRelease (Event::MouseButtonEvent event)
{
	mouseHold = false;
	return false;
}
