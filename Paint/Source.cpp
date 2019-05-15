#define NOMINMAX
#define BUFF_STRING_SIZE 128

#include "D:/TX/TXLib.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include "../Include/WinManager.h"
#include "resource.h"
#include "../Include/TagTree.h"

int main()
{
	txDisableAutoPause ();

	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Paint");
	
	window.clear (sf::Color::White);

	sf::WindowManager manager;
 
	sf::Palette palette (&manager);
	sf::DllLoader loader(&palette);
	loader.loadFromFolder("..//Debug");
	loader.loadFromFolder ("ext");

	sf::Canvas canvas (sf::Vector2f (1500, 800), sf::Vector2f (145, 100), sf::Color::Black, 10, &palette, &window);
	canvas.Clear (sf::Color::White);
	manager.AddWindow (&canvas);

	FILE * file = fopen("bin//palette.xml", "r");
	canvas.ReadXML (file);
	fclose (file);

	sf::Texture paletteTexture;
	paletteTexture.create(10, 10);
	sf::Sprite paletteSprite(paletteTexture);
	paletteSprite.setPosition(35, 100);
	sf::PaletteWnd paletteWnd(paletteSprite, &palette);

	sf::Texture textureMap;
	textureMap.loadFromFile("bin//texturePack2.png");
	
	manager.AddWindow(&paletteWnd);
	paletteWnd.IntegrateButtons(&manager, textureMap);

	std::vector<sf::Color> colors;
	colors.push_back (sf::Color::White);
	colors.push_back (sf::Color::Black);
	colors.push_back (sf::Color::Red);
	colors.push_back (sf::Color::Yellow);
	colors.push_back (sf::Color::Green);
	colors.push_back (sf::Color::Cyan);
	colors.push_back (sf::Color::Blue);
	colors.push_back (sf::Color::Magenta);

	sf::Texture colorPaletteTexture;
	colorPaletteTexture.create (20 * 1 + 15 * 2, 20 * 8 + 15 * 9);
	
	sf::TextureSampler sampler (textureMap);
	sampler.Sample (&colorPaletteTexture);

	sf::Sprite colorPaletteSprite (colorPaletteTexture);
	colorPaletteSprite.setPosition (window.getSize().x - 75  , 100);

	sf::ColorPalette colorPalette (colorPaletteSprite, colors, &canvas);
	manager.AddWindow (&colorPalette);

	colorPalette.IntegrateButtons (&manager);

	sf::Font font;
	font.loadFromFile ("bin//font.ttf");

	sf::String colorStringR, colorStringG, colorStringB;

	colorStringR = "0";
	colorStringG = "0";
	colorStringB = "0";

	char r[8] = "", g[8] = "", b[8] = "";
	_itoa_s (canvas.GetColor ().r, r, 10);
	_itoa_s (canvas.GetColor ().g, g, 10);
	_itoa_s (canvas.GetColor ().b, b, 10);

	colorStringR = r;
	colorStringG = g;
	colorStringB = b;

	sf::Text colorTextR, colorTextG, colorTextB;

	colorTextR.setFillColor (sf::Color::Red);
	colorTextG.setFillColor (sf::Color::Green);
	colorTextB.setFillColor (sf::Color::Blue);

	colorTextR.setFont (font);
	colorTextG.setFont (font);
	colorTextB.setFont (font);

	colorTextR.setCharacterSize (15);
	colorTextG.setCharacterSize (15);
	colorTextB.setCharacterSize (15);
	
	sf::TextWnd colorWndR (145, 25, 50, 20, textureMap, &colorStringR, colorTextR);
	sf::TextWnd colorWndG (145 + 50 + 10, 25, 50, 20, textureMap, &colorStringG, colorTextG);
	sf::TextWnd colorWndB (145 + 50 * 2 + 10 * 2, 25, 50, 20, textureMap, &colorStringB, colorTextB);

	manager.AddWindow (&colorWndR);
	manager.AddWindow (&colorWndG);
	manager.AddWindow (&colorWndB);

	auto lambda1 = [&]() 
	{
		std::string strR = colorStringR;
		std::string strG = colorStringG;
		std::string strB = colorStringB;

		canvas.SetColor (sf::Color::Color ( sf::Uint8(atoi (strR.c_str ())) , sf::Uint8(atoi (strG.c_str ())) , sf::Uint8(atoi (strB.c_str ())) , 255));
	};

	sf::Text btnText;
	btnText.setCharacterSize (15);
	btnText.setFont (font);
	btnText.setFillColor (sf::Color::Black);
	btnText.setString ("Color");

	
	sf::SpriteButton<decltype(lambda1)> colorSetButton (145 + 50 * 3 + 10 * 3, 25, 50, 20, textureMap, btnText, lambda1);
	manager.AddWindow (&colorSetButton);

	auto lambda2 = [&]() { canvas.SetThickness (canvas.GetThickness () + 5); };
	auto lambda3 = [&]() { if (canvas.GetThickness() > 4) canvas.SetThickness (canvas.GetThickness () - 5); };

	btnText.setCharacterSize (15);

	btnText.setString ("+");
	sf::SpriteButton<decltype(lambda2)> thicknessIncreaseBtn (145, 55, 20, 20, textureMap, btnText, lambda2);
	if (window.getSize().x >= 768) manager.AddWindow (&thicknessIncreaseBtn);

	btnText.setString ("-");
	sf::SpriteButton<decltype(lambda3)> thicknessDecreaseBtn (145 + 10 * 2 + 20 + 50, 55, 20, 20, textureMap, btnText, lambda3);
	
	sf::String thicknessString = "10";
	char thickness[8] = "";
	_itoa_s (canvas.GetThickness (), thickness, 10);
	thicknessString = thickness;

	sf::Text thicknessText;
	thicknessText.setFont (font);
	thicknessText.setCharacterSize (15);
	thicknessText.setFillColor (sf::Color::Black);

	sf::TextWnd thicknessWnd (145 + 10 + 20, 55, 50, 20, textureMap, &thicknessString, thicknessText);

	if (window.getSize ().x >= 768)manager.AddWindow (&thicknessWnd);
	if (window.getSize ().x >= 768)manager.AddWindow (&thicknessDecreaseBtn);

	auto saveImgLambda = [&]()
	{
		
	};

	auto loadImgLambda = [&]()
	{
		sf::Image img;
		img.loadFromFile (txInputBox ("Enter path to PNG image", "Import", "bin//example.png"));
		sf::Texture texture;
		texture.loadFromImage (img);
		canvas.SetTexture (texture);
	};

	btnText.setCharacterSize (13);

	btnText.setString ("Save");
	sf::SpriteButton<decltype(saveImgLambda)> saveImgBtn (145 + 10 * 3 + 20 * 2 + 50, 55, 50, 20, textureMap, btnText, saveImgLambda);
	manager.AddWindow (&saveImgBtn);

	btnText.setString ("Load");
	sf::SpriteButton<decltype(loadImgLambda)> loadImgBtn (145 + 10 * 4 + 20 * 2 + 50 * 2, 55, 50, 20, textureMap, btnText, loadImgLambda);
	manager.AddWindow (&loadImgBtn);

	auto SaveSettingsLambda = [&]()
	{
		file = fopen ("bin//palette.xml", "w");
		canvas.WriteXML (file);
		fclose (file);
	};

	btnText.setString ("Save settings");
	sf::SpriteButton<decltype(SaveSettingsLambda)> saveSettingsBtn (145 + 10 * 5 + 20 * 2 + 50 * 3, 55, 100, 20, textureMap, btnText, SaveSettingsLambda);
	manager.AddWindow (&saveSettingsBtn);
	
	float prevThickness = canvas.GetThickness ();
	sf::String prevThicknessStr = thicknessString;

	sf::Color prevColor = canvas.GetColor ();
	sf::String prevColorStrR = colorStringR;
	sf::String prevColorStrG = colorStringG;
	sf::String prevColorStrB = colorStringB;

	sf::BrightnessFilter brightnessFilter;
	sf::ContrastFilter contrastFilter;

	bool wasPressed = false;

	sf::Texture blur;
	blur.loadFromFile ("blur 30.png");

	sf::Shader blurS;
	blurS.loadFromFile ("bin//blur.frag", sf::Shader::Fragment);
	blurS.setUniform ("matrix", blur);
	blurS.setUniform ("matrixSize", sf::Vector2f(blur.getSize()));

	sf::Texture curr;
	curr.create (window.getSize ().x, window.getSize ().y);

	while (window.isOpen())
	{
		window.clear(sf::Color::Color(200, 200, 200 , 255));
		colorSetButton.setTextColor (sf::Color (255 - canvas.GetColor ().r, 255 - canvas.GetColor ().g, 255 - canvas.GetColor ().b, 255));

		if (prevColor != canvas.GetColor ())
		{
			char r[8] = "", g[8] = "", b[8] = "";
			_itoa_s (canvas.GetColor ().r, r, 10);
			_itoa_s (canvas.GetColor ().g, g, 10);
			_itoa_s (canvas.GetColor ().b, b, 10);
			colorStringR = r;
			colorStringG = g;
			colorStringB = b;
			colorWndR.Updatetext ();
			colorWndG.Updatetext ();
			colorWndB.Updatetext ();
			prevColor = canvas.GetColor ();
		}
		else if (colorStringB != prevColorStrB || colorStringG != prevColorStrG || colorStringR != prevColorStrR)
		{
			lambda1 ();
			
			prevColorStrR = colorStringR;
			prevColorStrG = colorStringG;
			prevColorStrB = colorStringB;

			prevColor = canvas.GetColor ();
		}

		if (prevThickness != canvas.GetThickness ())
		{
			char thickness[8] = "";
			_itoa_s (canvas.GetThickness (), thickness, 10);
			thicknessString = thickness;
			thicknessWnd.Updatetext ();
			prevThickness = canvas.GetThickness ();
		}
		else if (thicknessString != prevThicknessStr)
		{
			std::string str = thicknessString;
			canvas.SetThickness ((float)atoi (str.c_str ()));
			prevThicknessStr = thicknessString;
			thicknessWnd.Updatetext ();
			prevThickness = canvas.GetThickness ();
		}

		manager.Run(&window);

		window.display();
	}

	return 0;
}
