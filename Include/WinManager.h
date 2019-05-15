
#pragma once
#include "SFML\Graphics.hpp"
#include "..\Include\StandardCursor.h"
#include <vector>
#include <cassert>
#include <stdio.h>
#include "TagTree.h"
#include <iostream>


class Noncopyable
{
public:
	Noncopyable() {}
	Noncopyable(const Noncopyable & that) = delete;
	Noncopyable & operator = (const Noncopyable & that) = delete;
};

namespace sf
{
	class AbstWnd
	{
	public:
		AbstWnd();
		virtual void				 Activate() = 0;
		virtual void				 Deactivate() = 0;
		virtual bool				 OnClick(Event::MouseButtonEvent event) = 0;
		virtual bool		         OnRelease (Event::MouseButtonEvent event) = 0;
		virtual bool				 OnKey(Event::KeyEvent event) = 0;
		virtual bool				 OnTextEntered(Event::TextEvent event) = 0;
		virtual void				 Draw(RenderWindow * wnd) = 0;
		virtual bool				 IsHovered(Vector2f pos) = 0;
		virtual StandardCursor::TYPE GetCursorType() = 0;
		virtual bool				 OnMouseMove(Event::MouseMoveEvent event) = 0;
	};

	class WindowManager :
		public AbstWnd
	{
	public:
		WindowManager();
		WindowManager(std::vector<AbstWnd*> windows);
		~WindowManager();
		void						 Run(RenderWindow * wnd);
		virtual void				 Activate()							   override;
		virtual void				 Deactivate()							   override;
		virtual bool				 OnClick(Event::MouseButtonEvent event)   override;
		virtual bool				 OnKey(Event::KeyEvent event)			   override;
		virtual bool				 OnTextEntered(Event::TextEvent event)    override;
		virtual void				 Draw(RenderWindow * wnd)				   override;
		virtual bool				 IsHovered(Vector2f pos)				   override;
		virtual StandardCursor::TYPE GetCursorType()						   override;
		virtual bool				 OnMouseMove(Event::MouseMoveEvent event) override;
		void                         AddWindow(AbstWnd * window);
		void                         DeleteWindow ();
		virtual bool		         OnRelease (Event::MouseButtonEvent event) override;

	private:
		std::vector<AbstWnd*>		 windows_;
		AbstWnd *					 activeWindow;
		bool						 isActive;
	};

	class RectWnd :
		public AbstWnd
	{
	public:
		RectWnd();
		RectWnd(RectangleShape shape);
		RectWnd(float x, float y, float width, float height, Color color);
		~RectWnd();
		virtual void				 Activate()								   override;
		virtual void				 Deactivate()							   override;
		virtual bool				 OnClick(Event::MouseButtonEvent event)    override;
		virtual bool				 OnTextEntered(Event::TextEvent event)	   override;
		virtual void				 Draw(RenderWindow * wnd)				   override;
		virtual bool				 IsHovered(Vector2f pos)				   override;
		virtual StandardCursor::TYPE GetCursorType()						   override;
		virtual bool				 OnKey(Event::KeyEvent event)			   override;
		virtual bool				 OnMouseMove (Event::MouseMoveEvent event) override;
		virtual bool                 OnRelease (Event::MouseButtonEvent event) override;

	protected:
		sf::RectangleShape			 shape_;
		bool						 isActive; 
		bool                         mouseHold;
	};

	class SpriteWnd :
		public RectWnd
	{
	public:
		SpriteWnd();
		SpriteWnd(Sprite sprite);
		SpriteWnd (float x, float y, float width, float height, Texture textureMap);
		virtual void Draw(RenderWindow * wnd) override;
		virtual bool IsHovered(Vector2f pos) override;

	protected:
		Sprite sprite_;
		Texture texture_;
	};

