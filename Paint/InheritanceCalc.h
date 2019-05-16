#pragma once
#include "Person.h"

class InheritanceCalc
{
public:
	InheritanceCalc (Person * person);
	~InheritanceCalc ();

	void UpdateFinances ();

	int searchHers ();

	Person* person_;

	bool Empty ();


protected:



};

