#include "ent_Static.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Map.h"

ent_Static::ent_Static(int x, int y, std::string name) :j1Entity(Types::STATIC, x, y, name)
{
	if (name == "tree") {
		type = ent_Static::Type::TREE;
	}
	else if (name == "statue") {
		type = ent_Static::Type::STATUE;
	}


	//Isometric map
	else if (name == "pilar") {
		type = ent_Static::Type::PILAR;
	}
	else if (name == "corner") {
		type = ent_Static::Type::CORNER;
	}
	else if (name == "wall") {
		type = ent_Static::Type::WALL;
	}
	else if (name == "wall_l") {
		type = ent_Static::Type::WALL_L;
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}


	switch (type)
	{
	case ent_Static::Type::TREE:
		SetRect(16, 0, 32, 48);
		pivot = { 15, 36 };
		break;

	case ent_Static::Type::STATUE:
		SetRect(0, 48, 112, 160);
		pivot = { 60, 140 };
		break;


		//Isometric map
	case ent_Static::Type::PILAR:
		SetRect(192, 128, 64, 64);
		pivot = { 54,32 };
		break;
	case ent_Static::Type::CORNER:
		SetRect(128, 192, 64, 64);
		pivot = { 54,32 };
		break;
	case ent_Static::Type::WALL_L:
		SetRect(0, 192, 64, 64);
		pivot = { 54,32 };
		break;
	case ent_Static::Type::WALL:
		SetRect(64, 192, 64, 64);
		pivot = { 54,32 };
		break;

	case ent_Static::Type::UNKNOWN:
	default:
		LOG("Cannot find static object type");
		break;
	}

	size = iPoint(frame.w, frame.h);

	iPoint pos;
	pos = App->map->MapToWorld(pos.x, pos.y);
	pos = App->map->WorldToMap(position.x, position.y);
	

	position.create(pos.x, pos.y);

	data.tileset.texture = App->tex->Load("maps/Dungeon.png");
}

ent_Static::~ent_Static()
{
}

void ent_Static::Draw()
{
	App->render->Blit(data.tileset.texture, position.x, position.y, &frame);
}

void ent_Static::SetRect(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}