	template <class FN>
	class Button :
		public RectWnd,
		public NonCopyable
	{
	public:
		Button(RectangleShape shape, Text text, FN function) :
			AbstWnd(),
			text_(text),
			function_(function)
		{
			FloatRect textRect = text_.getLocalBounds ();
			text_.setOrigin (textRect.width / 2, textRect.height / 2);
			text_.setPosition (shape_.getPosition () + Vector2f(shape_.getSize ().x / 2, shape_.getSize ().y / 2));
		}

		Button(float x, float y, float width, float height, Color color, Text text, FN function) :
			RectWnd(x, y, width, height, color),
			text_(text),
			function_(function)
		{
			FloatRect textRect = text_.getLocalBounds();
			text_.setOrigin(textRect.width / 2, textRect.height / 2);
			text_.setPosition(shape_.getPosition() + Vector2f(shape_.getSize().x / 2, shape_.getSize().y / 2));
		}

		~Button()
		{
		}

		virtual bool OnClick(Event::MouseButtonEvent event) override
		{
			function_();
			return true;
		}

		virtual void Draw(RenderWindow * wnd)			     override
		{
			RectWnd::Draw (wnd);
			wnd->draw(text_);
		}

		void setColor (sf::Color color)
		{
			shape_.setFillColor (color);
		}

		void setTextColor (sf::Color color)
		{
			text_.setFillColor (color);
		}

	protected:
		Text text_;
		FN	 function_;
	};

	class TextWnd :
		public SpriteWnd,
		public NonCopyable
	{
	public:
		TextWnd(Sprite sprite, Font font, String * string, Text text);
		TextWnd(float x, float y, float width, float height, Texture texture, String * string, Text text);
		~TextWnd();
		virtual bool				 OnClick(Event::MouseButtonEvent event) override;
		virtual void				 Draw(RenderWindow * wnd)				 override;
		virtual StandardCursor::TYPE GetCursorType()						 override;
		virtual bool				 OnTextEntered(Event::TextEvent event)  override;
		virtual bool				 OnKey(Event::KeyEvent event)			 override;
		void						 Updatetext ();

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
		InstData(const char * name, sf::Texture * icon_pressed, sf::Texture * icon_released);
		const char * name_;
		sf::Texture * icon_pressed_, * icon_released_;
	};

	class AbstInst
	{
	public:
		AbstInst(InstData instData);
		virtual void Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture) = 0;
		virtual void Preview(Vector2f coords, float thickness, Color color, RenderTarget * texture) = 0;
		virtual void LoadWindows (sf::WindowManager * manager);
		virtual void UnloadWindows (sf::WindowManager * manager);
		virtual void ReadXML (FILE * file);
		virtual void WriteXML (FILE * file);
		InstData	 GetInstData();

