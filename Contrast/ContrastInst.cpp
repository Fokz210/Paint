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
			"Contrast",
			nullptr,
			nullptr
			}),
		wnd (nullptr)
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile ("ext//bin//Contrast.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture) override
	{
		Image img = texture->getTexture ().copyToImage ();

		int lAB = 0;
		int palette[256] = {};

		for (int x = 0; x < img.getSize ().x; x++)
			for (int y = 0; y < img.getSize ().y; y++)
				lAB += (int)(img.getPixel (x, y).r * 0.299f + img.getPixel (x, y).g * 0.587f + img.getPixel (x, y).b * 0.114);
		lAB /= img.getSize ().x * img.getSize ().y;

		float k = 1.0f + (wnd->getValue() - 128) / 100.0f;

		for (int i = 0; i < 256; i++)
		{
			int delta = (int) i - lAB;
			int temp = (int) (lAB + k * delta);

			if (temp < 0)
				temp = 0;

			if (temp >= 255)
				temp = 255;
			palette[i] = (unsigned char) temp;
		}

		for (int x = 0; x < img.getSize ().x; x++)
			for (int y = 0; y < img.getSize ().y; y++)
				img.setPixel (x, y, sf::Color (palette[img.getPixel (x, y).r], palette[img.getPixel (x, y).g], palette[img.getPixel (x, y).b], 255));

		sf::Texture tex;
		tex.loadFromImage (img);
		texture->draw (Sprite (tex));
		texture->display ();
	}

	virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override
	{

	}

	virtual void LoadWindows (sf::WindowManager * manager)
	{
		wnd = new SliderWnd<float> (145 + 10 * 4 + 50 * 4, 25, 100, 20, sf::Color::White, sf::Color::Black, 0, 255, 128);
		manager->AddWindow (wnd);
	}

	virtual void UnloadWindows (sf::WindowManager * manager)
	{
		if (wnd != nullptr)
		{
			manager->DeleteWindow ();
			delete (wnd);
		}
	}

private:
	SliderWnd<float> * wnd;

};

BOOL APIENTRY DllMain (HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		std::cout << "EraserInst.dll loaded" << std::endl;
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


