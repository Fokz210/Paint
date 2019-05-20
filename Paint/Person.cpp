#include "Person.h"

Person::Person (std::string name, bool dead, float finances, Person* spouse) :
	parents_ (),
	children_ (),
	spouse_ (spouse),
	name_ (name),
	finances_ (finances),
	dead_ (dead),
	btnSprite_ (nullptr),
	state_ (none),
	sex_ (Male),
	all_rels_ ()
{
}



Person::~Person ()
{
}

void Person::PrintHerRep ()
{
	printf ("The heirs of this person are:\n");
	for (unsigned int i = 0; i < heirs_.size (); i++)
		std::cout << heirs_[i]->name_ << "\n";

	printf ("The representatives of this person are:\n");
	for (unsigned int i = 0; i < repres_.size (); i++)
	{
		printf ("NEW GROUP:\n");
		for (unsigned int j = 0; j < repres_[i].size (); j++)
		{
			std::cout << repres_[i][j]->name_ << "\n";
		}
	}

}

bool Person::NotAddedHer (Person* Check)
{
	for (int i = 0; i < heirs_.size (); i++)
	{
		if (Check == heirs_[i])
			return false;
	}
	return true;
}

bool Person::NotAddedRep (Person* Check)
{
	for (unsigned int i = 0; i < repres_.size (); i++)
	{
		for (unsigned int j = 0; j < repres_[i].size (); j++)
		{
			if (Check == repres_[i][j])
				return false;
		}
	}
	return true;
}
