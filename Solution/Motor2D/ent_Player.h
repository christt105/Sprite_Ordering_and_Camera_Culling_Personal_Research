#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "j1Entity.h"
#include "Animation.h"
#include "j1Collision.h"


struct SDL_Texture;

enum PlayerState {
	IDLE = 0,
	WALKING,

	UNKNOWN
};

class Player : public j1Entity
{	

public:

	Player(int x, int y, std::string name);

	~Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void AddColliders();
	void ChangeState(PlayerState);
	void SetCollidersPos();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);
	void LoadCollider(pugi::xml_node&);

private:

	PlayerState state = IDLE;

	Animation anim_walking;

	Animation anim_default;
};

#endif 
