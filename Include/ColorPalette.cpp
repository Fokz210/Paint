#include "../Include/WinManager.h"


sf::ColorPalette::ColorPalette (Sprite sprite, std::vector<Color> colors, Canvas * canvas) : 
	SpriteWnd (sprite),
	colors_ (colors),
	canvas_ (canvas),
	buttons_()
{
}

void sf::ColorPalette::IntegrateButtons(WindowManager * manager)
{
	bool isHorisontal = sprite_.getLocalBounds ().height < sprite_.getLocalBounds ().width;

	float gap = isHorisontal ? sprite_.getLocalBounds ().height / 2 - 10 : sprite_.getLocalBounds ().width / 2 - 10;

	for (unsigned int i = 0; i < colors_.size (); i++)
	{
		ColorButton * button = new ColorButton (Vector2f(sprite_.getPosition().x + gap , sprite_.getPosition().y + gap +gap * (i) + i * 20), Vector2f(20, 20), colors_[i], canvas_);
		manager->AddWindow (button);
		buttons_.push_back(button);
	}
}

sf::StandardCursor::TYPE sf::ColorPalette::GetCursorType ()
{
	return StandardCursor::TYPE::NORMAL;
}

bool sf::ColorPalette::OnClick (Event::MouseButtonEvent event)
{
	return false;
}

sf::ColorPalette::~ColorPalette ()
{
	for (unsigned int i = 0; i < buttons_.size (); i++)
		delete(buttons_[i]);
}
