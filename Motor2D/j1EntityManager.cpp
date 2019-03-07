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
#include <algorithm>

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

bool SortByYPos(const j1Entity * ent1, const j1Entity * ent2)
{
	return ent1->position.y < ent2->position.y;
}

bool j1EntityManager::UpdateAll(float dt)
{
	bool ret = true;
	
	
	for (std::vector<j1Entity*>::iterator item = entities.begin();item != entities.end(); ++item) {
		if (*item != nullptr) {
			(*item)->Move(dt);
			ret = (*item)->Update(dt);
		}
	}
	

	std::sort(entities.begin(), entities.end(), SortByYPos);

	for (std::vector<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) {
		SDL_Rect r = { (int)(*item)->position.x, (int)(*item)->position.y, (*item)->size.x, (*item)->size.y };
		if (*item != nullptr && App->render->IsInCamera(r)) {
			(*item)->Draw();
			if (App->scene->entities_box) {
				DrawDebugQuad(*item);
			}
			LOG("Drawing: %s\nRect: x(%i) y(%i) w(%i) h(%i)", (*item)->name.data(), r.x, r.y, r.w, r.h);
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

j1Entity* j1EntityManager::CreateEntity(j1Entity::Types type, int PositionX, int PositionY, std::string name)
{
	static_assert(j1Entity::Types::UNKNOWN == (j1Entity::Types)2, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
		case j1Entity::Types::PLAYER: ret = new Player(PositionX, PositionY, name); break;
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

void j1EntityManager::DrawDebugQuad(j1Entity *entity)
{
	SDL_Rect section = { entity->position.x, entity->position.y, entity->size.x, entity->size.y };
	Uint8 alpha = 80;

	switch (entity->type) {
	case j1Entity::Types::PLAYER:
		App->render->DrawQuad(section, 255, 0, 0, alpha);
		break;

	case j1Entity::Types::STATIC:
		if (entity->name == "tree")
			App->render->DrawQuad(section, 0, 255, 255, alpha);
		else if (entity->name == "statue")
			App->render->DrawQuad(section, 0, 0, 255, alpha);
		break;
	}
}
