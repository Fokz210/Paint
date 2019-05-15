#include "../Include/WinManager.h"
#include "../Include/TagTree.h"
#include <string>
#include <algorithm>

sf::Canvas::Canvas (sf::Vector2f canvSize, sf::Vector2f pos, sf::Color color, float thickness, Palette * palette, sf::RenderWindow * window, sf::Texture * background) :
	RectWnd(pos.x, pos.y, canvSize.x, canvSize.y, color),
	texture_(),
	shapeSprite_(),
	window_(window),
	color_(color),
	thickness_(thickness),
	palette_(palette),
	settings (),
	prevKey(),
	history()
{
	texture_.create(unsigned int (canvSize.x), unsigned int (canvSize.y));
	if (background != nullptr)
		texture_.draw(Sprite(*background));
	
	shapeSprite_.setTexture(texture_.getTexture());
	shapeSprite_.setPosition(pos);
}

void sf::Canvas::LoadTexture (Texture texture)
{
	texture_.draw(Sprite(texture));
}

sf::Texture sf::Canvas::GetTexture ()
{
	return texture_.getTexture();
}

sf::RenderTexture * sf::Canvas::GetRenderTexture ()
{
	return &texture_;
}

void sf::Canvas::SetTexture (Texture texture)
{
	texture_.draw (Sprite (texture));
}

bool sf::Canvas::OnClick (Event::MouseButtonEvent event)
{
	history.push_back (texture_.getTexture ().copyToImage ());
	mouseHold = true;
	palette_->GetCurrInst()->Apply(Vector2f(texture_.mapPixelToCoords(Mouse::getPosition(*window_)).x - shape_.getPosition().x, Mouse::getPosition(*window_).y - shape_.getPosition().y), thickness_, color_, &texture_);
	return true;
}

bool sf::Canvas::OnClick ()
{
	history.push_back (texture_.getTexture ().copyToImage ());
	palette_->GetCurrInst()->Apply(Vector2f(texture_.mapPixelToCoords(Mouse::getPosition(*window_)).x - shape_.getPosition().x, Mouse::getPosition(*window_).y - shape_.getPosition().y), thickness_, color_, &texture_);
	mouseHold = true;
	return true;
}

void sf::Canvas::Draw (RenderWindow * wnd)
{
	texture_.display();
	shapeSprite_.setTexture(texture_.getTexture());
	window_->draw(shapeSprite_);
	if (IsHovered(Vector2f(texture_.mapPixelToCoords(Mouse::getPosition(*window_))))) palette_->GetCurrInst()->Preview(Vector2f(texture_.mapPixelToCoords(Mouse::getPosition(*window_))), thickness_, color_, window_);
}

void sf::Canvas::Clear (sf::Color clearColor)
{
	texture_.clear(clearColor);
}
 
void sf::Canvas::SetThickness (float thickness)
{
	thickness_ = thickness;
}

void sf::Canvas::SetColor (Color color)
{
	color_ = color;
}

bool sf::Canvas::OnKey (Event::KeyEvent event)
{
	switch (event.code)
	{
	case Keyboard::Key::Q:
	{
		SetColor(sf::Color::Black);
		break;
	}

	case Keyboard::Key::W:
	{
		SetColor(sf::Color::Red);
		break;
	}

	case Keyboard::Key::E:
	{
		SetColor(sf::Color::Green);
		break;
	}

	case Keyboard::Key::R:
	{
		SetColor(sf::Color::Blue);
		break;
	}

	case Keyboard::Key::A:
	{
		if (palette_->GetCurrInstPos() > 0) palette_->SetActiveInst(palette_->GetCurrInstPos() - 1);
		break;
	}

	case Keyboard::Key::S:
	{
		if ((unsigned int)palette_->GetCurrInstPos() < palette_->GetPalette().size() - 1) palette_->SetActiveInst(palette_->GetCurrInstPos() + 1);
		break;
	}

	case Keyboard::Key::Z:
	{
		if (prevKey == sf::Keyboard::Key::LControl)
		{
			Cancel ();
			break;
		}

		if (thickness_ > 0) thickness_ -= 5;
		break;
	}

	case Keyboard::Key::X:
	{
		thickness_ += 5;
		break;
	}

	default:
		break;
	}

	prevKey = event.code;

	return false;
}

void sf::Canvas::SetActiveInst (int CurrInstPos, sf::WindowManager * manager)
{
	palette_->SetActiveInst(CurrInstPos);
}

sf::Color sf::Canvas::GetColor ()
{
	return color_;
}

float sf::Canvas::GetThickness ()
{

	return thickness_;
}

bool sf::Canvas::ReadXML (FILE * file)
{
	if (!file)
		return false;

	settings.loadFromFile (file);

	thickness_ = stoi (settings.getDataByPath ("paletteXML/globalSettings/size", "10"));
	
	string color = settings.getDataByPath ("paletteXML/globalSettings/color", "255 255 255");
	int r, g, b;
	sscanf_s (color.c_str (), "%d %d %d", &r, &g, &b);
	color_ = Color (r, g, b);

	palette_->SetActiveInst (stoi (settings.getDataByPath ("paletteXML/globalSettings/currInst", "0")));

	return true;
}

bool sf::Canvas::WriteXML (FILE * file)
{
	if (!file)
		return false;
	
	settings.setDataByPath ("paletteXML/globalSettings/size", std::to_string (thickness_));

	char buffer[BUFF_STRING_SIZE] = "";
	sprintf_s <BUFF_STRING_SIZE> (buffer, "%d %d %d", color_.r, color_.g, color_.b);
	settings.setDataByPath ("paletteXML/globalSettings/color", buffer);

	settings.setDataByPath ("paletteXML/globalSettings/currInst", std::to_string (palette_->GetCurrInstPos ()));

	settings.saveToFile (file);
		
	return true;
}

bool sf::Canvas::OnRelease (Event::MouseButtonEvent event)
{
	if (mouseHold)
	{
		mouseHold = false;
		return true;
	}
	return false;
}

bool sf::Canvas::OnMouseMove (Event::MouseMoveEvent event)
{
	if (mouseHold)
		palette_->GetCurrInst ()->Apply (Vector2f (texture_.mapPixelToCoords (Mouse::getPosition (*window_)).x - shape_.getPosition ().x, Mouse::getPosition (*window_).y - shape_.getPosition ().y), thickness_, color_, &texture_);
	return mouseHold;
}

void sf::Canvas::Cancel ()
{
	if (history.size ())
	{
		sf::Texture texture;
		texture.loadFromImage (history[history.size () - 1]);
		texture_.draw (Sprite (texture));
		history.pop_back ();
	}
	
}

