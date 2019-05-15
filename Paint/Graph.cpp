#include "../Include/WinManager.h"

sf::Graph::Graph (float x, float y, sf::WindowManager * manager, float width, float height, Color background) :
	RectWnd (x, y, width, height, background),
	manager_ (manager),
	graph ()
{
}

sf::Graph::~Graph ()
{
	for (SlidingWnd * el : keyPoints)
		delete (el);
}

void sf::Graph::insertPoint (float x, float y, float size, Color color)
{
	SlidingWnd * newPoint = new SlidingWnd (x, y, size, color);
	keyPoints.push_back (newPoint);
	manager_->AddWindow (newPoint);
}

void sf::Graph::insertPoint (float x, float y, float size, unsigned position, Color color)
{
	SlidingWnd * newPoint = new SlidingWnd (x, y, size, color);
	keyPoints.push_back (newPoint);
	ROR (keyPoints, position, keyPoints.size () - 1);
	manager_->AddWindow (newPoint);
}

void sf::Graph::Draw (RenderWindow * wnd)
{
	RectWnd::Draw (wnd);

	sf::Vertex line1[] =
	{
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize().x / 3.0f, 0.0f)),
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize ().x / 3.0f, shape_.getSize().y))
	};

	wnd->draw (line1, 2, sf::Lines);

	sf::Vertex line2[] =
	{
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize ().x / 3.0f * 2.0f, 0.0f)),
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize ().x / 3.0f * 2.0f, shape_.getSize ().y))
	};

	wnd->draw (line2, 2, sf::Lines);

	sf::Vertex line3[] =
	{
		sf::Vertex (shape_.getPosition () + sf::Vector2f (0.0f, shape_.getSize ().y / 3.0f)),
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize().x, shape_.getSize ().y / 3.0f))
	};

	wnd->draw (line3, 2, sf::Lines);

	sf::Vertex line4[] =
	{
		sf::Vertex (shape_.getPosition () + sf::Vector2f (0.0f, shape_.getSize ().y / 3.0f * 2.0f)),
		sf::Vertex (shape_.getPosition () + sf::Vector2f (shape_.getSize ().x, shape_.getSize ().y / 3.0f * 2.0f))
	};

	wnd->draw (line4, 2, sf::Lines);

	getGraph ();

	for (unsigned i = 1; i < 256; i++)
	{
		sf::Vertex line[] =
		{
			sf::Vertex (shape_.getPosition () + sf::Vector2f (i - 1, 256 - graph[i - 1])),
			sf::Vertex (shape_.getPosition () + sf::Vector2f (i, 256 - graph[i]))
		};

		wnd->draw (line, 2, sf::Lines);
	}
}

bool sf::Graph::OnClick (Event::MouseButtonEvent event)
{
	for (unsigned i = 0; i < keyPoints.size (); i++)
	{
		if (keyPoints[i]->getPos ().x > event.x || keyPoints[i]->getPos ().y < event.y)
		{
			insertPoint (float (event.x), float (event.y), 10.0f, i, Color::Red);
			keyPoints[i]->OnClick (event);
			break;
		}
	}

	return true;
}

void sf::Graph::removePoints (WindowManager * manager)
{
	for (SlidingWnd * el : keyPoints)
	{
		manager->DeleteWindow ();
		delete(el);
	}

	keyPoints.clear ();
}

float * sf::Graph::getGraph ()
{
	const vector <SlidingWnd*> & keyPoints_ = keyPoints;

	const int N = keyPoints_.size () + 2;

	double xx[128] = {};
	for (int i = 1; i < N - 1; i++)
		xx[i] = keyPoints_[i - 1]->getPos ().x - shape_.getPosition().x;

	xx[0] = 0;
	xx[N - 1] = 256;

	double yy[128] = {};
	for (int i = 1; i < N - 1; i++)
		yy[i] = 256 - (keyPoints_[i - 1]->getPos ().y - shape_.getPosition ().y);

	yy[0] = 0;
	yy[N - 1] = 256;

	for (int i = 1; i < N - 2; i++)
	{
		for (double t = 0; t <= 1; t += 0.001)
		{
			if (xx[i] < 0 || xx[i] > 255 || yy[i] < 0 || yy[i] > 255)
				continue;

			double x = CatmullRom (&xx[i], 1, t);
			double y = CatmullRom (&yy[i], 1, t);

			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x > 255) x = 255;
			if (y > 255) y = 255;
			
			graph[int(x)] = y;
		}
	}

	return graph;
}
