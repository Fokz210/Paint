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

	sf::Color zvet_yayza_drozda (0, 204, 204);

	sf::WindowManager manager;

	sf::Image img1, img2, img3, img4, img5, img6;
	img1.create (100, 90, sf::Color::Transparent);
	img2.loadFromFile ("+.png");
	img3.loadFromFile ("Background.png");
	img4.loadFromFile ("ADD_MALE.png");
	img5.loadFromFile ("ADD_FEMALE.png");
	img6.loadFromFile ("rip.png");

	sf::Texture iconTexture, linkTexture, btnTexture, addMTexture, addFTexture, killTexture;
	iconTexture.loadFromImage (img1);
	linkTexture.loadFromImage (img2);
	btnTexture.loadFromImage (img3);
	addMTexture.loadFromImage (img4);
	addFTexture.loadFromImage (img5);
	killTexture.loadFromImage (img6);

	sf::Sprite iconSprite (iconTexture), addSprite (addMTexture), btnSprite (btnTexture), linkSprite (linkTexture), killSprite (killTexture);

	Person** inheritant = new Person*;
	*inheritant = new Person ("me", false, 100);
	sf::Font font;
	font.loadFromFile ("bin//font.ttf");

	btnSprite.setPosition (window.getSize ().x / 2, window.getSize ().y / 2);

	sf::PersonButton button (btnSprite, linkSprite, iconSprite, killSprite, *inheritant, inheritant, &manager, font);
	manager.AddWindow (&button);
	button.IntegrateButtons ();
	auto lambda1 = [&]() { restart = true; };

	sf::Image icon;
	icon.loadFromFile ("icon.png");
	window.setIcon (icon.getSize ().x, icon.getSize ().y, icon.getPixelsPtr ());

	sf::Text text;
	text.setFont (font);

	text.setString ("RESTART");

	sf::Button <decltype (lambda1)> bttn (window.getSize ().x - 150, 0, 150, 50, zvet_yayza_drozda, text, lambda1);
	manager.AddWindow (&bttn);

	addSprite.setPosition (window.getSize ().x / 2 - addSprite.getLocalBounds ().width - 20, 20);
	sf::Sprite addSprite2 = addSprite;
	addSprite2.setPosition (window.getSize ().x / 2 + 20, 20);
	addSprite2.setTexture (addFTexture);

	sf::AddPersonButton addBtn (addSprite, Person::Male, inheritant, &manager, linkSprite, iconSprite, btnSprite, killSprite, font);
	manager.AddWindow (&addBtn);
	sf::AddPersonButton addBtn2 (addSprite2, Person::Female, inheritant, &manager, linkSprite, iconSprite, btnSprite, killSprite, font);
	manager.AddWindow (&addBtn2);

	(*inheritant)->btnSprite_ = button.GetSprite ();

	InheritanceCalc calc (inheritant);

	auto calclabmbda = [&]() { calc.UpdateFinances (); };

	text.setCharacterSize (20);

	text.setString ("Calc");
	sf::Button  <decltype (calclabmbda)> calcBtn (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2, 140, 150, 60, zvet_yayza_drozda, text, calclabmbda);
	manager.AddWindow (&calcBtn);

	auto resetFinLambda = [&]() {calc.ResetFinances (); };

	text.setString ("Reset\nFinances");
	sf::Button  <decltype (resetFinLambda)> rfinBtn (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2 + 150 + 20, 140, 150, 60, zvet_yayza_drozda, text, resetFinLambda);
	manager.AddWindow (&rfinBtn);

	auto resetNameLambda = [&]() {calc.relateHers (); };

	text.setString ("Reset\nNames");
	sf::Button  <decltype (resetNameLambda)> rnmBtn (window.getSize ().x / 2 - addSprite.getLocalBounds ().width / 2 - 150 - 20, 140, 150, 60, zvet_yayza_drozda, text, resetNameLambda);
	manager.AddWindow (&rnmBtn);

	float color = 200;

	while (window.isOpen ())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) color += 0.01;
		

		window.clear (sf::Color::Color (color, color, color, 255));

		manager.Run (&window);

		window.display ();

		if (restart) 
			return true;
	}

	delete (*inheritant);
	delete (inheritant);

	return false;
}


int main()
{
	sf::RenderWindow window (sf::VideoMode (sf::VideoMode::getDesktopMode ().width, sf::VideoMode::getDesktopMode ().height), "Inheritance", sf::Style::Fullscreen);

	while (run (window));

	return 0;
}
