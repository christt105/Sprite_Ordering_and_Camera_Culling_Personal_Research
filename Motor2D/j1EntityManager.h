#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "p2DynArray.h"

#define MAX_ENEMIES 15

struct EntityData
{
	j1Entity::Types type = j1Entity::Types::UNKNOWN;
	p2SString tsx_file;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();
	
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool UpdateAll(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();
	
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	j1Entity* CreateEntity(j1Entity::Types type, int PositionX, int PositionY);
	void DestroyEntity(j1Entity* entity);

public:

	p2DynArray<j1Entity*> entities;

private:

	bool do_logic = true;
	uint32 accumulated_time = 0;
	uint32 update_ms_cycle = 500;
	EntityData queue[MAX_ENEMIES];

};

#endif // __J1ENEMIES_H__
