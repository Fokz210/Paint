// PaintInst.cpp : Defines the exported functions for the DLL application.
//

#define NOMINMAX
#define Assert(x) if ((x) == 0) DebugBreak()


#include "D:/TX/TXLib.h"
#include <Windows.h>
#include "../Include/WinManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int p3DTo1D (int z, int y, int x, int dimz, int dimy, int dimx)
{
	Assert (0 <= x); Assert (x < dimx);
	Assert (0 <= y); Assert (y < dimy);
	Assert (0 <= z); Assert (z < dimz);

	return ((dimx) * (dimy) * (z) + (dimx) * (y) + (x));
}

int p2DTo1D (int y, int x, int dimy, int dimx)
{
	Assert (0 <= x); Assert (x < dimx);
	Assert (0 <= y); Assert (y < dimy);

	return  ((dimx) * (y) + (x));
}

using namespace sf;

class PaintInst :
	public AbstInst
{
public:
	PaintInst () :
		AbstInst ({
			"Brightness ",
			nullptr,
			nullptr
			}),
		wnd (nullptr),
		imgCopy (nullptr)
	{
		sf::Texture * texture = new sf::Texture;
		texture->loadFromFile ("ext//bin//Brightness.png");
		instData_.icon_released_ = texture;
	}

	virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture) override
	{
		if (!wnd)
			return;

		if (!imgCopy)
		{
			imgCopy = new Image;
			*imgCopy = texture->getTexture ().copyToImage ();
		}

		Image matrixImg;
		Texture matrixTex;

		const int matrixSize = wnd->getValue ();
		double matrix[1000] = {};

		for (int y = -matrixSize / 2; y < matrixSize / 2; y++)
			for (int x = -matrixSize / 2; x < matrixSize / 2; x++)
				matrix[p2DTo1D (y + matrixSize / 2, x + matrixSize / 2, matrixSize, matrixSize)] = exp (-(x * x + y * y) / (matrixSize / 2));

		int summ = 0;

		for (int i = 0; i < matrixSize * matrixSize; i++)
		{
			summ += matrix[i];
		}

		for (int i = 0; i < matrixSize * matrixSize; i++)
		{
			matrix[i] /= summ;
		}

		Uint8 * matrixArr = new Uint8[matrixSize * matrixSize * 4];

		for (int c = 0; c < 4; c++)
			for (int y = 0; y < matrixSize; y++)
				for (int x = 0; x < matrixSize; x++)
				{
					if (c == 3)
						matrixArr[p3DTo1D (y, x, c, matrixSize, matrixSize, 4)] = 255;
					else
						matrixArr[p3DTo1D (y, x, c, matrixSize, matrixSize, 4)] = matrix[p2DTo1D (y, x, matrixSize, matrixSize)] * 255;
				}

		matrixImg.create (matrixSize, matrixSize, matrixArr);
		matrixTex.loadFromImage (matrixImg);

		Texture source = texture->getTexture ();

		sf::Shader shader;
		shader.loadFromFile ("bin//blur.frag", sf::Shader::Fragment);
		shader.setUniform ("texSize", Vector2f(texture->getSize().x, texture->getSize().y));
		shader.setUniform ("matrixSize", Vector2f (matrixSize, matrixSize));
		shader.setUniform ("tex", source);
		shader.setUniform ("matrix", matrixTex);

		texture->draw (Sprite (source), &shader);
		texture->display ();

		delete[] matrixArr;
	}

	virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override
	{

	}

	virtual void LoadWindows (sf::WindowManager * manager)
	{
		wnd = new SliderWnd<float> (145 + 10 * 4 + 50 * 4, 25, 100, 20, sf::Color::White, sf::Color::Black, 0, 21, 21);
		manager->AddWindow (wnd);
	}

	virtual void UnloadWindows (sf::WindowManager * manager)
	{
		if (wnd != nullptr)
		{
			manager->DeleteWindow ();
			delete (wnd);
		}
		if (imgCopy)
			delete (imgCopy);
	}

private:
	SliderWnd<float> * wnd;
	Image * imgCopy;
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


