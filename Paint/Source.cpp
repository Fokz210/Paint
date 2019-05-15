#define NOMINMAX
#define BUFF_STRING_SIZE 128

#include <SFML\Graphics.hpp>
#include <iostream>
#include "../Include/WinManager.h"
#include "resource.h"
#include "../Include/TagTree.h"
#include "PersonButton.h"

int main()
{
	sf::RenderWindow window (sf::VideoMode (800, 600), "INH DEBUG");

	sf::WindowManager manager;

	sf::Image img1, img2, img3;
	img1.create (50, 50, sf::Color::Red);
	img2.create (50, 20, sf::Color::Blue);
	img3.create (50, 90, sf::Color::Transparent);

	sf::Texture iconTexture, addTexture, btnTexture;
	iconTexture.loadFromImage (img1);
	addTexture.loadFromImage (img2);
	btnTexture.loadFromImage (img3);

	sf::Sprite iconSprite (iconTexture), addSprite (addTexture), btnSprite (btnTexture);

	Person inheritant ("", 0, 0);
	
	sf::PersonButton button (btnSprite, addSprite, iconSprite, &inheritant, &manager);
	manager.AddWindow (&button);
	button.IntegrateButtons ();

	inheritant.btnSprite_ = button.GetSprite ();

	while (window.isOpen ())
	{
		window.clear (sf::Color::Color (200, 200, 200, 255));

		manager.Run (&window);

		window.display ();
	}

	return 0;
}
