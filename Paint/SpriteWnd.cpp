#include "../Include/WinManager.h"
#include <iostream>

sf::SpriteWnd::SpriteWnd() :
	RectWnd(),
	sprite_(),
	texture_()
{

}

sf::SpriteWnd::SpriteWnd(Sprite sprite) :
	RectWnd(sprite.getPosition().x, sprite.getPosition().y, sprite.getGlobalBounds().width, sprite_.getGlobalBounds().height, sf::Color::Transparent),
	sprite_(sprite),
	texture_()
{

}

sf::SpriteWnd::SpriteWnd (float x, float y, float width, float height, Texture textureMap) :
	RectWnd (x, y, width, height, sf::Color::Transparent),
	sprite_ (),
	texture_ ()
{
	TextureSampler sampler(textureMap);
	texture_.create (unsigned int (width), unsigned int (height));
	sampler.Sample (&texture_);
	sprite_.setTexture (texture_);
	sprite_.setPosition (x, y);
}

void sf::SpriteWnd::Draw(RenderWindow * wnd)
{
	if (isActive)
	{
		RectangleShape shape;
		shape.setPosition (sprite_.getPosition ());
		shape.setSize (Vector2f (sprite_.getLocalBounds ().width, sprite_.getLocalBounds ().height));
		shape.setOutlineThickness (2);
		shape.setOutlineColor (Color::Black);
		shape.setFillColor (Color::Transparent);

		wnd->draw (shape);
	}

	wnd->draw(sprite_);
}

bool sf::SpriteWnd::IsHovered(Vector2f pos)
{
	FloatRect bounds = sprite_.getGlobalBounds();
	return pos.x >= bounds.left && pos.y >= bounds.top && pos.x <= bounds.left + bounds.width && pos.y <= bounds.top + bounds.height;
}
