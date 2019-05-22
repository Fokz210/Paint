#pragma once
#include "Person.h"

class InheritanceCalc
{
public:
	InheritanceCalc (Person ** person);
	~InheritanceCalc ();

	void UpdateFinances ();

	int searchHers ();

	Person** person_;

	bool Empty ();

	void relateHers ();
	void ResetFinances ();


	void ResetAllNames ()
	{
		(*person_)->name_ = "";

		(*person_)->all_rels_.clear ();
		(*person_)->UpdateAllRels (*person_);

		for (auto p : (*person_)->all_rels_)
			p->name_ = "";

	}


protected:



};

