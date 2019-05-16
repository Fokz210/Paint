#include "PersonButton.h"

namespace sf
{
	
	__KOSTYL__ADDITIONAL__ KOSTYL = {};

	LinkPersonButton::LinkPersonButton (Sprite sprite, Sprite PersonBtnSprite, Sprite iconSprite, Person * person, WindowManager * manager, relatives currrent) :
		SpriteWnd		  (sprite),
		person_			  (person),
		PersonBtnSprite_  (PersonBtnSprite),
		iconSprite_		  (iconSprite),
		manager_		  (manager),
		currentRelatives_ (currrent)
	{
	}

	StandardCursor::TYPE sf::LinkPersonButton::GetCursorType ()
	{
		return StandardCursor::TYPE::HAND;
	}

	bool LinkPersonButton::OnClick (Event::MouseButtonEvent event)
	{
		if (!KOSTYL.p1)
			KOSTYL = { person_, currentRelatives_ };
		else if (KOSTYL.r1 == currentRelatives_ || KOSTYL.p1 == person_)
			KOSTYL = {};
		else
		{
			if (currentRelatives_ == children)
			{
				KOSTYL.p1->parents_.push_back (person_);
				person_->children_.push_back (KOSTYL.p1);
			}
			else
			{
				KOSTYL.p1->children_.push_back (person_);
				person_->parents_.push_back (KOSTYL.p1);
			}

			KOSTYL = {};
		}
		
		return true;
	}

	Sprite & LinkPersonButton::getSprite ()
	{
		return sprite_;
	}

	PersonButton::PersonButton (Sprite sprite, Sprite addBtnSprite, Sprite iconSprite, Sprite killSprite, Person* person, WindowManager * manager, Font font) :
		SpriteWnd	 (sprite),
		person_		 (person),
		addChildren_ (nullptr),
		addParents_  (nullptr),
		icon_		 (iconSprite),
		manager_	 (manager),
		holding_	 (false),
		msdelta_	 (),
		font_		 (font),
		kill_		 (nullptr)
	{
		addBtnSprite.setPosition (sprite.getPosition ());
		addParents_ = new LinkPersonButton (addBtnSprite, sprite, iconSprite, person, manager, parents);

		killSprite.setPosition (sprite_.getPosition ().x + addBtnSprite.getLocalBounds ().width - killSprite.getLocalBounds ().width, sprite_.getPosition ().y + addBtnSprite.getLocalBounds ().height);
		kill_ = new KillPersonButton (killSprite, person_);

		addBtnSprite.setPosition (Vector2f (sprite.getPosition ().x, sprite.getPosition ().y + sprite.getLocalBounds ().height - addBtnSprite.getLocalBounds ().height));
		addChildren_ = new LinkPersonButton (addBtnSprite, sprite, iconSprite, person, manager, children);

		icon_.setPosition (Vector2f (sprite.getPosition ().x, sprite.getPosition ().y + addBtnSprite.getLocalBounds ().height));
	}

	void PersonButton::IntegrateButtons ()
	{
		manager_->AddWindow (addParents_);
		manager_->AddWindow (addChildren_);
		manager_->AddWindow (kill_);
	}

	bool PersonButton::OnClick (Event::MouseButtonEvent event)
	{
		holding_ = !holding_;
		msdelta_ = Vector2f (event.x - sprite_.getPosition ().x, event.y - sprite_.getPosition ().y);

		return true;
	}

	bool PersonButton::OnRelease (Event::MouseButtonEvent event)
	{
		holding_ = false;
		msdelta_ = Vector2f ();

		return true;
	}

