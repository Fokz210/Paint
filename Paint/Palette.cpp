#include "../Include/WinManager.h"

sf::Palette::Palette() :
	currInstPos_(0),
	palette_(),
	manager_(nullptr)
{

}

sf::Palette::Palette(sf::WindowManager * manager) :
	palette_(),
	currInstPos_(0),
	manager_(manager)
{
}

void sf::Palette::AddInst(AbstInst * inst)
{
	palette_.push_back(inst);
}

void sf::Palette::DelInst()
{
	palette_.pop_back();
}

void sf::Palette::SetActiveInst(int currInstPos)
{
	if (currInstPos_ == currInstPos)
		return;

	palette_[currInstPos_]->UnloadWindows (manager_);
	currInstPos_ = currInstPos;
	palette_[currInstPos_]->LoadWindows (manager_);
}

sf::AbstInst * sf::Palette::GetCurrInst()
{
	return palette_[currInstPos_];
}

int sf::Palette::GetCurrInstPos()
{
	return currInstPos_;
}

std::vector<sf::AbstInst*> & sf::Palette::GetPalette()
{
	return palette_;
}
