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

	enum state
	{
		none,
		active,
		passive
	};

	enum gender
	{
		Male,
		Female,
		Agender,
		Androgyne,
		Androgynous,
		CisFemale,
		CisMale,
		CisMan,
		CisWoman,
		Cisgender,
		CisgenderFemale,
		CisgenderMale,
		CisgenderMan,
		CisgenderWoman,
		FemaletoMale,
		FTM,
		GenderFluid,
		GenderNonconforming,
		GenderQuestioning,
		GenderVariant,
		Genderqueer,
		Intersex,
		MaletoFemale,
		MTF,
		Neither,
		Neutrois,
		Nonbinary,
		Other,
		Pangender,
		TransFemale,
		TransMale,
		TransMan,
		TransPerson,
		TransWoman,
		TransasteriskFemale,
		TransasteriskMale,
		TransasteriskMan,
		TransasteriskPerson,
		TransasteriskWoman,
		TransexualFemale,
		TransexualMale,
		TransexualMan,
		TransexualPerson,
		TransexualWoman,
		TransgenderFemale,
		TransgenderMale,
		TransgenderMan,
		TransgenderPerson,
		TransgenderWoman,
		Transmasculine,
		Twospirit,
	};



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
	bool NotAddedRel (Person* Check);

	void SetRel (std::string relative)
	{
		if (name_ == "")
			name_ = relative;
	}
	std::vector <Person*> all_rels_;
    void PrintHerRep();
	
	sf::Sprite * btnSprite_;

	state state_;
	gender sex_;

};

