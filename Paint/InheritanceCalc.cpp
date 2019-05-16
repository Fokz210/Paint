#include "InheritanceCalc.h"

InheritanceCalc::InheritanceCalc (Person* person) :
	person_ (person)
{
}


InheritanceCalc::~InheritanceCalc ()
{
}

void InheritanceCalc::UpdateFinances ()
{
	searchHers ();

	unsigned int NumHer = person_->heirs_.size () + person_->repres_.size ();


	printf ("Heirs + Representative Groups = %d\n", NumHer);
	printf ("The dead man had %f finances\n", person_->finances_);

	float part = person_->finances_ / NumHer;

	printf ("So each heir or group gets %f finances\n", part);

	for (int i = 0; i < person_->heirs_.size (); i++)
	{
		person_->heirs_[i]->PrintName (0);
		printf (" used to have %f finances\nNow", person_->heirs_[i]->finances_);
		person_->heirs_[i]->finances_ += part;
		person_->heirs_[i]->PrintName (0);
		printf (" has %f finances\n", person_->heirs_[i]->finances_);
	}

	for (int i = 0; i < person_->repres_.size (); i++)
	{
		float part_of_part = part / person_->repres_[i].size ();
		printf ("IN GROUP %d THERE ARE:\n%d persons, so each of them gets:\n%f finances\n", i + 1, person_->repres_[i].size (), part_of_part);

		for (int j = 0; j < person_->repres_[i].size (); j++)
			person_->repres_[i][j]->finances_ += part_of_part;
	}

	person_->finances_ = 0;
}

int InheritanceCalc::searchHers ()
{   // =========================================================
	// ============== NOBODY LOVES ME ==========================
	// =========================================================

	if (!person_->spouse_ && person_->parents_.empty () && person_->children_.empty ())
		return 0;

	// =========================================================
	// =============== LEVEL ONE ===============================
	// =========================================================

	if (person_->spouse_)
		if (!person_->spouse_->dead_ && person_->NotAddedHer (person_->spouse_))
			person_->heirs_.push_back (person_->spouse_);

	if (!person_->parents_.empty ())
		for (auto p : person_->parents_)
			if (!p->dead_ && person_->NotAddedHer (p))
				person_->heirs_.push_back (p);

	if (!person_->children_.empty ())
		for (auto c : person_->children_)
		{
			if (!c->dead_ && person_->NotAddedHer (c))
				person_->heirs_.push_back (c);

			if (c->dead_ && !c->children_.empty ())
			{
				std::vector <Person*> cc_group;

				for (auto cc : c->children_)
				{
					if (!cc->dead_ && person_->NotAddedRep (cc))
						cc_group.push_back (cc);
				}

				if (!cc_group.empty ())
					person_->repres_.push_back (cc_group);
			}
		}

	if (!Empty ()) return 1; //NEXT LEVEL

	// =========================================================
	// =============== LEVEL TWO ===============================
	// =========================================================

	if (!person_->parents_.empty ())          //brothas & sisters (parent -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->children_.empty ())
			{
				for (auto pc : p->children_)
				{
					if (!pc->dead_ && person_->NotAddedHer (pc))
						person_->heirs_.push_back (pc);

					if (pc->dead_ && !pc->children_.empty ())
					{
						std::vector <Person*> pc_group;

						for (auto pcc : pc->children_)
						{
							if (!pcc->dead_ && person_->NotAddedRep (pcc))
								pc_group.push_back (pcc);
						}

						if (!pc_group.empty ())
							person_->repres_.push_back (pc_group);

					}

				}
			}
		}
	}

	if (!person_->parents_.empty ())          //grandparents (parent -> parent)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->dead_ && person_->NotAddedHer (pp))
						person_->heirs_.push_back (pp);
				}
			}
		}
	}

	if (!Empty ()) return 2; //NEXT LEVEL

// =========================================================
// =============== LEVEL THREE =============================
// =========================================================

	if (!person_->parents_.empty ())          //uncles & aunts (parent -> parent -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
					{
						for (auto ppc : pp->children_)
						{
							if (!ppc->dead_ && person_->NotAddedHer (ppc))
								person_->heirs_.push_back (ppc);

							if (ppc->dead_ && !ppc->children_.empty ())
							{
								std::vector <Person*> ppcc_group;

								for (auto ppcc : ppc->children_)
									if (!ppcc->dead_ && person_->NotAddedRep (ppcc))
										ppcc_group.push_back (ppcc);

								if (!ppcc_group.empty ())
									person_->repres_.push_back (ppcc_group);
							}
						}
					}

				}
			}
		}
	}

	if (!Empty ()) return 3; //NEXT LEVEL

