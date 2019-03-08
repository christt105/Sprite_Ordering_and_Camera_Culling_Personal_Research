#include "ent_Static.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"

ent_Static::ent_Static(int x, int y, std::string name) :j1Entity(Types::STATIC, x, y, name)
{
	if (name == "tree") {
		type = ent_Static::Type::TREE;
	}
	else if (name == "statue") {
		type = ent_Static::Type::STATUE;
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
	default:
		LOG("Cannot find static object type");
		break;
	}

	size = iPoint(frame.w, frame.h);

	data.tileset.texture = App->tex->Load("maps/Pokemon_Objects.png");
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
