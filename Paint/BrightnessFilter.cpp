#include "../Include/WinManager.h"
#include <iostream>

sf::BrightnessFilter::BrightnessFilter ()
{
}

void sf::BrightnessFilter::Apply (RenderTexture * texture, float value)
{
	Image img = texture->getTexture ().copyToImage ();
	
	if (value >= 0)
	{
		for (int x = 0; x < img.getSize ().x; x++)
			for (int y = 0; y < img.getSize ().y; y++)
			{
				img.setPixel (x, y, img.getPixel (x, y) + sf::Color (value, value, value, 255));
			}
	}
	else
	{
		value = -value;

		for (int x = 0; x < img.getSize ().x; x++)
			for (int y = 0; y < img.getSize ().y; y++)
			{
				img.setPixel (x, y, sf::Color (img.getPixel(x, y).r - value > 0 ? img.getPixel (x, y).r - value : 0,
					img.getPixel (x, y).g - value > 0 ? img.getPixel (x, y).g - value : 0,
					img.getPixel (x, y).b - value > 0 ? img.getPixel (x, y).b - value : 0, 255));
			}
	}

	sf::Texture tex;
	tex.loadFromImage (img);
	texture->draw (Sprite (tex));
	texture->display ();
}