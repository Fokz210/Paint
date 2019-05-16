#define NOMINMAX
#define BUFF_STRING_SIZE 128

#include <SFML\Graphics.hpp>
#include <iostream>
#include "../Include/WinManager.h"
#include "resource.h"
#include "../Include/TagTree.h"



#include "PersonButton.h"


bool run (sf::RenderWindow& window)
{
	bool restart = false;

	sf::WindowManager manager;

	sf::Image img1, img2, img3, img4;
	img1.create (100, 90, sf::Color::Transparent);
	img2.loadFromFile ("+.png");
	img3.loadFromFile ("Background.png");
	img4.loadFromFile ("ADD.png");

	sf::Texture iconTexture, linkTexture, btnTexture, addTexture;
	iconTexture.loadFromImage (img1);
	linkTexture.loadFromImage (img2);
	btnTexture.loadFromImage (img3);
	addTexture.loadFromImage (img4);

	sf::Sprite iconSprite (iconTexture), addSprite (addTexture), btnSprite (btnTexture), linkSprite (linkTexture);

	Person inheritant ("", 0, 0);

	sf::PersonButton button (btnSprite, linkSprite, iconSprite, &inheritant, &manager);
	manager.AddWindow (&button);
	button.IntegrateButtons ();
	auto lambda1 = [&]() { restart = true; };

	sf::Font font;
	font.loadFromFile ("bin//font.ttf");

	sf::Text text;
	text.setFont (font);

	text.setString ("RESTART");

	sf::Button <decltype (lambda1)> bttn (window.getSize ().x - 150, 0, 150, 50, sf::Color::Red, text, lambda1);

	manager.AddWindow (&bttn);

	addSprite.setPosition (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2, 20);

	sf::AddPersonButton addBtn (addSprite, &manager, linkSprite, iconSprite, btnSprite);
	manager.AddWindow (&addBtn);

	inheritant.btnSprite_ = button.GetSprite ();

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
