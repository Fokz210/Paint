#include "InheritanceCalc.h"

InheritanceCalc::InheritanceCalc (Person * person) :
	person_ (person)
{
}

InheritanceCalc::~InheritanceCalc ()
{
}

void InheritanceCalc::UpdateFinances ()
{
	

	
}

int InheritanceCalc::checkLevel (Person* person)
{
	// =========================================================
	// =============== LEVEL ONE ===============================
	// =========================================================

	if (person->spouse_)
		if (!person->spouse_->dead_)
			return 1;

	if (!person->parents_.empty ())
		for (auto p : person_->parents_)
			if (!p->dead_)
				return 1;

	if (!person->children_.empty ())
		for (auto p : person_->parents_)
		{
			if (p->dead_)
			{
				if (!p->children_.empty ())
				{
					for (auto c : p->children_)
					{
						if (!c->dead_)
							return 1;
					}
				}
			}
			else
				return 1;
		}

	// =========================================================
	// =============== LEVEL TWO ===============================
	// =========================================================

	// todo: продолжить остальное по примеру
}
			


