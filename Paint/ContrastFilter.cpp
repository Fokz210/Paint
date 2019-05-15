#include "../Include/WinManager.h"

sf::ContrastFilter::ContrastFilter ()
{

}

void sf::ContrastFilter::Apply (RenderTexture * texture, float value)
{
	Image img = texture->getTexture ().copyToImage ();

	int lAB = 0;
	int palette[256] = {};

	for (int x = 0; x < img.getSize ().x; x++)
		for (int y = 0; y < img.getSize ().y; y++)
			lAB += (int)(img.getPixel (x, y).r * 0.299f + img.getPixel (x, y).g * 0.587f + img.getPixel (x, y).b * 0.114);
	lAB /= img.getSize ().x * img.getSize ().y;

	float k = 1.0f + value / 100.0f;

	for (int i = 0; i < 256; i++)
	{
		int delta = (int)i - lAB;
		int temp = (int)(lAB + k * delta);

		if (temp < 0)
			temp = 0;

		if (temp >= 255)
			temp = 255;
		palette[i] = (unsigned char)temp;
	}

	for (int x = 0; x < img.getSize ().x; x++)
		for (int y = 0; y < img.getSize ().y; y++)
			img.setPixel (x, y, sf::Color (palette[img.getPixel (x, y).r], palette[img.getPixel (x, y).g], palette[img.getPixel (x, y).b], 255));

	sf::Texture tex;
	tex.loadFromImage (img);
	texture->draw (Sprite (tex));
	texture->display ();
}