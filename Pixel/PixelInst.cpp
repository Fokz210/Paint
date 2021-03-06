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
	PaintInst () :
		AbstInst ({
			"Pixel",
			nullptr,
			nullptr
			}),
		gridStart ()
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile ("ext//bin//Pixel.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture) override
	{	
		RectangleShape rect;
		thickness *= 2;

		rect.setSize (Vector2f (thickness  , thickness ));
		rect.setFillColor (color);
		rect.setPosition (Vector2f (float (int (coords.x) - int (coords.x) % int (thickness)) , float (int (coords.y) - int (coords.y) % int (thickness)) ));
		texture->draw (rect);
	}

	virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override
	{
		
		
	}

	Vector2f gridStart;
};

BOOL APIENTRY DllMain (HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::cout << "PixelInst.dll loaded" << std::endl;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern"C" __declspec(dllexport) AbstInst * CreateInstObject ()
{
	AbstInst * object = new PaintInst ();
	return object;
}

extern"C" __declspec(dllexport) sf::Sprite GetSprite ()
{
	static sf::Texture texture;
	texture.loadFromFile ("D://pencil.png");
	static sf::Sprite sprite (texture);
	return sprite;
}


