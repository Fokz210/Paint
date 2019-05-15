#pragma once
#include "../include/WinManager.h"
#include "Person.h"

namespace sf
{
	enum relatives
	{
		parents,
		children
	};

	class AddPersonButton :
		public SpriteWnd
	{
	public:
									 AddPersonButton (Sprite sprite, Sprite PersonBtnSprite, Sprite iconSprite, Person * person, WindowManager * manager, relatives current);
		virtual	StandardCursor::TYPE GetCursorType   ()								 override;
		virtual bool				 OnClick		 (Event::MouseButtonEvent event) override;
		Sprite	&					 getSprite ();

	protected:
		Sprite						 PersonBtnSprite_, iconSprite_;
		Person *					 person_;
		WindowManager *				 manager_;
		relatives                    currentRelatives_;
	};

	class PersonButton :
		public SpriteWnd
	{
	public:
						  PersonButton	   (Sprite sprite, Sprite addBtnSprite, Sprite iconSprite, Person * person, WindowManager * manager);
		void			  IntegrateButtons ();
		bool			  OnClick		   (Event::MouseButtonEvent event) override;
		bool              OnRelease		   (Event::MouseButtonEvent event) override;
		void		      Draw			   (RenderWindow* wnd)			   override;
						  ~PersonButton	   ();
		Sprite *		  GetSprite		   ();

	protected:
		AddPersonButton * addParents_, * addChildren_;
		Sprite			  icon_, addBtnSprite_;
		Person *		  person_;
		WindowManager*	  manager_;
		bool			  holding_;
		sf::Vector2f	  msdelta_;
	};

}


