#include "../Include/WinManager.h"
#include <iostream>

sf::InstrumentButton::InstrumentButton(Sprite sprite, Palette * palette, int instNum) :
	SpriteWnd(sprite),
	palette_(palette),
	INST_NUM(instNum)
{
}

bool sf::InstrumentButton::OnClick(Event::MouseButtonEvent)
{
	palette_->SetActiveInst(INST_NUM);
	return true;
}