#include "PersonButton.h"

namespace sf
{

	AddPersonButton::AddPersonButton (Sprite sprite, Sprite PersonBtnSprite, Sprite iconSprite, Person * person, WindowManager * manager, relatives currrent) :
		SpriteWnd		  (sprite),
		person_			  (person),
		PersonBtnSprite_  (PersonBtnSprite),
		iconSprite_		  (iconSprite),
		manager_		  (manager),
		currentRelatives_ (currrent)
	{
	}

	StandardCursor::TYPE sf::AddPersonButton::GetCursorType ()
	{
		return StandardCursor::TYPE::HAND;
	}

	bool AddPersonButton::OnClick (Event::MouseButtonEvent event)
	{
		Person* person = new Person ("", false, 1, nullptr);

		switch (currentRelatives_)
		{
		case parents:
			person->children_.push_back (person_);
			person_->parents_.push_back (person);
			PersonBtnSprite_.setPosition (sf::Vector2f (sprite_.getPosition ().x, sprite_.getPosition ().y - 110));
			break;

		case children:
			person->parents_.push_back (person_);
			person_->children_.push_back (person);
			PersonBtnSprite_.setPosition (sf::Vector2f (sprite_.getPosition ().x, sprite_.getPosition ().y + 40));
			break;

		default:
			break;
		}

		PersonButton* btn = new PersonButton (PersonBtnSprite_, sprite_, iconSprite_, person, manager_);
		manager_->AddWindow (btn);
		btn->IntegrateButtons ();
		person->btnSprite_ = btn->GetSprite ();

		return true;
	}


	Sprite & AddPersonButton::getSprite ()
	{
		return sprite_;
	}

	PersonButton::PersonButton (Sprite sprite, Sprite addBtnSprite, Sprite iconSprite, Person* person, WindowManager * manager) :
		SpriteWnd	 (sprite),
		person_		 (person),
		addChildren_ (nullptr),
		addParents_  (nullptr),
		icon_		 (iconSprite),
		manager_	 (manager),
		holding_	 (false),
		msdelta_	 ()
	{
		addBtnSprite.setPosition (sprite.getPosition ());
		addParents_ = new AddPersonButton (addBtnSprite, sprite, iconSprite, person, manager, parents);

		addBtnSprite.setPosition (Vector2f (sprite.getPosition ().x, sprite.getPosition ().y + sprite.getLocalBounds ().height - addBtnSprite.getLocalBounds ().height));
		addChildren_ = new AddPersonButton (addBtnSprite, sprite, iconSprite, person, manager, children);

		icon_.setPosition (Vector2f (sprite.getPosition ().x, sprite.getPosition ().y + addBtnSprite.getLocalBounds ().height));
	}

	void PersonButton::IntegrateButtons ()
	{
		manager_->AddWindow (addParents_);
		manager_->AddWindow (addChildren_);
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
		}

		SpriteWnd::Draw (wnd);

		wnd->draw (icon_);

		sf::Vector2f mid1 = sf::Vector2f (sprite_.getPosition ().x + sprite_.getLocalBounds ().width / 2.0f, sprite_.getPosition ().y + sprite_.getLocalBounds ().height / 2);

		for (auto p : person_->children_)
		{
			sf::Vector2f mid2 = sf::Vector2f (p->btnSprite_->getPosition ().x + p->btnSprite_->getLocalBounds ().width / 2.0f, p->btnSprite_->getPosition ().y + p->btnSprite_->getLocalBounds ().height / 2);

			sf::Vertex line[] = 
			{
				sf::Vertex (mid1),
				sf::Vertex (mid2)
			};

			wnd->draw (line, 2, sf::Lines);
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
}