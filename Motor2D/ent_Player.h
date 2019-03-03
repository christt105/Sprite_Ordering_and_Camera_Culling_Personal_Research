#ifndef __ENT_PLAYER_H__
#define __ENT_PLAYER_H__

#include "j1Entity.h"
#include "p2Point.h"
#include "Animation.h"
#include "p2List.h"
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
class Label;
class Image;


enum PlayerState {
	IDLE = 0,
	WALKING,
	JUMP,
	FALL,
	LAND,
	DEAD,
	HADOUKEN,
	PUNCH,
	IDLE_GHOST,
	WALKING_GHOST,
	JUMP_GHOST,
	FALL_GHOST,
	LAND_GHOST,
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

	bool Save(pugi::xml_node&) const;
	//bool Load(pugi::xml_node&);

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
	void Draw(float dt);

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
