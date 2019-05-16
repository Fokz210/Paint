#pragma once
#include <vector>
#include <string>
#include <ctime>
#include "stdio.h"
#include <iostream>
#include "SFML/Graphics.hpp"

class Person
{
public:
	Person (std::string name, bool dead, float finances = 0.0, Person* spouse = nullptr);
	~Person ();

	std::vector <Person*> parents_, children_;
	std::vector <Person*> stepparents_, stepchildren_;
	Person* spouse_;
	std::string name_;

	float finances_;
	bool dead_;

	std::vector <Person*> heirs_;
	std::vector <std::vector <Person*>> repres_;

	void PrintName (bool new_str = true)
        {
        std::cout << name_;
        if (new_str) std::cout << "\n";
        }

    bool NotAddedHer (Person* Check);
    bool NotAddedRep (Person* Check);

    void PrintHerRep();
	
	sf::Sprite * btnSprite_;

};