	void PersonButton::Draw (RenderWindow* wnd)
	{
		if (holding_)
		{
			Vector2i mousepos = sf::Mouse::getPosition (*wnd);
			sprite_.setPosition (Vector2f (mousepos.x - msdelta_.x, mousepos.y - msdelta_.y));
			addParents_->getSprite ().setPosition (sprite_.getPosition ());
			addChildren_->getSprite ().setPosition (Vector2f (sprite_.getPosition ().x, sprite_.getPosition ().y + sprite_.getLocalBounds ().height - addChildren_->getSprite ().getLocalBounds ().height));
			icon_.setPosition (Vector2f (sprite_.getPosition ().x, sprite_.getPosition ().y + addParents_->getSprite ().getLocalBounds ().height));
			kill_->GetSprite ().setPosition (sprite_.getPosition ().x + 100 - kill_->GetSprite ().getLocalBounds ().width, sprite_.getPosition ().y + 25);
		}

		SpriteWnd::Draw (wnd);

		wnd->draw (icon_);

		sf::Text text;
		text.setFont (font_);
		text.setString (std::to_string ((int)person_->finances_));
		text.setOrigin (text.getLocalBounds ().width / 2, text.getLocalBounds ().height / 2);
		text.setPosition (sprite_.getPosition ().x + sprite_.getLocalBounds ().width / 2, sprite_.getPosition ().y + sprite_.getLocalBounds ().height / 2);
		text.setFillColor (Color::Magenta);

		wnd->draw (text);

		if (person_->dead_)
		{
			text.setString ("DEAD");
			text.setPosition (text.getPosition ().x + 130, text.getPosition ().y);
			text.setFillColor (Color::Red);
			wnd->draw (text);
		}

		sf::Vector2f mid1 = sf::Vector2f (sprite_.getPosition ().x + sprite_.getLocalBounds ().width / 2.0f, sprite_.getPosition ().y + sprite_.getLocalBounds ().height);

		for (auto p : person_->children_)
		{
			sf::Vector2f mid2 = sf::Vector2f (p->btnSprite_->getPosition ().x + p->btnSprite_->getLocalBounds ().width / 2.0f, p->btnSprite_->getPosition ().y);

			sf::Vector2f size = Vector2f (5, sqrtf ((mid2.y - mid1.y) * (mid2.y - mid1.y) + (mid2.x - mid1.x) * (mid2.x - mid1.x)));

			sf::RectangleShape shape (size);
			float angle = atan2f (mid1.x - mid2.x, mid2.y - mid1.y) * 180.0f / 3.1415926f;
			shape.setOrigin (0, 2.5f);
			shape.setRotation (angle);
			shape.setFillColor (Color::Red);
			shape.setPosition (mid1);

			wnd->draw (shape);
		}
	}

	PersonButton::~PersonButton ()
	{
		delete (addChildren_);
		delete (addParents_);
	}

	Sprite* PersonButton::GetSprite ()
	{
		return &sprite_;
	}

	AddPersonButton::AddPersonButton (Sprite sprite, WindowManager* manager, Sprite addBtnSprite, Sprite iconSprite, Sprite background, Sprite killBtnSprite, Font font) :
		SpriteWnd	   (sprite),
		manager_	   (manager),
		addBtnSprite_  (addBtnSprite),
		iconSprite_    (iconSprite),
		background_    (background),
		killBtnSprite_ (killBtnSprite),
		people_		   (),
		font_		   (font)
	{

	}

	AddPersonButton::~AddPersonButton ()
	{
		for (auto p : people_)
			delete (p);
	}

	bool AddPersonButton::OnClick (Event::MouseButtonEvent event)
	{
		Person* person = new Person ("", false, 0, nullptr);

		background_.setPosition (sf::Vector2f ());

		PersonButton* btn = new PersonButton (background_, addBtnSprite_, iconSprite_, killBtnSprite_, person, manager_, font_);
		manager_->AddWindow (btn);
		btn->IntegrateButtons ();
		person->btnSprite_ = btn->GetSprite ();

		people_.push_back (person);

		return true;
	}

	KillPersonButton::KillPersonButton (Sprite sprite, Person* person) :
		SpriteWnd (sprite),
		person_ (person)
	{
	}

	bool KillPersonButton::OnClick (Event::MouseButtonEvent event)
	{
		person_->dead_ = true;

		return true;
	}

	Sprite& KillPersonButton::GetSprite ()
	{
		return sprite_;
	}
}