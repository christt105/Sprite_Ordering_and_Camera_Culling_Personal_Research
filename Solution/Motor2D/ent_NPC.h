#ifndef __ENT_NPC_H__
#define __ENT_NPC_H__

#include "j1Entity.h"
#include "Animation.h"


struct SDL_Texture;

class ent_NPC : public j1Entity
{

public:

	ent_NPC(int x, int y, std::string name);

	~ent_NPC();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*, float dt);

private:

	void PushBack();
	void IdAnimToEnum();
	void LoadProperties(pugi::xml_node&);

private:

	EntityState state = EntityState::IDLE;

};

#endif 
