#include <iostream>
#include "../Include/WinManager.h"
#include "../Include/StandardCursor.h"

sf::PaletteWnd::PaletteWnd(Sprite sprite, Palette * palette) :
	SpriteWnd(sprite),
	palette_(palette),
	buttons_(),
	textures_()
{

}

void sf::PaletteWnd::IntegrateButtons(WindowManager * manager, Texture textureMap)
{
	using std::cout;
	using std::endl;

	std::vector<AbstInst*> palette = palette_->GetPalette();

	Texture * texture = new Texture;
	textures_.push_back(texture);
	int iconSize = palette[0]->GetInstData().icon_released_->getSize().x;
	texture->create(iconSize + iconSize / 2, iconSize * 0.5 + iconSize * float(palette.size()) + iconSize * (float(palette.size()) - 1) * 0.25);

	TextureSampler sampler(textureMap);
	sampler.Sample(texture);

	sprite_.setTexture(*texture, true);

	for (unsigned int i = 0; i < palette.size(); i++)
	{
		sf::Sprite sprite;
		sprite.setTexture(*palette[i]->GetInstData().icon_released_);
		sprite.setPosition(Vector2f(sprite_.getPosition().x + iconSize / 4, sprite_.getPosition().y + iconSize / 4 +  iconSize * i + iconSize * i * 0.25));

		InstrumentButton * button = new InstrumentButton(sprite, palette_, i);
		manager->AddWindow(button);
		buttons_.push_back(button);
	}
}

sf::StandardCursor::TYPE sf::PaletteWnd::GetCursorType()
{
	return StandardCursor::TYPE::NORMAL;
}

bool sf::PaletteWnd::OnClick(Event::MouseButtonEvent event)
{
	return false;
}

sf::PaletteWnd::~PaletteWnd()
{
	for (unsigned int i = 0; i < buttons_.size(); i++)
		delete(buttons_[i]);

	for (unsigned int i = 0; i < textures_.size(); i++)
		delete(textures_[i]);
}
