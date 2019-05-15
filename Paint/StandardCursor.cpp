#include "../Include/StandardCursor.h"
#include <iostream>

sf::StandardCursor::StandardCursor(const sf::StandardCursor::TYPE t)
{
    switch(t)
    {
        case sf::StandardCursor::WAIT :
		{
			Cursor = LoadCursor (NULL, IDC_WAIT);
		}
        break;
        case sf::StandardCursor::HAND :
		{
			Cursor = LoadCursor (NULL, IDC_HAND);
		}
        break;
        case sf::StandardCursor::NORMAL :
		{
			Cursor = LoadCursor (NULL, IDC_ARROW);
		}
        break;
        case sf::StandardCursor::TEXT :
		{
			Cursor = LoadCursor (NULL, IDC_IBEAM);
		}
        break;
    }
}

void sf::StandardCursor::Set(const sf::WindowHandle& aWindowHandle) const
{
    SetClassLongPtr(aWindowHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));
}

sf::StandardCursor::~StandardCursor()
{
}
