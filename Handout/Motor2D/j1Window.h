#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"

struct SDL_Window;
struct SDL_Surface;

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Change title
	void SetNewTitle(const char* new_title);
	void AddStringToTitle(const char* add_title);
	void ClearTitle();

	// Retrive window size
	uint GetWindowWidth() const;
	uint GetWindowHeight() const;

	// Retrieve window scale
	uint GetScale() const;

	void SetScale(int scale);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	bool fullscreen;

private:
	std::string	title;
	uint		width;
	uint		height;
	uint		scale;
};

#endif // __j1WINDOW_H__