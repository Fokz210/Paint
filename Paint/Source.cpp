#define NOMINMAX
#define BUFF_STRING_SIZE 128

#include <SFML\Graphics.hpp>
#include <iostream>
#include "../Include/WinManager.h"
#include "resource.h"
#include "../Include/TagTree.h"


#include "InheritanceCalc.h"
#include "PersonButton.h"


bool run (sf::RenderWindow& window)
{
	bool restart = false;

	sf::WindowManager manager;

	sf::Image img1, img2, img3, img4, img5;
	img1.create (100, 90, sf::Color::Transparent);
	img2.loadFromFile ("+.png");
	img3.loadFromFile ("Background.png");
	img4.loadFromFile ("ADD.png");
	img5.loadFromFile ("rip.png");

	sf::Texture iconTexture, linkTexture, btnTexture, addTexture, killTexture;
	iconTexture.loadFromImage (img1);
	linkTexture.loadFromImage (img2);
	btnTexture.loadFromImage (img3);
	addTexture.loadFromImage (img4);
	killTexture.loadFromImage (img5);

	sf::Sprite iconSprite (iconTexture), addSprite (addTexture), btnSprite (btnTexture), linkSprite (linkTexture), killSprite (killTexture);

	Person inheritant ("", 0, 100);
	sf::Font font;
	font.loadFromFile ("bin//font.ttf");

	sf::PersonButton button (btnSprite, linkSprite, iconSprite, killSprite, &inheritant, &manager, font);
	manager.AddWindow (&button);
	button.IntegrateButtons ();
	auto lambda1 = [&]() { restart = true; };


	sf::Text text;
	text.setFont (font);

	text.setString ("RESTART");

	sf::Button <decltype (lambda1)> bttn (window.getSize ().x - 150, 0, 150, 50, sf::Color::Red, text, lambda1);

	manager.AddWindow (&bttn);

	addSprite.setPosition (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2, 20);

	sf::AddPersonButton addBtn (addSprite, &manager, linkSprite, iconSprite, btnSprite, killSprite, font);
	manager.AddWindow (&addBtn);

	inheritant.btnSprite_ = button.GetSprite ();

	InheritanceCalc calc (&inheritant);

	auto calclabmbda = [&]() { calc.UpdateFinances (); };

	text.setString ("CALC");
	sf::Button  <decltype (calclabmbda)> calcBtn (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2, 200, 150, 50, sf::Color::Red, text, calclabmbda);
	manager.AddWindow (&calcBtn);

	while (window.isOpen ())
	{
		window.clear (sf::Color::Color (200, 200, 200, 255));

		manager.Run (&window);

		window.display ();

		if (restart) 
			return true;
	}

	return false;
}


int main()
{
	sf::RenderWindow window (sf::VideoMode (1366, 768), "INH DEBUG");

	while (run (window));

	return 0;
}
