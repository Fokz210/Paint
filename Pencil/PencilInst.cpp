// PencilInst.cpp: Defines the exported functions for the DLL application.
//
#define NOMINMAX

#include <Windows.h>
#include "../Include/WinManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class PencilInst :
	public AbstInst
{
public:
	PencilInst() :
		AbstInst({
			"Pencil",
			nullptr,
			nullptr
			})
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile("ext//bin//Pencil.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture)			    override
	{
		RectangleShape rect;
		rect.setSize(Vector2f(thickness * 2, thickness * 2));
		rect.setOrigin(thickness, thickness);
		rect.setPosition(coords);
		rect.setFillColor(color);
		rect.setOutlineThickness(0.0);

		texture->draw(rect);
	}

	virtual void Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override
	{
		RectangleShape rect;
		rect.setSize(Vector2f(thickness * 2, thickness * 2));
		rect.setOrigin(thickness, thickness);
		rect.setPosition(coords);
		rect.setOutlineColor(color);
		rect.setFillColor(Color::Transparent);
		rect.setOutlineThickness(1.0f);

		texture->draw(rect);
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
		std::cout << "PencilInst.dll loaded" << std::endl;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	} 
	return TRUE;
}

extern"C" __declspec(dllexport) AbstInst * CreateInstObject()
{
	AbstInst * object = new PencilInst;
	return object;
}



