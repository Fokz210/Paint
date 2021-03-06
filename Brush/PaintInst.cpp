// PaintInst.cpp : Defines the exported functions for the DLL application.
//

#define NOMINMAX

#include <Windows.h>
#include "../Include/WinManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class PaintInst :
	public AbstInst
{
public:
	PaintInst() :
		AbstInst({
			"Brush",
			nullptr,
			nullptr
			})
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile("ext//bin//Brush.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture) override
	{
		CircleShape circle;
		circle.setRadius(thickness );
		circle.setOrigin(Vector2f(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2));
		circle.setPosition(coords);
		circle.setFillColor(color);
		circle.setOutlineThickness(0.0);
		texture->draw(circle);
	}

	virtual void Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override
	{
		CircleShape circle;
		circle.setRadius(thickness);
		circle.setOrigin(Vector2f(circle.getLocalBounds().width / 2, circle.getLocalBounds().height / 2));
		circle.setPosition(coords);
		circle.setFillColor(Color::Color(0, 0, 0, 0));
		circle.setOutlineColor(color);
		circle.setOutlineThickness(1.0f);
		texture->draw(circle);

	}
};

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::cout << "PaintInst.dll loaded" << std::endl;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern"C" __declspec(dllexport) AbstInst * CreateInstObject()
{
	AbstInst * object = new PaintInst();
	return object;
}

extern"C" __declspec(dllexport) sf::Sprite GetSprite()
{
	static sf::Texture texture;
	texture.loadFromFile("D://pencil.png");
	static sf::Sprite sprite(texture);
	return sprite;
}


