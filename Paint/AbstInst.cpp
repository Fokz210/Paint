#include "../Include/WinManager.h"

sf::AbstInst::AbstInst(InstData instData) :
	instData_(instData)
{

}

sf::InstData sf::AbstInst::GetInstData()
{
	return instData_;
}

void sf::AbstInst::LoadWindows (sf::WindowManager * manager)
{

}

void sf::AbstInst::UnloadWindows (sf::WindowManager * manager)
{
}

void sf::AbstInst::ReadXML (FILE * file)
{
}

void sf::AbstInst::WriteXML (FILE * file)
{
}


