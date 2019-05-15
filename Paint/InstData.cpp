#include "../Include/WinManager.h"

sf::InstData::InstData(const char * name, sf::Texture * icon_pressed, sf::Texture * icon_released) :
	name_(name),
	icon_pressed_(icon_pressed),
	icon_released_(icon_released)
{
}	