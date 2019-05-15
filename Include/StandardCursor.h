#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <Windows.h>

namespace sf
{
	class StandardCursor
	{
	public:
		enum TYPE
		{
			WAIT,
			TEXT,
			NORMAL,
			HAND
		};

		StandardCursor(const TYPE t);
		~StandardCursor();
		void Set(const sf::WindowHandle & aWindowHandle) const;

	private:
		HCURSOR Cursor;
	};
}