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
			"BrightnessGraph",
			nullptr,
			nullptr
			}),
		wnd (nullptr),
		copyOfImg (nullptr)
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile ("ext//bin//Brightness.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture) override
	{
		if (!wnd) 
			return;
		
		if (copyOfImg == nullptr)
		{
			copyOfImg = new Image;
			*copyOfImg = texture->getTexture ().copyToImage ();
		}

		const Uint8 * texturePtr = copyOfImg->getPixelsPtr ();

		const unsigned size = copyOfImg->getSize ().x * copyOfImg->getSize ().y * 4;
		Uint8 * pixels = new Uint8 [size];

		float * palette = wnd->getGraph ();

		for (unsigned i = 0; i < size; i++)
			pixels[i] = Uint8(palette[texturePtr[i]]);

		Image img;
		img.create (copyOfImg->getSize ().x, copyOfImg->getSize ().y, pixels);
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
		wnd = new Graph (1565, 100, manager, 256, 256, Color (74, 74, 74));
		manager->AddWindow (wnd);
		wnd->insertPoint (1565, 356, 10, Color::Red);
		wnd->insertPoint (1565 + 128, 228, 10, Color::Red);
		wnd->insertPoint (1565 + 256, 100, 10, Color::Red);
	}

	virtual void UnloadWindows (sf::WindowManager * manager)
	{
		if (wnd != nullptr)
		{
			wnd->removePoints (manager);
			manager->DeleteWindow ();
			delete (wnd);
		}

		if (copyOfImg != nullptr)
		{
			delete (copyOfImg);
			copyOfImg = 0;
		}
	}

private:
	Graph * wnd;
	Image * copyOfImg;
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


