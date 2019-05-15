#pragma once
#include "SFML\Graphics.hpp"
#include "..\Include\StandardCursor.h"
#include <vector>

class Noncopyable
{
public:
				  Noncopyable () {}
				  Noncopyable (const Noncopyable & that) = delete;
	Noncopyable & operator = (const Noncopyable & that) = delete;
};

namespace sf
{
	class AbstWnd
	{
	public:
		AbstWnd();
		virtual void				 Activate ()							   = 0;
		virtual void				 Deactivate ()							   = 0;
		virtual bool				 OnClick (Event::MouseButtonEvent event)   = 0;
		virtual bool				 OnKey (Event::KeyEvent event)			   = 0;
		virtual bool				 OnTextEntered (Event::TextEvent event)    = 0;
		virtual void				 Draw (RenderWindow * wnd)				   = 0;
		virtual bool				 IsHovered (Vector2f pos)				   = 0;
		virtual StandardCursor::TYPE GetCursorType ()						   = 0;
		virtual bool				 OnMouseMove (Event::MouseMoveEvent event) = 0;
	};

	class WindowManager : 
		public AbstWnd
	{
	public:
									 WindowManager ();
									 WindowManager (std::vector<AbstWnd*> windows);
									 ~WindowManager ();
		void						 Run (RenderWindow * wnd);
		virtual void				 Activate ()							   override;
		virtual void				 Deactivate ()							   override;
		virtual bool				 OnClick (Event::MouseButtonEvent event)   override;
		virtual bool				 OnKey (Event::KeyEvent event)			   override;
		virtual bool				 OnTextEntered (Event::TextEvent event)    override;
		virtual void				 Draw (RenderWindow * wnd)				   override;
		virtual bool				 IsHovered (Vector2f pos)				   override;
		virtual StandardCursor::TYPE GetCursorType ()						   override;
		virtual bool				 OnMouseMove (Event::MouseMoveEvent event) override;
	
	private:
		std::vector<AbstWnd*>		 windows_;
		AbstWnd *					 activeWindow; 
		bool						 isActive;
	};

	class RectWnd :
		public AbstWnd
	{
	public:
									 RectWnd ();
									 RectWnd (RectangleShape shape);
									 RectWnd (float x, float y, float width, float height, Color color);
									 ~RectWnd ();
		virtual void				 Activate ()							   override;
		virtual void				 Deactivate ()							   override;
		virtual bool				 OnClick (Event::MouseButtonEvent event)   override;
		virtual bool				 OnTextEntered (Event::TextEvent event)	   override;
		virtual void				 Draw (RenderWindow * wnd)				   override;
		virtual bool				 IsHovered (Vector2f pos)				   override;
		virtual StandardCursor::TYPE GetCursorType ()						   override;
		virtual bool				 OnKey (Event::KeyEvent event)			   override;
		virtual bool				 OnMouseMove (Event::MouseMoveEvent event) override;

	protected:
		sf::RectangleShape			 shape_;
		bool						 isActive;
	};

	template <class FN>
	class Button :
		public RectWnd,
		public NonCopyable
	{
	public:
		Button (RectangleShape shape, Text text, FN function) :
			AbstWnd(),
			text_(text),
			function_(function)
		{
			FloatRect textRect = text_.getLocalBounds ();
			text_.setOrigin (textRect.width / 2, textRect.height / 2);
			text_.setPosition (shape_.getPosition() + shape_.getSize() / 2);
		}

		Button (float x, float y, float width, float height, Color color, Text text, FN function) :
			RectWnd(x, y, width, height, color), 
			text_(text),
			function_(function)
		{
			FloatRect textRect = text_.getLocalBounds();
			text_.setOrigin(textRect.width / 2, textRect.height / 2);
			text_.setPosition(shape_.getPosition() + Vector2f(shape_.getSize().x / 2, shape_.getSize().y / 2));
		}

		~Button ()
		{
		}

		virtual bool OnClick (Event::MouseButtonEvent event) override
		{
			function_();
			return true;
		}

		virtual void Draw (RenderWindow * wnd)			     override
		{ 
			wnd->draw(shape_);
			wnd->draw(text_);
		}

	protected:
		Text text_;
		FN	 function_;
	};

	class TextWnd :
		public RectWnd, 
		public NonCopyable
	{
	public:
									 TextWnd (RectangleShape shape, Font font, String * string, Text text);
									 TextWnd (float x, float y, float width, float height, Color color, String * string, Text text);
									 ~TextWnd ();
		virtual bool				 OnClick (Event::MouseButtonEvent event) override;
		virtual void				 Draw (RenderWindow * wnd)				 override;
		virtual StandardCursor::TYPE GetCursorType ()						 override;
		virtual bool				 OnTextEntered (Event::TextEvent event)  override;
		virtual bool				 OnKey (Event::KeyEvent event)			 override;

	protected:
		sf::Text					 text_;
		sf::String *				 string_;
		bool						 wasActive;
		int							 cursorPos;
		sf::Clock					 timer;

	};

	class InstData
	{
	public:
					 InstData (const char * name, sf::Texture icon_pressed, sf::Texture icon_released);
		const char * name_;
		sf::Texture  icon_pressed_, icon_released_;
	};

	class AbstInst
	{
	public:
				     AbstInst (InstData instData);
		virtual void Apply   (Vector2f coords, float thickness, Color color, RenderTexture * texture) = 0;
		virtual void Preview (Vector2f coords, float thickness, Color color, RenderTarget * texture)  = 0;
		InstData	 GetInstData ();

	protected:
		InstData     instData_;
	};

	class Palette
	{
	public:
							   Palette ();
							   Palette (std::vector<AbstInst*> palette);		void				   AddInst (AbstInst * inst);
		void				   DelInst ();
		void				   SetActiveInst (int currInstPos);
		AbstInst *			   GetCurrInst ();
		std::vector<AbstInst*> GetPalette ();

	private:
		std::vector<AbstInst*> palette_;
		int					   currInstPos_;
	};

	class Canvas :
		public RectWnd,
		public Noncopyable
	{
	public:
						   Canvas (sf::Vector2f canvSize, sf::Vector2f pos, sf::Color color, float thickneess, Palette palette, sf::RenderWindow * window, sf::Texture * background = nullptr);
		void			   LoadTexture (sf::Texture texture);
		sf::Texture		   GetTexture ();
		virtual bool	   OnClick (Event::MouseButtonEvent event) override;
		bool			   OnClick ();
		virtual void	   Draw (RenderWindow * wnd)			   override;
		void			   Clear (sf::Color clearColor);
		void			   SetThickness (float thickness);
		void			   SetColor (Color color);
		virtual bool	   OnKey (Event::KeyEvent event)		   override;
		void			   SetActiveInst (int CurrInstPos);
	

	protected:
		sf::RenderWindow * window_;
		sf::RenderTexture  texture_;
		sf::Sprite		   shapeSprite_;
		sf::Color		   color_;
		float			   thickness_;
		Palette		       palette_;
	};

	class PaintInst :
		public AbstInst
	{
	public:
					 PaintInst ();
		virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture)		   		 override;
		virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override;
	};
	
	class PencilInst :
		public AbstInst
	{
	public:
					 PencilInst ();
		virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture)			     override;
		virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override;
	};

	class Spray :
		public AbstInst,
		public Noncopyable
	{
	public:
					 Spray (Shader * sprayShader);
		virtual void Apply (Vector2f coords, float thickness, Color color, RenderTexture * texture)			     override;
		virtual void Preview (sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override;

	private:
		Shader * sprayShader_;
	};
}


