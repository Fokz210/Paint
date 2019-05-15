#include "../Include/WinManager.h"

sf::ColorButton::ColorButton(Vector2f pos, Vector2f size, Color color, Canvas * canvas) :
	RectWnd(pos.x, pos.y, size.x, size.y, color),
	canvas_(canvas)
{

}

bool sf::ColorButton::OnClick(Event::MouseButtonEvent event)
{
	canvas_->SetColor(shape_.getFillColor());

	return true;
}
