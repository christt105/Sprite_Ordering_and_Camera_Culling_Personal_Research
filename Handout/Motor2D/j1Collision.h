#ifndef __J1COLLISION_H__
#define __J1COLLISION_H__

#include "j1Module.h"
#include <vector>

class j1Entity;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_FLOOR,
	COLLIDER_SCENE,
	COLLIDER_PLAYER,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Entity* callback = nullptr;
	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Entity* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	bool CheckCollision(const SDL_Rect& r) const;
};
class j1Colliders : public j1Module
{
public:
	j1Colliders();
	~j1Colliders();
	bool Awake(pugi::xml_node&);
	bool PreUpdate();
	bool Update(float dt);
	//update_status PostUpdate();
	bool CleanUp();
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity * callback = nullptr);
	bool EraseAllCollider(Collider* collider);
	bool EraseMapCollider();
	void DebugDraw();
	bool checkColisionList(Collider* enemCollider);
	bool Check(Collider* c1, COLLIDER_TYPE type);

	bool debug = false;

private:
	std::vector<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};
#endif // __J1COLLIDERS_H__ 
