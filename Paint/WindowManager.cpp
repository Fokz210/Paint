#include "..\Include\WinManager.h"
#include "../Include/WinManager.h"
#include <iostream>

sf::WindowManager::WindowManager() :
	windows_(),
	activeWindow(nullptr),
	isActive(false)
{
}

sf::WindowManager::WindowManager(std::vector<AbstWnd*> windows) : 
	windows_(windows),
	activeWindow(nullptr),
	isActive(false)
{
}

sf::WindowManager::~WindowManager()
{
}

void sf::WindowManager::Run(sf::RenderWindow * wnd)
{
	static unsigned int activeWindowNum = 0;

	Event event = {};
	while (wnd->pollEvent(event))
	{
		switch (event.type)
		{
		case Event::EventType::MouseButtonPressed:
		{
			for (int i = windows_.size () - 1; i >= 0 ; i--)
			{
				if (windows_[i]->IsHovered(Vector2f(float(event.mouseButton.x), float(event.mouseButton.y))))
				{
					if (activeWindow != nullptr) activeWindow->Deactivate();
					activeWindow = windows_[i];
					activeWindow->Activate();
					activeWindowNum = i;

					if (windows_[i]->OnClick(event.mouseButton)) break;
				}
			}
			break;
		}

		case Event::EventType::MouseMoved:
		{
			StandardCursor cursor(StandardCursor::TYPE::NORMAL);
			cursor.Set(wnd->getSystemHandle());
			for (int i = windows_.size () - 1; i >= 0; i--)
			{
				if (windows_[i]->IsHovered(Vector2f(float(event.mouseMove.x), float(event.mouseMove.y))))
				{
					if (windows_[i]->OnMouseMove(event.mouseMove))
						break;
					StandardCursor cursor(windows_[i]->GetCursorType()); 
					cursor.Set(wnd->getSystemHandle());
				}

			}
			break;
		}

		case Event::EventType::TextEntered:
		{
			for (int i = windows_.size () - 1; i >= 0; i--)
			{
				if (windows_[i]->OnTextEntered(event.text))
					break;
			}
			break;
		}

		case Event::EventType::KeyPressed:
		{
			if (event.key.code == Keyboard::Key::Escape) wnd->close ();
			else if (event.key.code == Keyboard::Key::Tab)
			{
				if (activeWindow != nullptr) activeWindow->Deactivate ();
				activeWindowNum = (activeWindowNum + 1) % windows_.size ();
				activeWindow = windows_[activeWindowNum];
				activeWindow->Activate ();
			}
			else if (event.key.code == Keyboard::Key::Enter)
				activeWindow->OnClick (event.mouseButton);

			for (int i = windows_.size () - 1; i >= 0; i--)
			{
				if (windows_[i]->OnKey(event.key))
					break;
			}
			break;
		}

		case Event::EventType::MouseButtonReleased:
		{
			for (int i = windows_.size () - 1; i >= 0; i--)
			{
				if (windows_[i]->OnRelease (event.mouseButton))
					break;
			}
			break;
		}

		case Event::EventType::Closed:
		{
			wnd->close();
			break;
		}

		default:
			break;
		}

		

	}
	for (unsigned int i = 0; i < windows_.size(); i++)
	{
		windows_[i]->Draw(wnd);
	}

}

void sf::WindowManager::Activate()
{
	isActive = true;
}

void sf::WindowManager::Deactivate()
{
	isActive = false;
}

bool sf::WindowManager::OnClick(Event::MouseButtonEvent event)
{
	return true;
}

bool sf::WindowManager::OnKey(Event::KeyEvent event)
{
	return false;
}

bool sf::WindowManager::OnTextEntered(Event::TextEvent event)
{
	return false;
}

void sf::WindowManager::Draw(RenderWindow * wnd)
{
}

bool sf::WindowManager::IsHovered(Vector2f pos)
{
	return false;
}

sf::StandardCursor::TYPE sf::WindowManager::GetCursorType()
{
	return StandardCursor::TYPE();
}

bool sf::WindowManager::OnMouseMove(Event::MouseMoveEvent event)
{
	return false;
}

void sf::WindowManager::AddWindow(AbstWnd * window)
{
	windows_.push_back(window);
}

void sf::WindowManager::DeleteWindow ()
{
	windows_.pop_back ();
}

bool sf::WindowManager::OnRelease (Event::MouseButtonEvent event)
{
	return false;
}
