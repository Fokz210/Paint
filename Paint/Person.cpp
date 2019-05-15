#include "Person.h"

Person::Person (std::string name, bool dead, float finances, Person* spouse) :
	parents_ (),
	children_ (),
	spouse_ (spouse),
	name_ (name),
	finances_ (finances),
	dead_ (dead),
	btnSprite_ (false)
{
}

Person::~Person ()
{
}
