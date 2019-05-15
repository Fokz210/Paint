#include "../Include/WinManager.h"
#include <iostream>


sf::TextWnd::TextWnd(sf::Sprite sprite, sf::Font font, sf::String * string, sf::Text text) :
	SpriteWnd(sprite),
	text_(text),
	string_(string),
	wasActive(false),
	cursorPos(string_->getSize()),
	timer()
{
	text_.setString(*string_);
	text_.setPosition(Vector2f(shape_.getPosition().x + 1.0f, shape_.getPosition().y + 1.0f));
}

sf::TextWnd::TextWnd(float x, float y, float width, float height, Texture textureMap, sf::String * string, sf::Text text) :
	SpriteWnd(x, y, width, height, textureMap),
	text_(text),
	string_(string),
	wasActive(false),
	cursorPos(string_->getSize()),
	timer()
{
	text_.setString(*string_);
	text_.setPosition(Vector2f(shape_.getPosition().x + 2.0f, shape_.getPosition().y + 1.0f));
}

sf::TextWnd::~TextWnd()
{
}

bool sf::TextWnd::OnClick(Event::MouseButtonEvent event)
{
	if (event.x - text_.getCharacterSize() / 4 > text_.findCharacterPos(string_->getSize()).x) 
		cursorPos = string_->getSize();
	else
		cursorPos = 0;

	float characterSize = float(text_.getCharacterSize());

	for (unsigned int i = 0; i < string_->getSize(); i++)
	{
		float characterPos = text_.findCharacterPos(i).x;
		if (event.x - characterSize / 4 > characterPos && event.x - characterSize / 4 < characterPos + characterSize) cursorPos = i + 1;
	}

	return true;
}

void sf::TextWnd::Draw(RenderWindow * wnd)
{	
	SpriteWnd::Draw (wnd);

	RectangleShape cursorShape;
	cursorShape.setSize(Vector2f(2.0f, text_.getLocalBounds().height + text_.getLocalBounds().height / 3 ));
	cursorShape.setFillColor(text_.getFillColor());
	cursorShape.setPosition(text_.findCharacterPos(cursorPos));

	text_.setOrigin(Vector2f(0, text_.getLocalBounds().height / 2));
	text_.setPosition(Vector2f(shape_.getPosition().x + 2, shape_.getPosition().y + shape_.getLocalBounds().height / 2));

	wnd->draw(text_);
	if (isActive && int(timer.getElapsedTime().asSeconds() * 2) % 2 == 0) wnd->draw(cursorShape);
}

sf::StandardCursor::TYPE sf::TextWnd::GetCursorType()
{
	return StandardCursor::TYPE::TEXT;
}

bool sf::TextWnd::OnTextEntered(Event::TextEvent event)
{
	if (isActive && wasActive)
	{
		char buffer = char(event.unicode);

		if (buffer == 8 && string_->getSize() > 0 && cursorPos > 0)
		{
			string_->erase(cursorPos - 1);
			cursorPos--;
		}

		else if (buffer >= 32)
		{
			string_->insert(cursorPos , buffer);
			cursorPos++;
		}

		text_.setString(*string_);
	}
	else if (isActive && !wasActive)
	{
		wasActive = true;
	}
	else if (!isActive)
	{
		wasActive = false;
	}

	return isActive;
}

bool sf::TextWnd::OnKey(Event::KeyEvent event)
{
	static bool wasKeyPressed = false;

	if (event.code == Keyboard::Key::Left && cursorPos >= 0)
	{
		cursorPos--;
		wasKeyPressed = true;
	}

	if (event.code == Keyboard::Key::Right && (unsigned int(cursorPos) <= string_->getSize()))
	{
		cursorPos++;
		wasKeyPressed = true;
	}
	
	return isActive;
}

void sf::TextWnd::Updatetext ()
{
	text_.setString (*string_);
}
