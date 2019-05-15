#pragma once
#include "Person.h"

class InheritanceCalc
{
public:
	InheritanceCalc (Person * person);
	~InheritanceCalc ();

	void UpdateFinances ();

protected:
	Person* person_;
	 
	int checkLevel	 (Person* person);
};

