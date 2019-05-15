#include "../Include/WinManager.h"

sf::Spray::Spray(Shader * sprayShader) :
	sprayShader_(sprayShader),
	AbstInst({
			"Spray",
			nullptr,
			nullptr
		})
{
}

void sf::Spray::Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture)
{
	sprayShader_->setUniform("coords", Glsl::Vec2(coords.x, coords.y));
	sprayShader_->setUniform("color", Glsl::Vec4(color));
	sprayShader_->setUniform("thickness", thickness);
	sprayShader_->setUniform("random", 1000000);
	sprayShader_->setUniform("textureSize", Glsl::Vec2(texture->getSize()));
	
	Texture buff = texture->getTexture();
	sprayShader_->setUniform("texture", buff);

	texture->draw(Sprite(buff), sprayShader_);
	texture->display();
}

void sf::Spray::Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture)
{
}
