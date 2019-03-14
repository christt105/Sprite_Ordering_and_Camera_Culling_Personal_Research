#ifndef __ENT_STATIC_H__
#define __ENT_STATIC_H__

#include "j1Entity.h"


struct SDL_Texture;

class ent_Static : public j1Entity
{

public:

	enum class Type {
		TREE,
		STATUE,
		POST,
		HOUSE,
		CENTER,

		PILAR,
		CORNER,
		WALL_UP_RIGHT,
		WALL_UP_LEFT,
		WALL_DOWN_RIGHT,
		WALL_DOWN_LEFT,
		DOOR,

		UNKNOWN
	};

public:

	ent_Static(int x, int y, std::string name);

	~ent_Static();

	//void OnCollision(Collider*, Collider*, float dt);

private:

	void Draw();
	void SetRect(int x, int y, int w, int h);

private:
	SDL_Rect frame;
	Type type;
};

#endif