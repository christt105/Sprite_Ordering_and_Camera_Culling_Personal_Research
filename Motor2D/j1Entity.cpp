#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"

#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"


j1Entity::j1Entity(Types type, int PositionX, int PositionY, std::string name) : type(type), position(PositionX, PositionY), name(name)
{}

j1Entity::~j1Entity()
{
	if (collider.collider != nullptr)
		collider.collider->to_delete = true;
}

const Collider* j1Entity::GetCollider() const
{
	return collider.collider;
}

bool j1Entity::Update(float dt) {
	BROFILER_CATEGORY("UpdateEntity", Profiler::Color::Red);

	return true;
}

void j1Entity::Draw() {
	if (current_animation != nullptr)
		App->render->Blit(data.tileset.texture, position.x, position.y, &current_animation->frames[current_animation->GetNumCurrentFrame()], 1.0F, true, flip);
	else
		App->render->Blit(data.tileset.texture, position.x, position.y);
}

bool j1Entity::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(data.tileset.texture);
	if (collider.collider != nullptr)
		collider.collider->to_delete = true;
	current_animation = nullptr;
	

	return ret;
}



//---------------------------------Load Entity XML DATA----------------------------------------------------------------------------

bool j1Entity::LoadEntityData(const char* file) {

	bool ret = true;

	pugi::xml_parse_result result = entity_file.load_file(file);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}

	//fill tileset info
	pugi::xml_node node = entity_file.child("tileset");

	data.tileset.name.assign(node.attribute("name").as_string());
	data.tileset.tilewidth = node.attribute("tilewidth").as_uint();
	data.tileset.tileheight = node.attribute("tileheight").as_uint();
	data.tileset.spacing = node.attribute("spacing").as_uint();
	data.tileset.margin = node.attribute("margin").as_uint();
	data.tileset.tilecount = node.attribute("tilecount").as_uint();
	data.tileset.columns = node.attribute("columns").as_uint();
	data.tileset.imagePath = folder += node.child("image").attribute("source").as_string();
	data.tileset.width = node.child("image").attribute("width").as_uint();
	data.tileset.height = node.child("image").attribute("height").as_uint();

	size = iPoint(data.tileset.tilewidth, data.tileset.tileheight);

	//count how many animations are in file
	node = node.child("tile");
	data.num_animations = 0;
	while (node != NULL) {
		data.num_animations++;
		node = node.next_sibling("tile");
	}

	//reserve memory for all animations
	data.animations = new EntityAnim[data.num_animations];

	//count how many frames for each animation, assign memory for those frames and set id frame start
	node = entity_file.child("tileset").child("tile");
	for (uint i = 0; i < data.num_animations; ++i) {
		data.animations[i].FrameCount(node.child("animation").child("frame"));
		data.animations[i].frames = new SDL_Rect[data.animations[i].num_frames];
		data.animations[i].id = node.attribute("id").as_uint();
		node = node.next_sibling("tile");
	}

	//fill frame array with current information
	node = entity_file.child("tileset").child("tile");
	pugi::xml_node node_frame;
	for (uint i = 0; i < data.num_animations; ++i) {
		node_frame = node.child("animation").child("frame");
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			data.animations[i].frames[j] = data.tileset.GetTileRect(node_frame.attribute("tileid").as_uint());
			node_frame = node_frame.next_sibling("frame");
		}
		node = node.next_sibling("tile");
	}
	//LOG all animation information
	/*for (uint i = 0; i < data.num_animations; ++i) {
		LOG("Animation %i--------", data.animations[i].id);
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			LOG("frame %i x: %i y: %i w: %i h: %i",
				j, data.animations[i].frames[j].x, data.animations[i].frames[j].y,
				data.animations[i].frames[j].w, data.animations[i].frames[j].h);
		}
	}*/

	//Load data
	LoadProperties(entity_file.child("tileset").child("properties").child("property")); //Load properties, is a virtual function because every entity has its variables
	
	LoadCollider(entity_file.child("tileset").child("tile").child("objectgroup").child("object")); //Load collider

	//Convert id animations to enum
	IdAnimToEnum();

	PushBack();

	//deleting entity animation data already loaded in its corresponding animation variables
	for (uint i = 0; i < data.num_animations; ++i) {		//this block of code delete animation data loaded of xml,
		if (data.animations[i].frames != nullptr) {			//is in PushBack() because when load all animation in its
			delete[] data.animations[i].frames;				//corresponding variables, that data is useless
			data.animations[i].frames = nullptr;
		}
	}
	if (data.animations != nullptr) {
		delete[] data.animations;
		data.animations = nullptr;
	}

	return ret;
}

void j1Entity::LoadProperties(pugi::xml_node & node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}

void j1Entity::LoadCollider(pugi::xml_node &node)
{
	collider.offset.x = node.attribute("x").as_int();
	collider.offset.y = node.attribute("y").as_int();
	collider.width = node.attribute("width").as_uint();
	collider.height = node.attribute("height").as_uint();

	std::string colliderType = node.attribute("type").as_string();
	if (colliderType == "COLLIDER_PLAYER")
		collider.type = COLLIDER_TYPE::COLLIDER_PLAYER;
}

void j1Entity::IdAnimToEnum()
{
	data.animations[0].animType = EntityState::IDLE;
}

void j1Entity::AddColliders(j1Entity* callback)
{
		SDL_Rect r = {	(int)position.x + collider.offset.x,
						(int)position.y + collider.offset.y,
						collider.width, collider.height };

		collider.collider = App->collider->AddCollider(r, collider.type, callback);

}

void j1Entity::SetPivot(const int & x, const int & y)
{
	pivot.create(x, y);
}

//Functions to help loading data in xml-------------------------------------
//Get the rect info of an id of tileset
SDL_Rect TileSetEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}

//Return how many frames are in one animation
uint EntityAnim::FrameCount(pugi::xml_node& n) {
	num_frames = 0;
	pugi::xml_node node = n;
	for (; node != NULL; node = node.next_sibling("frame")) {
		num_frames++;
	}

	return num_frames;
}