// =========================================================
// =============== LEVEL FOUR ==============================
// =========================================================


	if (!person_->parents_.empty ())          //great-grandparents (parent -> parent -> parent)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->parents_.empty ())
					{
						for (auto ppp : pp->parents_)
						{
							if (!ppp->dead_ && person_->NotAddedHer (ppp))
								person_->heirs_.push_back (ppp);
						}
					}

				}
			}
		}
	}

	if (!Empty ()) return 4; //NEXT LEVEL

// =========================================================
// =============== LEVEL FIVE ==============================
// =========================================================

	if (!person_->parents_.empty ())     //great-uncles & -aunts (parent -> parent -> parent -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->parents_.empty ())
					{
						for (auto ppp : pp->parents_)
						{
							if (!ppp->children_.empty ())
							{
								for (auto pppc : ppp->children_)
								{
									if (!pppc->dead_ && person_->NotAddedHer (pppc))
										person_->heirs_.push_back (pppc);
								}
							}

						}
					}

				}
			}
		}
	}



	if (!person_->parents_.empty ())     //cousin grandsons & daughters (parent -> children -> children -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->children_.empty ())
			{
				for (auto pc : p->children_)
				{
					if (!pc->children_.empty ())
					{
						for (auto pcc : pc->children_)
						{
							if (!pcc->children_.empty ())
							{
								for (auto pccc : pcc->children_)
								{
									if (!pccc->dead_ && person_->NotAddedHer (pccc))
										person_->heirs_.push_back (pccc);
								}
							}

						}
					}

				}
			}
		}
	}

	if (!Empty ()) return 5; //NEXT LEVEL

// =========================================================
// =============== LEVEL SIX ===============================
// =========================================================

	if (!person_->parents_.empty ())     // (parent -> parent -> parent -> children -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->parents_.empty ())
					{
						for (auto ppp : pp->parents_)
						{
							if (!ppp->children_.empty ())
							{
								for (auto pppc : ppp->children_)
								{
									if (!pppc->children_.empty ())
									{
										for (auto pppcc : pppc->children_)
										{
											if (!pppcc->dead_ && person_->NotAddedHer (pppcc))
												person_->heirs_.push_back (pppcc);
										}
									}
								}
							}

						}
					}

				}
			}
		}
	}



	if (!person_->parents_.empty ())     // (parent -> parent -> children -> children -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
					{
						for (auto ppc : pp->children_)
						{
							if (!ppc->children_.empty ())
							{
								for (auto ppcc : ppc->children_)
								{
									if (!ppcc->children_.empty ())
									{
										for (auto ppccc : ppcc->children_)
										{
											if (!ppccc->dead_ && person_->NotAddedHer (ppccc))
												person_->heirs_.push_back (ppccc);
										}
									}
								}
							}

						}
					}

				}
			}
		}
	}


	if (!person_->parents_.empty ())     // (parent -> children -> children -> children -> children)
	{
		for (auto p : person_->parents_)
		{
			if (!p->children_.empty ())
			{
				for (auto pc : p->children_)
				{
					if (!pc->children_.empty ())
					{
						for (auto pcc : pc->children_)
						{
							if (!pcc->children_.empty ())
							{
								for (auto pccc : pcc->children_)
								{
									if (!pccc->children_.empty ())
									{
										for (auto pcccc : pccc->children_)
										{
											if (!pcccc->dead_ && person_->NotAddedHer (pcccc))
												person_->heirs_.push_back (pcccc);
										}
									}
								}
							}

						}
					}

				}
			}
		}
	}

	if (!Empty ()) return 6; //NEXT LEVEL

// =========================================================
// =============== LEVEL SEVEN =============================
// =========================================================


	if (!person_->stepparents_.empty ())
		for (auto sp : person_->stepparents_)
			if (!sp->dead_ && person_->NotAddedHer (sp))
				person_->heirs_.push_back (sp);

	if (!person_->stepchildren_.empty ())
		for (auto sc : person_->stepchildren_)
		{
			if (!sc->dead_ && person_->NotAddedHer (sc))
				person_->heirs_.push_back (sc);

		}

	if (!Empty ()) return 7; //NEXT LEVEL


//=============================================================================


	return 0;

}

//-----------------------------------------------------------------------------

bool InheritanceCalc::Empty ()
{
	if (!person_->heirs_.empty ())
		return false;

	for (int i = 0; i < person_->repres_.size (); i++)
		if (!person_->repres_[i].empty ())
		{
			return false;
		}

	return true;

}
