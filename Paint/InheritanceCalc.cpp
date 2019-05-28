#include "InheritanceCalc.h"

InheritanceCalc::InheritanceCalc (Person** person) :
	person_ (person)
{
}


InheritanceCalc::~InheritanceCalc ()
{
}

void InheritanceCalc::UpdateFinances ()
{
	(*person_)->dead_ = true;

	searchHers ();

	unsigned int NumHer = (*person_)->heirs_.size () + (*person_)->repres_.size ();


	//printf ("Heirs + Representative Groups = %d\n", NumHer);
	//printf ("The dead man had %f finances\n", (*person_)->finances_);

	float part = (*person_)->finances_ / NumHer;

	//printf ("So each heir or group gets %f finances\n", part);

	for (int i = 0; i < (*person_)->heirs_.size (); i++)
	{
		//(*person_)->heirs_[i]->PrintName(0);
		//printf (" used to have %f finances\nNow", (*person_)->heirs_[i]->finances_);
		(*person_)->heirs_[i]->finances_ += part;
		//(*person_)->heirs_[i]->PrintName(0);
		//printf (" has %f finances\n", (*person_)->heirs_[i]->finances_);
	}

	for (int i = 0; i < (*person_)->repres_.size (); i++)
	{
		float part_of_part = part / (*person_)->repres_[i].size ();
		//printf ("IN GROUP %d THERE ARE:\n%d persons, so each of them gets:\n%f finances\n", i + 1, (*person_)->repres_[i].size(), part_of_part);

		for (int j = 0; j < (*person_)->repres_[i].size (); j++)
			(*person_)->repres_[i][j]->finances_ += part_of_part;
	}
	(*person_)->finances_ = 0;

}

void InheritanceCalc::ResetFinances ()
{
	relateHers ();

	for (int i = 0; i < (*person_)->all_rels_.size (); i++)
		(*person_)->all_rels_[i]->finances_ = 0.0f;

	(*person_)->finances_ = 100.0f;
}

void InheritanceCalc::SubRels (Person* relative, std::vector <Person*> other_reps)
{
	std::vector <Person*> reps;

	if (!relative->dead_) return;

	if (relative->spouse_ && !relative->spouse_->dead_ && (*person_)->NotAddedRel (relative->spouse_))
		reps.push_back (relative->spouse_);

	if (!relative->children_.empty ())
		for (auto c : relative->children_)
		{
			if (!c->dead_ && (*person_)->NotAddedRel (c))
			{
				reps.push_back (c);
			}
		}

	if (!relative->parents_.empty ())
		for (auto p : relative->parents_)
		{
			if (!p->dead_ && (*person_)->NotAddedRel (p))
				reps.push_back (p);
		}

	reps.insert (reps.end (), other_reps.begin (), other_reps.end ());

	if (!reps.empty ())
		(*person_)->repres_.push_back (reps);
}

void InheritanceCalc::SubRels (Person* relative)
{
	std::vector <Person*> reps;

	if (!relative->dead_) return;

	if (relative->spouse_ && !relative->spouse_->dead_ && (*person_)->NotAddedRel (relative->spouse_))
		reps.push_back (relative->spouse_);

	if (!relative->children_.empty ())
		for (auto c : relative->children_)
		{
			if (!c->dead_ && (*person_)->NotAddedRel (c))
			{
				reps.push_back (c);
			}
		}

	if (!relative->parents_.empty ())
		for (auto p : relative->parents_)
		{
			if (!p->dead_ && (*person_)->NotAddedRel (p))
				reps.push_back (p);
		}

	//reps.insert(reps.end(), other_reps.begin(), other_reps.end() ); THERE ARE NO MERGES

	if (!reps.empty ())
		(*person_)->repres_.push_back (reps);
}

