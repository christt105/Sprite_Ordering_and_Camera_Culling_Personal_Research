#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "ent_Player.h"
#include "ent_Static.h"

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.assign("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;
	
	return ret;
}


bool j1EntityManager::Start()
{
	LOG("loading enemies");

	return true;
}

// Called before render is available
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntityManager", Profiler::Color::Red);

	return UpdateAll(dt);
}

bool j1EntityManager::UpdateAll(float dt)
{
	bool ret = true;
	
	for (std::vector<j1Entity*>::iterator item = entities.begin();item != entities.end(); ++item) {
		if (*item != nullptr) {
			(*item)->Move(dt);
			ret = (*item)->Update(dt);
			(*item)->Draw();
		}
	}
	
	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::Green);

	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	for (std::vector<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item)
	{
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			RELEASE(*item);
		}
	}
	entities.clear();

	return true;
}

j1Entity* j1EntityManager::CreateEntity(j1Entity::Types type, int PositionX, int PositionY, std::string name = "")
{
	static_assert(j1Entity::Types::UNKNOWN == (j1Entity::Types)2, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
		case j1Entity::Types::PLAYER: ret = new Player(PositionX, PositionY); break;
		case j1Entity::Types::STATIC: ret = new ent_Static(PositionX, PositionY, name); break;
	}
	if (ret != nullptr) {
		entities.push_back(ret);
		ret->Start();
	}
	
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity * entity)
{
	
	if (entity != nullptr) {
		entity->CleanUp();
		for (int i = 0; i < entities.size(); ++i) {
			if (entities[i] == entity) {
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}
}