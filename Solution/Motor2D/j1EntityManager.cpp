#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "ent_Player.h"
#include "ent_Static.h"
#include "ent_NPC.h"
#include "j1Window.h"
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

bool j1EntityManager::UpdateAll(float dt)
{
	bool ret = true;

	//TODO 4: Save entities on camera during update iteration in draw_entities vector and iterate after update iteration
	//move draw function, entities_drawn++ and DrawDebugQuad to the new iteration

	std::vector<j1Entity*> draw_entities;
	uint entities_drawn = 0;

	for (std::vector<j1Entity*>::iterator item = entities.begin();item != entities.end(); ++item) {
		if (*item != nullptr) {
			ret = (*item)->Update(dt);
			
			if (App->render->IsOnCamera((*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y)) {
				draw_entities.push_back(*item);
			}
		}
	}

	// TODO 5: Use std::sort(Iterator first, Iterator last, Compare comp) before iterate draw_entities. Sort only entities in draw_entities vector. For function to compare you can use j1EntityManager::SortByYPos
	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityManager::SortByYPos);

	for (std::vector<j1Entity*>::iterator item = draw_entities.begin(); item != draw_entities.end(); ++item) {
		(*item)->Draw();
		entities_drawn++;

		if (App->scene->entities_box) {
			DrawDebugQuad(*item);
		}
	}

	draw_entities.clear();
	
	static char title[30];
	sprintf_s(title, 30, " | Entities drawn: %u", entities_drawn);
	
	App->win->AddStringToTitle(title);

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
	static_assert(j1Entity::Types::UNKNOWN == (j1Entity::Types)3, "code needs update");
	j1Entity* ret = nullptr;
	switch (type) {
	case j1Entity::Types::PLAYER: ret = new Player(PositionX, PositionY, name); break;
	case j1Entity::Types::STATIC: ret = new ent_Static(PositionX, PositionY, name); break;
	case j1Entity::Types::NPC: ret = new ent_NPC(PositionX, PositionY, name); break;
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
		for (std::vector<j1Entity*>::iterator i = entities.begin(); i != entities.end(); ++i) {
			if (*i == entity) {
				delete *i;
				*i = nullptr;
			}
		}
	}
}

void j1EntityManager::DrawDebugQuad(j1Entity *entity) //Draw rectangle occupied by the entity
{
	SDL_Rect section = { entity->position.x, entity->position.y, entity->size.x, entity->size.y };
	Uint8 alpha = 80;

	switch (entity->type) {
	case j1Entity::Types::PLAYER:
		App->render->DrawQuad(section, 255, 0, 0, alpha);
		break;
	case j1Entity::Types::NPC:
		App->render->DrawQuad(section, 0, 255, 0, alpha);
		break;

	case j1Entity::Types::STATIC:
		if (entity->name == "tree")
			App->render->DrawQuad(section, 0, 255, 255, alpha);
		else if (entity->name == "statue")
			App->render->DrawQuad(section, 0, 0, 255, alpha);
		else if(entity->name == "pilar")
			App->render->DrawQuad(section, 0, 255, 0, alpha);
		else if (entity->name == "corner" || entity->name == "door")
			App->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_d_l")
			App->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_d_r")
			App->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_u_l")
			App->render->DrawQuad(section, 255, 255, 0, alpha);
		else if (entity->name == "wall_u_r")
			App->render->DrawQuad(section, 255, 255, 0, alpha);
		else {
			App->render->DrawQuad(section, 255, 255, 255, alpha);
		}
		break;
	default:
		App->render->DrawQuad(section, 255, 255, 255, alpha);
		break;
	}

	App->render->DrawCircle(section.x + entity->pivot.x, section.y + entity->pivot.y, 3, 0, 255, 0);
}

//TODO 6: add the pivot position to general position
bool j1EntityManager::SortByYPos(const j1Entity * ent1, const j1Entity * ent2)
{
	return ent1->pivot.y + ent1->position.y < ent2->pivot.y + ent2->position.y;
}