int InheritanceCalc::searchHers ()
{   // =========================================================
	// ============== NOBODY ===================================
	// =========================================================

	relateHers ();

	if (!(*person_)->spouse_ && (*person_)->parents_.empty () && (*person_)->children_.empty ())
		return 0;

	// =========================================================
	// =============== LEVEL ONE ===============================
	// =========================================================

	if ((*person_)->spouse_)
	{
		if (!(*person_)->spouse_->dead_ && (*person_)->NotAddedHer ((*person_)->spouse_))
			(*person_)->heirs_.push_back ((*person_)->spouse_);

		if ((*person_)->spouse_->dead_)
			SubRels ((*person_)->spouse_);

	}

	if (!(*person_)->parents_.empty ())
		for (auto p : (*person_)->parents_)
		{
			if (!p->dead_ && (*person_)->NotAddedHer (p))
				(*person_)->heirs_.push_back (p);

			if (p->dead_)
				SubRels (p);
		}

	if (!(*person_)->children_.empty ())
		for (auto c : (*person_)->children_)
		{
			if (!c->dead_ && (*person_)->NotAddedHer (c))
				(*person_)->heirs_.push_back (c);

			if (c->dead_)
			{

				std::vector <Person*> c_repres;

				for (auto cc : c->children_)
				{
					if (!cc->dead_ && (*person_)->NotAddedRep (cc))
						c_repres.push_back (cc);
				}

				SubRels (c, c_repres);




			}
		}

	if (!Empty ()) return 1; //NEXT LEVEL

	// =========================================================
	// =============== LEVEL TWO ===============================
	// =========================================================

	if (!(*person_)->parents_.empty ())          //brothas & sisters (parent -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->children_.empty ())
			{
				for (auto pc : p->children_)
				{
					if (!pc->dead_ && (*person_)->NotAddedHer (pc))
						(*person_)->heirs_.push_back (pc);

					if (pc->dead_)
					{

						std::vector <Person*> pc_repres;

						for (auto pcc : pc->children_)
						{
							if (!pcc->dead_ && (*person_)->NotAddedRep (pcc))
								pc_repres.push_back (pcc);
						}

						SubRels (pc, pc_repres);

					}

				}
			}
		}
	}

	if (!(*person_)->parents_.empty ())          //grandparents (parent -> parent)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->dead_ && (*person_)->NotAddedHer (pp))
						(*person_)->heirs_.push_back (pp);

					if (pp->dead_)
						SubRels (pp);
				}
			}
		}
	}

	if (!Empty ()) return 2; //NEXT LEVEL

// =========================================================
// =============== LEVEL THREE =============================
// =========================================================

	if (!(*person_)->parents_.empty ())          //uncles & aunts (parent -> parent -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
					{
						for (auto ppc : pp->children_)
						{
							if (!ppc->dead_ && (*person_)->NotAddedHer (ppc))
								(*person_)->heirs_.push_back (ppc);

							if (ppc->dead_)
							{

								std::vector <Person*> ppc_repres;

								for (auto ppcc : ppc->children_)
								{
									if (!ppcc->dead_ && (*person_)->NotAddedRep (ppcc))
										ppc_repres.push_back (ppcc);
								}

								SubRels (ppc, ppc_repres);

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


	if (!(*person_)->parents_.empty ())          //great-grandparents (parent -> parent -> parent)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->parents_.empty ())
					{
						for (auto ppp : pp->parents_)
						{
							if (!ppp->dead_ && (*person_)->NotAddedHer (ppp))
								(*person_)->heirs_.push_back (ppp);

							if (ppp->dead_)
								SubRels (ppp);
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

	if (!(*person_)->parents_.empty ())     //great-uncles & -aunts (parent -> parent -> parent -> children)
	{
		for (auto p : (*person_)->parents_)
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
									if (!pppc->dead_ && (*person_)->NotAddedHer (pppc))
										(*person_)->heirs_.push_back (pppc);

									if (pppc->dead_)
										SubRels (pppc);
								}
							}

						}
					}

				}
			}
		}
	}



	if (!(*person_)->parents_.empty ())     //cousin grandsons & daughters (parent -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
									if (!pccc->dead_ && (*person_)->NotAddedHer (pccc))
										(*person_)->heirs_.push_back (pccc);

									if (pccc->dead_)
										SubRels (pccc);
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


	if (!(*person_)->parents_.empty ())     // (parent -> parent -> parent -> children -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
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
											if (!pppcc->dead_ && (*person_)->NotAddedHer (pppcc))
												(*person_)->heirs_.push_back (pppcc);

											if (pppcc->dead_)
												SubRels (pppcc);
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



	if (!(*person_)->parents_.empty ())     // (parent -> parent -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
											if (!ppccc->dead_ && (*person_)->NotAddedHer (ppccc))
												(*person_)->heirs_.push_back (ppccc);

											if (ppccc->dead_)
												SubRels (ppccc);
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


	if (!(*person_)->parents_.empty ())     // (parent -> children -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
											if (!pcccc->dead_ && (*person_)->NotAddedHer (pcccc))
												(*person_)->heirs_.push_back (pcccc);

											if (pcccc->dead_)
												SubRels (pcccc);
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


	if (!(*person_)->stepparents_.empty ())
		for (auto sp : (*person_)->stepparents_)
		{
			if (!sp->dead_ && (*person_)->NotAddedHer (sp))
				(*person_)->heirs_.push_back (sp);

			if (sp->dead_)
				SubRels (sp);
		}

	if (!(*person_)->stepchildren_.empty ())
		for (auto sc : (*person_)->stepchildren_)
		{
			if (!sc->dead_ && (*person_)->NotAddedHer (sc))
				(*person_)->heirs_.push_back (sc);

			if (sc->dead_)
				SubRels (sc);

		}

	if (!Empty ()) return 7; //NEXT LEVEL


