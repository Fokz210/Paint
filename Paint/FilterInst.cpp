//#include "WinManager.h"
//
//sf::FilterInst::FilterInst(Shader * filter) :
//	AbstInst(),
//	filter_(filter)
//{
//
//}
//
//void sf::FilterInst::Apply(Vector2f coords, float thickness, Color color, RenderTarget * texture)
//{
//	filter_->setUniform("position", coords);
//	filter_->setUniform("thickness", thickness);
//	filter_->setUniform("texture", texture->getTexture());
//	
//	filter_->setUniform("color", static_cast<Glsl::Vec4>(color));
//
//	applyShader(filter_, texture);
//}
//
//void sf::FilterInst::Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture)
//{
//}
//
//void sf::FilterInst::ChangeShader(Shader * filter)
//{
//	filter_ = filter;
//}
//
//void sf::FilterInst::applyShader(const sf::Shader * shader, sf::RenderTarget * output)
//{
//	sf::Vector2f outputSize = static_cast<sf::Vector2f>(output->getSize());
//
//	sf::VertexArray vertices(sf::TrianglesStrip, 4);
//	vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
//	vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
//	vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
//	vertices[3] = sf::Vertex(sf::Vector2f(outputSize), sf::Vector2f(1, 0));
//
//	sf::RenderStates states;
//	states.shader = shader;
//	states.blendMode = sf::BlendNone;
//
//	output->draw(vertices, states);
//}
