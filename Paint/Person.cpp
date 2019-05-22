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

void Person::UpdateAllRels (Person * person)
{
	if (find (all_rels_, person) >= 0)
		return;

	for (auto p : person->parents_)
	{
		addRel (p);
		UpdateAllRels (p);
	}

	for (auto p : person->children_)
	{
		addRel (p);
		UpdateAllRels (p);
	}
	
	for (auto p : person->stepchildren_)
	{
		addRel (p);
		UpdateAllRels (p);
	}

	for (auto p : person->stepparents_)
	{
		addRel (p);
		UpdateAllRels (p);
	}
}

void Person::addRel (Person* rel)
{
	if (find (all_rels_, rel) == -1)
		all_rels_.push_back (rel);
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
