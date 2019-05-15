#include "../Include/WinManager.h"

sf::EventEngine::EventEngine (std::vector<WindowManager*>* managers) :
	managers_(managers)
{
}

void sf::EventEngine::Run (RenderWindow * wnd)
{
	StandardCursor cursor (StandardCursor::TYPE::NORMAL);
	cursor.Set (wnd->getSystemHandle ());

	Event event;

	while (wnd->pollEvent (event))
	{
		for (unsigned int i = 0; i < managers_->size (); i++)
			(*managers_)[i]->Run(event, wnd);
	}

	for (unsigned int i = 0; i < managers_->size (); i++)
		(*managers_)[i]->DrawWindows (wnd);
}