//=============================================================================


	return 0;

}

void InheritanceCalc::relateHers ()
{
	//MYSELF

	(*person_)->SetRel ("ME");

	// =========================================================
	// =============== LEVEL ONE ===============================
	// =========================================================

	if ((*person_)->spouse_ && (*person_)->NotAddedRel ((*person_)->spouse_))
	{
		(*person_)->all_rels_.push_back ((*person_)->spouse_);
		((*person_)->spouse_->sex_) ? (*person_)->spouse_->SetRel ("WIFE") : (*person_)->spouse_->SetRel ("HUSBAND");
	}

	if (!(*person_)->parents_.empty ())
		for (auto p : (*person_)->parents_)
		{
			if ((*person_)->NotAddedRel (p))
			{
				(*person_)->all_rels_.push_back (p);
			}

			(p->sex_) ? p->SetRel ("MOTHER") : p->SetRel ("FATHER");
		}

	if (!(*person_)->children_.empty ())
		for (auto c : (*person_)->children_)
		{
			if ((*person_)->NotAddedRel (c))
				(*person_)->all_rels_.push_back (c);

			(c->sex_) ? c->SetRel ("DAUGHTER") : c->SetRel ("SON");

			if (!c->children_.empty ())
			{

				for (auto cc : c->children_)
				{
					if ((*person_)->NotAddedRel (cc))
						(*person_)->all_rels_.push_back (cc);

					(cc->sex_) ? cc->SetRel ("GRANDDAUGHTER") : cc->SetRel ("GRANDSON");
				}
			}
		}

	// =========================================================
	// =============== LEVEL TWO ===============================
	// =========================================================

	if (!(*person_)->parents_.empty ())          //brothas & sisters (parent -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->children_.empty ())
			{
				for (auto pc : p->children_)
				{
					if ((*person_)->NotAddedRel (pc))
						(*person_)->all_rels_.push_back (pc);

					(pc->sex_) ? pc->SetRel ("SISTER") : pc->SetRel ("BROTHER");

					if (!pc->children_.empty ())
					{

						for (auto pcc : pc->children_)
						{
							if ((*person_)->NotAddedRel (pcc))
								(*person_)->all_rels_.push_back (pcc);

							(pcc->sex_) ? pcc->SetRel ("NIECE") : pcc->SetRel ("NEPHEW");
						}
					}

				}
			}
		}
	}

	if (!(*person_)->parents_.empty ())          //grandparents (parent -> parent)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if ((*person_)->NotAddedRel (pp))
						(*person_)->all_rels_.push_back (pp);

					(pp->sex_) ? pp->SetRel ("GRANDMOTHER") : pp->SetRel ("GRANDFATHER");
				}
			}
		}
	}


	// =========================================================
	// =============== LEVEL THREE =============================
	// =========================================================

	if (!(*person_)->parents_.empty ())          //uncles & aunts (parent -> parent -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
					{
						for (auto ppc : pp->children_)
						{
							if ((*person_)->NotAddedRel (ppc))
								(*person_)->all_rels_.push_back (ppc);

							(ppc->sex_) ? ppc->SetRel ("AUNT") : ppc->SetRel ("UNCLE");

							if (!ppc->children_.empty ())
							{

								for (auto ppcc : ppc->children_)
								{
									if ((*person_)->NotAddedRel (ppcc))
										(*person_)->all_rels_.push_back (ppcc);

									(ppcc->sex_) ? ppcc->SetRel ("COUSIN-F") : ppcc->SetRel ("COUSIN-M");
								}

							}
						}
					}

				}
			}
		}
	}

	// =========================================================
	// =============== LEVEL FOUR ==============================
	// =========================================================


	if (!(*person_)->parents_.empty ())          //great-grandparents (parent -> parent -> parent)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->parents_.empty ())
					{
						for (auto ppp : pp->parents_)
						{
							if ((*person_)->NotAddedRel (ppp))
								(*person_)->all_rels_.push_back (ppp);

							(ppp->sex_) ? ppp->SetRel ("GREAT-GRANDMOTHER") : ppp->SetRel ("GREAT-GRANDFATHER");
						}
					}

				}
			}
		}
	}

	// =========================================================
	// =============== LEVEL FIVE ==============================
	// =========================================================

	if (!(*person_)->parents_.empty ())     //great-uncles & -aunts (parent -> parent -> parent -> children)
	{
		for (auto p : (*person_)->parents_)
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
									if ((*person_)->NotAddedRel (pppc))
										(*person_)->all_rels_.push_back (pppc);

									(pppc->sex_) ? pppc->SetRel ("GREAT-AUNT [pppc]") : pppc->SetRel ("GREAT-UNCLE [pppc]");
								}
							}

						}
					}

				}
			}
		}
	}



	if (!(*person_)->parents_.empty ())     //cousin grandsons & daughters (parent -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
									if ((*person_)->NotAddedRel (pccc))
										(*person_)->all_rels_.push_back (pccc);

									(pccc->sex_) ? pccc->SetRel ("COUSIN GRANDDAUGHTER") : pccc->SetRel ("COUSIN GRANDSON");
								}
							}

						}
					}

				}
			}
		}
	}


	// =========================================================
	// =============== LEVEL SIX ===============================
	// =========================================================

	if (!(*person_)->parents_.empty ())     // (parent -> parent -> parent -> children -> children)
	{
		for (auto p : (*person_)->parents_)
		{
			if (!p->parents_.empty ())
			{
				for (auto pp : p->parents_)
				{
					if (!pp->children_.empty ())
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
											if ((*person_)->NotAddedRel (pppcc))
												(*person_)->all_rels_.push_back (pppcc);

											(pppcc->sex_) ? pppcc->SetRel ("GREAT-AUNT [pppcc]") : pppcc->SetRel ("GREAT-UNCLE [pppcc]");
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



	if (!(*person_)->parents_.empty ())     // (parent -> parent -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
											if ((*person_)->NotAddedRel (ppccc))
												(*person_)->all_rels_.push_back (ppccc);

											(ppccc->sex_) ? ppccc->SetRel ("GREAT-NIECE") : ppccc->SetRel ("GREAT-NEPHEW");
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


	if (!(*person_)->parents_.empty ())     // (parent -> children -> children -> children -> children)
	{
		for (auto p : (*person_)->parents_)
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
											if ((*person_)->NotAddedRel (pcccc))
												(*person_)->all_rels_.push_back (pcccc);

											(pcccc->sex_) ? pcccc->SetRel ("GREAT-GRANDDAUGHTER COUSIN") : pcccc->SetRel ("GREAT-GRANDSON COUSIN");
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

	// =========================================================
	// =============== LEVEL SEVEN =============================
	// =========================================================


	if (!(*person_)->stepparents_.empty ())
		for (auto sp : (*person_)->stepparents_)
		{
			if ((*person_)->NotAddedRel (sp))
				(*person_)->all_rels_.push_back (sp);

			(sp->sex_) ? sp->SetRel ("STEPMOTHER") : sp->SetRel ("STEPFATHER");
		}

	if (!(*person_)->stepchildren_.empty ())
		for (auto sc : (*person_)->stepchildren_)
		{
			if ((*person_)->NotAddedRel (sc))
				(*person_)->all_rels_.push_back (sc);

			(sc->sex_) ? sc->SetRel ("STEPDAUGHTER") : sc->SetRel ("STEPSON");
		}

	//=============================================================================

}

//-----------------------------------------------------------------------------

bool InheritanceCalc::Empty ()
{
	if (!(*person_)->heirs_.empty ())
		return false;

	for (int i = 0; i < (*person_)->repres_.size (); i++)
		if (!(*person_)->repres_[i].empty ())
		{
			return false;
		}

	return true;

}
