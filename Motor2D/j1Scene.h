#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& map);

	// Called before the first frame
	bool Start();

	void CreateEntities();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

private:

	uint win_width = 0;
	uint win_height = 0;
};

#endif // __j1SCENE_H__