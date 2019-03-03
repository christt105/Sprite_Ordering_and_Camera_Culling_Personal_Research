#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Window.h"
#include <math.h>
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "p2Point.h"

#include "Brofiler\Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw", Profiler::Color::Azure);

	if(map_loaded == false)
		return;

	p2List_item<MapLayer*>* layer = data.layers.start;
	uint id = 0;
	
	//Local variables to simplify code
	int scale = App->win->GetScale();
	iPoint camera(WorldToMap(-App->render->camera.x/scale, -App->render->camera.y/scale)); //camera pass virtual coordinates to map coordinates with the correct scale
	iPoint cameraSize(WorldToMap(App->render->camera.w / scale, App->render->camera.h / scale));

	while (layer) {
		if (layer->data->visible)
			for (uint i = camera.y; i <= cameraSize.y + camera.y && i < layer->data->height; ++i) { //since camera position to camera size plus initial position or to final of layer
				for (uint j = camera.x; j <= cameraSize.x + camera.x + 1 && j < layer->data->width; ++j) {

					id = layer->data->Get(j, i);
					if (id != 0) {
						TileSet* tileset = GetTilesetFromTileId(id);
						if (tileset != nullptr)
							App->render->Blit(tileset->texture, MapToWorld(j, i).x, MapToWorld(j, i).y, &tileset->GetTileRect(id));
					}
				}
			}
		layer = layer->next;
	}

}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* tileset = data.tilesets.start;
	TileSet* ret = tileset->data;
	while (tileset) {
		if (id >= tileset->data->firstgid)
			ret = tileset->data;
		else return ret;

		tileset = tileset->next;
	}
	return ret;
}

void j1Map::AddCollidersMap()
{

	p2List_item<ColliderObject*>* colliders = data.colliders.start;

	while (colliders) {
		SDL_Rect collider_rect = { colliders->data->coll_x,colliders->data->coll_y,colliders->data->coll_width,colliders->data->coll_height };
		App->collider->AddCollider(collider_rect, colliders->data->type);
	
		colliders = colliders->next;
	}

}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);


	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = int (data.tile_width * 0.5F) * (x - y);
		ret.y = int (data.tile_height * 0.5F) * (x + y);
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const {
	iPoint ret(0, 0);

	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = x / data.tile_width + y / data.tile_height;
		ret.y = 2 * y / data.tile_height - ret.x;
		ret.x--; ret.y--;
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}


inline uint MapLayer::Get(int x, int y) const {
	return tiles[y * this->width + x];
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;
	
	while (item2 != NULL) {
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Remove all colliders
	p2List_item<ColliderObject*>* item3;
	item3 = data.colliders.start;

	while (item3!= NULL) {
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.colliders.clear();



	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer")) {

		MapLayer* lay = new MapLayer();

		if (ret == true)
			ret = LoadLayer(layer, lay);

		data.layers.add(lay);
	}

	

	//Load Object data ------------------------------------------------
	pugi::xml_node objectGroup;
	pugi::xml_node object;
	
	for (objectGroup = map_file.child("map").child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
	{
		for (object = objectGroup.child("object"); object;object = object.next_sibling("object")) {

			ColliderObject* obj = new ColliderObject();
		
			if (ret == true && object != NULL) 
				ret = LoadObject(object, obj);
				
			data.colliders.add(obj);
		}
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		/*p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}

		p2List_item<ColliderObject*>* item_object = data.colliders.start;
		while (item_object != NULL) {
			ColliderObject* obj = item_object->data;
			LOG("Object ------");
			LOG("name: %s", obj->name.GetString());
			LOG("Collider Position: (%i , %i)", obj->coll_x, obj->coll_y);
			LOG("width: %i  height: %i", obj->coll_width, obj->coll_height);
			item_object = item_object->next;
		}*/

	}
	map_file.reset();
	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		LoadProperties(map.child("properties"));
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red;
			p2SString green;
			p2SString blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

void j1Map::LoadProperties(pugi::xml_node& properties_node, MapLayer* layer) {

	for (properties_node = properties_node.child("property"); properties_node != NULL; properties_node = properties_node.next_sibling()) {
		p2SString prop = properties_node.attribute("name").as_string();
		
	}
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();


	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	if (node.empty())	ret = false;

	layer->name.create(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->visible = node.attribute("visible").as_bool(true);

	//Load properties of layer
	pugi::xml_node n_property = node.child("properties");
	LoadProperties(n_property, layer);
	
	layer->tiles = new uint[layer->width*layer->height];

	memset(layer->tiles, 0, layer->width*layer->height * sizeof(uint));

	pugi::xml_node tile = node.child("data").child("tile");

	for (uint i = 0; i < layer->width*layer->height; i++) {
		layer->tiles[i] = tile.attribute("gid").as_uint();
		tile = tile.next_sibling("tile");
	}

	return ret;
}

bool j1Map::LoadObject(pugi::xml_node& node_object, ColliderObject* obj) {

	bool ret = true;
	if (node_object.empty())	ret = false;

	//Load Collider / Entity data
	obj->name = node_object.attribute("name").as_string();
	obj->tile_id = node_object.attribute("id").as_uint();
	obj->coll_x = node_object.attribute("x").as_int();
	obj->coll_y = node_object.attribute("y").as_int();
	obj->coll_height = node_object.attribute("height").as_uint();
	obj->coll_width = node_object.attribute("width").as_uint();

	//Load Collider type from ObjectGroup
	pugi::xml_node objGroup = node_object.parent();
	p2SString type(objGroup.child("properties").child("property").attribute("value").as_string());

	if (type == "COLLIDER_NONE")
	{
		obj->type = COLLIDER_NONE;
	}
	else if (type == "COLLIDER_FLOOR")
	{
		obj->type = COLLIDER_FLOOR;
	}

	return ret;
}