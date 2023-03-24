#pragma once 

#include <iomanip>

#include <SDL_TTF.h>

#include "Writer.h"
#include "Engine.h"

using namespace std;

/** A struct that has pointer of type rect and texture being
	later used to draw an object on the screen.

	The point of this struct is to minimize the use of classes
	in the client
*/

class Presenter
{
public:
	static SDL_Window* m_main_window;
	static SDL_Renderer* m_main_renderer;
	
	static unsigned int m_SCREEN_WIDTH;
	static unsigned int m_SCREEN_HEIGHT;

	void init();
	void update();
	void draw();

	SDL_Renderer* getRenderer();
	Writer* getWriter();

	static void drawObject(SDL_Texture* texture);
	static void drawObject(Drawable& drawable);
	static void drawObject(DrawableWithOpacity& drawable);
	
	void drawLine(/*line ln, */ float2 start, float2 finish);

private:
	int2 screenCenter;

	Writer* m_writer;

	void improveRenderer();
};

namespace
{
	template <typename T>
	void drawObject(T& t)
	{
		Presenter::drawObject(t);
	}

	SDL_Texture* loadTexture(string path)
	{
		return LoadTexture(path, Presenter::m_main_renderer);
	}
}