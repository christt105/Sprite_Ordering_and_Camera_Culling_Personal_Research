#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;

enum PlayerState {
	IDLE = 0,
	WALKING,

	UNKNOWN
};

class Player : public j1Entity
{	

public:

	Player(int x, int y);

	~Player();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void AddColliders();
	void Move(float dt);
	void ChangeState(PlayerState);
	void SetCollidersPos();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);
	void LoadCollider(pugi::xml_node&);

private:

	PlayerState state = IDLE;

	float		maxSpeed;
	float		incrementSpeedX;

	bool		air = false;
	bool		platformOverstep = false;
	bool		death = false;
	
	Animation anim_walking;

	Animation anim_default;
};

#endif 
