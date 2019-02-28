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

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;

	// TSX of each enemy
	pugi::xml_node node = conf.child("entity");
	for (int i = 0; node; node = node.next_sibling()) {
		queue[i].tsx_file.create(node.attribute("file").as_string());
		++i;
	}
	
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
	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;
	UpdateAll(dt, do_logic);
	if (do_logic == true) {
		accumulated_time = update_ms_cycle - accumulated_time;
		do_logic = false;
	}
	return true;
}

bool j1EntityManager::UpdateAll(float dt, bool do_logic)
{
	bool ret = true;

	for (int i = 0; i < entities.Count(); ++i) {
		if (entities[i] != nullptr) {
			entities[i]->Move(dt);
			ret = entities[i]->Update(dt);
			entities[i]->Draw();
			if (do_logic) {
				entities[i]->CreatePath();
			}
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

	for (int i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr) {
			entities[i]->CleanUp();
			RELEASE(entities[i]);
		}
	}
	entities.Clear();

	return true;
}

bool j1EntityManager::Save(pugi::xml_node & node) const
{
	bool ret = false;
	LOG("Saving all entities...");

	for (int i = 0; i < entities.Count(); ++i) {
		if (entities[i] != nullptr)
			ret = entities[i]->Save(node);
	}
	return ret;
}

bool j1EntityManager::Load(pugi::xml_node & node)
{
	bool ret = false;
	LOG("Loading all entities...");

	ret = CleanUp();

	return ret;
}

j1Entity* j1EntityManager::CreateEntity(j1Entity::Types type, int PositionX, int PositionY)
{
	static_assert(j1Entity::Types::UNKNOWN == (j1Entity::Types)4, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
		case j1Entity::Types::PLAYER: ret = new Player(PositionX, PositionY); break;
	}
	if (ret != nullptr) {
		entities.PushBack(ret);
		ret->Start();
	}
	
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity * entity)
{
	
	if (entity != nullptr) {
		entity->CleanUp();
		for (int i = 0; i < entities.Count(); ++i) {
			if (entities[i] == entity) {
				delete entities[i];
				entities[i] = nullptr;
			}
		}
	}
}