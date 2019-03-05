#ifndef __ENT_STATIC_H__
#define __ENT_STATIC_H__

#include "j1Entity.h"
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;

class ent_Static : public j1Entity
{

public:

	enum class Type {
		TREE,
		STATUE,

		UNKNOWN
	};

public:

	ent_Static(int x, int y, p2SString name);

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