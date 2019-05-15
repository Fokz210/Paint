#include "..//include/WinManager.h"

sf::TextureSampler::TextureSampler(Texture texturePack) :
	texturePack_(texturePack)
{
}

void sf::TextureSampler::Sample(Texture * texture)
{
	RenderTexture target;
	target.create(texture->getSize().x, texture->getSize().y);
	target.clear (Color::Transparent);
	Sprite s(texturePack_);

	s.setTextureRect(IntRect(40, 0, 10, 10));

	for (unsigned int x = 10; x < texture->getSize ().x - 10; x += 10)
	{
		if (x > texture->getSize ().x - 20)
			s.setTextureRect (IntRect (40, 0, texture->getSize ().x % 10, s.getTextureRect().height));
		else 
			s.setTextureRect (IntRect (40, 0, 10, s.getTextureRect ().height));
		for (unsigned int y = 10; y < texture->getSize ().y - 10; y += 10)
		{
			if (y > texture->getSize ().y - 20)
				s.setTextureRect (IntRect (40, 0, s.getTextureRect ().width, texture->getSize ().y % 10));
			else
				s.setTextureRect (IntRect (40, 0, s.getTextureRect ().width, 10));
			s.setPosition (x, y);
			target.draw (s);
		}
	}

	s.setTextureRect(IntRect(0, 0, 10, 10));

	for (unsigned int x = 10; x < texture->getSize ().x - 10; x += 10)
	{
		s.setPosition (x, 0);
		target.draw (s);
	}

	s.setTextureRect(IntRect(10, 0, 10, 10));

	for (unsigned int y = 10; y < texture->getSize ().y - 10; y += 10)
	{
		s.setPosition (0, y);
		target.draw (s);
	}

	s.setTextureRect(IntRect(20, 0, 10, 10));

	for (unsigned int x = 10; x < texture->getSize ().x - 10; x += 10)
	{
		s.setPosition (x, texture->getSize ().y - 10);
		target.draw (s);
	}

	s.setTextureRect(IntRect(30, 0, 10, 10));

	for (unsigned int y = 10; y < texture->getSize ().y - 10; y += 10)
	{
		s.setPosition (texture->getSize ().x - 10, y);
		target.draw (s);
	}

	s.setTextureRect(IntRect(0, 10, 10, 10));
	s.setPosition(0, 0);
	target.draw(s);

	s.setTextureRect(IntRect(10, 10, 10, 10));
	s.setPosition(target.getSize().x - 10, 0);
	target.draw(s);

	s.setTextureRect(IntRect(20, 10, 10, 10));
	s.setPosition(0, target.getSize().y - 10);
	target.draw(s);

	s.setTextureRect(IntRect(30, 10, 10, 10));
	s.setPosition(target.getSize().x - 10, target.getSize().y - 10);
	target.draw(s);

	target.display();

	*texture = target.getTexture();
}