	protected:
		InstData     instData_;
	};

	class Palette
	{
	public:
		Palette();
		Palette( sf::WindowManager * manager);
		void				   AddInst(AbstInst * inst);
		void				   DelInst();
		void				   SetActiveInst(int currInstPos);
		AbstInst *			   GetCurrInst();
		int                    GetCurrInstPos();
		std::vector<AbstInst*> & GetPalette();

	private:
		std::vector<AbstInst*> palette_;
		int					   currInstPos_;
		WindowManager *		   manager_;
	};

	class Canvas :
		public RectWnd,
		public Noncopyable
	{
	public:
						    Canvas (sf::Vector2f canvSize, sf::Vector2f pos, sf::Color color, float thickneess, Palette * palette, sf::RenderWindow * window, sf::Texture * background = nullptr);
		void			    LoadTexture (sf::Texture texture);
		sf::Texture		    GetTexture ();
		sf::RenderTexture * GetRenderTexture ();
		void                SetTexture (Texture texture);
		virtual bool	    OnClick (Event::MouseButtonEvent event) override;
		virtual bool	    OnClick ();
		virtual void	    Draw (RenderWindow * wnd)			   override;
		void			    Clear (sf::Color clearColor);
		void			    SetThickness (float thickness);
		void			    SetColor (Color color);
		virtual bool	    OnKey (Event::KeyEvent event)		   override;
		void			    SetActiveInst (int CurrInstPos, sf::WindowManager * manager);
		sf::Color           GetColor ();
		float               GetThickness ();
		bool                ReadXML (FILE * file);
		bool                WriteXML (FILE * file);
		virtual bool        OnRelease (Event::MouseButtonEvent event) override;
		virtual bool		OnMouseMove (Event::MouseMoveEvent event) override;
		void                Cancel ();

	protected:
		sf::RenderWindow *  window_;
		sf::RenderTexture   texture_;
		sf::Sprite		    shapeSprite_;
		sf::Color		    color_;
		float			    thickness_;
		Palette	*	        palette_;
		TagTree             settings;
		sf::Keyboard::Key   prevKey;
		std::vector<sf::Image> history;
	};

	class DllLoader :
		public Noncopyable
	{
	public:
		DllLoader(Palette * palette1);
		~DllLoader();

		void	  loadFromFolder(const char * path);
		void      loadFromFile(const char * path);

	private:
		Palette * palette_;
	};

	class Spray :
		public AbstInst,
		public Noncopyable
	{
	public:
		Spray(Shader * sprayShader);
		virtual void Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture)			     override;
		virtual void Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override;

	private:
		Shader *     sprayShader_;
	};

	class Brush :
		public AbstInst
	{
	public:
		Brush(Texture brush);
		virtual void Apply(Vector2f coords, float thickness, Color color, RenderTexture * texture)			     override;
		virtual void Preview(sf::Vector2f coords, float thickness, sf::Color color, sf::RenderTarget * texture) override;

	private:
		Texture brush_;
	};

	class TextureSampler
	{
	public:
		TextureSampler(Texture texturePack_);
		void    Sample(Texture * texture);

	protected:
		Texture texturePack_;
	};

	class InstrumentButton :
		public SpriteWnd
	{
	public:
		InstrumentButton(Sprite sprite, Palette * palette, int instNum);
		virtual bool OnClick(Event::MouseButtonEvent) override;

	protected:
		Palette *	 palette_;
		const int	 INST_NUM;


	};

	class PaletteWnd :
		public SpriteWnd
	{
	public:
		PaletteWnd (Sprite sprite, Palette * palette);
		void IntegrateButtons(WindowManager * manager, Texture textureMap);
		virtual StandardCursor::TYPE GetCursorType() override;
		virtual bool OnClick(Event::MouseButtonEvent event) override;
		~PaletteWnd ();

	private:
		Palette * palette_;
		std::vector<InstrumentButton*> buttons_;
		std::vector<Texture*> textures_;
	};

	class ColorButton :
		public RectWnd
	{
	public:
		ColorButton (Vector2f pos, Vector2f size, Color color, Canvas * canvas);
		virtual bool OnClick (Event::MouseButtonEvent event) override;

	protected:
		Canvas * canvas_;
	};

	class ColorPalette :
		public SpriteWnd
	{
	public:
		ColorPalette (Sprite sprite, std::vector<Color> colors, Canvas * canvas);
		void IntegrateButtons (WindowManager *  manager);
		virtual StandardCursor::TYPE GetCursorType() override;
		virtual bool OnClick(Event::MouseButtonEvent event) override;
		~ColorPalette ();

	protected:
		std::vector<Color> colors_;
		Canvas * canvas_;
		std::vector<ColorButton*> buttons_;
	};

	template <class FN>
	class SpriteButton :
		public SpriteWnd
	{
	public:
		SpriteButton (Sprite sprite, Text text, FN function) :
			SpriteWnd (sprite),
			text_ (text),
			function_ (function)
		{
			FloatRect textRect = text_.getLocalBounds ();
			text_.setOrigin (textRect.width / 2, textRect.height / 2);
			text_.setPosition (shape_.getPosition () + Vector2f (shape_.getSize ().x / 2, shape_.getSize().y / 2) );
		}

		SpriteButton (float x, float y, float width, float height, Texture textureMap, Text text, FN function) :
			SpriteWnd(x, y, width, height, textureMap),
			text_ (text),
			function_ (function)
		{
			FloatRect textRect = text_.getLocalBounds ();
			text_.setOrigin (textRect.width / 2, textRect.height / 2);
			text_.setPosition (sprite_.getPosition () + Vector2f (width / 2, height / 2));
		}

		virtual void Draw (RenderWindow * wnd) override
		{
			SpriteWnd::Draw (wnd);
			wnd->draw (text_);
		}

		virtual bool OnClick (Event::MouseButtonEvent event) override
		{
			function_ ();
			return true;
		}

		void setTextColor (sf::Color color)
		{
			text_.setFillColor (color);
		}

	private:
		Text text_;
		FN function_;
	};

	template <class T>
	class SliderWnd :
		public RectWnd
	{
	public:
		SliderWnd (float x, float y, float width, float height, Color sliderPathColor, Color sliderColor, T minValue, T maxValue, T value) :
			RectWnd (x, y, width, height, Color::Transparent),
			value_(value),
			minValue_(minValue),
			maxValue_(maxValue),
			sliderPath_(Vector2f(width, 10.0f)),
			slider_(Vector2f(10.0f, height))
		{
			sliderPath_.setOrigin (sliderPath_.getSize () / 2.0f);
			sliderPath_.setPosition (shape_.getPosition () + shape_.getSize () / 2.0f);
			sliderPath_.setFillColor (sliderPathColor);

			slider_.setOrigin (slider_.getSize () / 2.0f);
			assert (maxValue_ - minValue_);
			slider_.setPosition (Vector2f (shape_.getPosition ().x + (value_ / (maxValue_ - minValue_)) * shape_.getSize ().x, shape_.getPosition ().y + shape_.getSize ().y / 2));
			slider_.setFillColor (sliderColor);
		}

		virtual bool OnClick (Event::MouseButtonEvent event) override
		{
			T range = maxValue_ - minValue_;
			value_ = range * (event.x - shape_.getPosition ().x) / shape_.getSize ().x;
			slider_.setPosition (Vector2f (event.x, slider_.getPosition ().y));
			mouseHold = true;

			return true;
		}

		virtual bool OnMouseMove (Event::MouseMoveEvent event) override
		{
			if (mouseHold)
			{
				T range = maxValue_ - minValue_;
				value_ = range * (event.x - shape_.getPosition ().x) / shape_.getSize ().x;
				slider_.setPosition (Vector2f (event.x, slider_.getPosition ().y)); 
				return true;
			}

			return false;
		}

		virtual bool OnRelease (Event::MouseButtonEvent event) override
		{
			if (mouseHold)
			{
				mouseHold = false;
				return true;
			}
			return false;
		}

		virtual void Draw (RenderWindow * wnd) override
		{
			RectWnd::Draw (wnd);
			wnd->draw (sliderPath_);
			wnd->draw (slider_);
		}

		T getValue ()
		{
			return value_; 
		}

		void setValue (T value)
		{
			value_ = value;
			slider_.setPosition (shape_.getPosition () + Vector2f (value_ / (maxValue_ - minValue_) * shape_.getSize ().x, shape_.getSize ().y / 2));
		}

	protected:
		T value_, minValue_, maxValue_;
		RectangleShape sliderPath_, slider_;
	};

	class AbstFilter
	{
	public:
		AbstFilter ();
		virtual void Apply (RenderTexture * texture, float value) = 0;

	};

	class BrightnessFilter :
		public AbstFilter
	{
	public:
		BrightnessFilter ();
		virtual void Apply (RenderTexture * texture, float value) override;
	};

	class ContrastFilter :
		public AbstFilter
	{
	public:
		ContrastFilter ();
		virtual void Apply (RenderTexture * texture, float value) override;
	};

	class SlidingWnd :
		public RectWnd
	{
	public:
		SlidingWnd (float x, float y, float size, Color fill);
		virtual bool OnClick (Event::MouseButtonEvent event) override;
		virtual bool OnRelease (Event::MouseButtonEvent event) override;
		virtual bool OnMouseMove (Event::MouseMoveEvent event) override;
		Vector2 <float> getPos ();

	protected:
		Vector2f holdPoint;
	};

	class Graph :
		public RectWnd,
		public Noncopyable
	{
	public:
		Graph (float x, float y, WindowManager * manager, float width = 256.0f, float height = 256.0f, Color Background = Color::Black);
		~Graph ();
		void insertPoint (float x, float y, float size, Color color);
		void insertPoint (float x, float y, float size, unsigned position, Color color);
		virtual void Draw (RenderWindow * wnd) override;
		virtual bool OnClick (Event::MouseButtonEvent event) override;
		void removePoints (WindowManager * Manager);
		float * getGraph ();
		
	protected:;
		std::vector <SlidingWnd*> keyPoints;
		WindowManager * manager_;
		float graph[256];
		
		template <typename T>
		void ROR (T & arr, unsigned a, unsigned b)
		{
			auto buff = arr[b];
			for (unsigned i = b; i > a; i--)
				arr[i] = arr[i - 1];
			arr[a] = buff;
		}
		
		double CatmullRom (double* y, double dx, double t)
		{
			double m0 = (y[1] - y[-1]) / 2,
				m1 = (y[2] - y[0]) / 2;

			double t2 = t * t,
				t3 = t2 * t,
				t3_2 = t3 * 2,
				t2_3 = t2 * 3;

			double hermite00 = t3_2 - t2_3 + 1;
			double hermite10 = t3 - t2 * 2 + t;
			double hermite01 = -t3_2 + t2_3;
			double hermite11 = t3 - t2;

			return hermite00 * y[0] + hermite10 * m0 * dx +
				hermite01 * y[1] + hermite11 * m1 * dx;
		}

	};
}
