#include "../Include/WinManager.h"

sf::Brush::Brush(sf::Texture brush) :
	AbstInst({
			"Brush",
			nullptr,
			nullptr
			}),
	brush_(brush)
{
}

void sf::Brush::Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture)
{
	sf::Sprite sprite(brush_);
}

void sf::Brush::Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture)
{
}
