#include "ent_Static.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Map.h"

ent_Static::ent_Static(int x, int y, std::string name) :j1Entity(Types::STATIC, x, y, name)
{
	//assign type of static entity, texture rect and pivot
	//Orthogonal map ------------------------
	if (name == "tree") {
		type = ent_Static::Type::TREE;
		SetRect(16, 0, 32, 48);
		SetPivot(15, 36);
	}
	else if (name == "statue") {
		type = ent_Static::Type::STATUE;
		SetRect(0, 48, 112, 160);
		SetPivot(60, 140);
	}
	else if (name == "house") {
		type = ent_Static::Type::HOUSE;
		SetRect(128, 0, 80, 96);
		SetPivot(40, 94);
	}
	else if (name == "center") {
		type = ent_Static::Type::HOUSE;
		SetRect(128, 96, 80, 96);
		SetPivot(40, 90);
	}
	//TODO 3: Create a post on Tiled and integrate in code
	/*Warnings:
				Each entity has a name and a type. For static entities, type must be "static"
				Only the Object in Tiled will be significant to the code
				Texture rect for post is: (0, 0, 16, 32)
				Correct pivot might be (8, 15) (we will see what it is for later)
	*/
	else if (name == "post") {
		type = ent_Static::Type::POST;
		SetRect(0, 0, 16, 32);
		SetPivot(8, 15);
	}

	//Isometric map--------------------------
	else if (name == "pilar") {
		type = ent_Static::Type::PILAR;
		SetRect(256, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "corner") {
		type = ent_Static::Type::CORNER;
		SetRect(64, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_u_r") {
		type = ent_Static::Type::WALL_UP_RIGHT;
		SetRect(0, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_u_l") {
		type = ent_Static::Type::WALL_UP_LEFT;
		SetRect(384, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_d_r") {
		type = ent_Static::Type::WALL_DOWN_RIGHT;
		SetRect(448, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "wall_d_l") {
		type = ent_Static::Type::WALL_DOWN_LEFT;
		SetRect(192, 0, 64, 64);
		SetPivot(32, 48);
	}
	else if (name == "door") {
		type = ent_Static::Type::DOOR;
		SetRect(128, 0, 64, 64);
		SetPivot(32, 48);
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);


	if (App->map->data.type == MAPTYPE_ISOMETRIC) { //If map is isometric, we have to transform orthogonal position to isometric position
		position.x /= App->map->data.tile_width * 0.5f;
		position.y /= App->map->data.tile_height;

		iPoint pos = App->map->MapToWorld(position.x + 1, position.y + 1);
		position.create(pos.x, pos.y);
	}

	data.tileset.texture = App->tex->Load(App->map->data.properties.objects_path.data()); //Load object texture
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
