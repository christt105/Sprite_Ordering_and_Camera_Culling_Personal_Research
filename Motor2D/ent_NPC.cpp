#include "j1App.h"
#include "ent_NPC.h"
#include "j1Render.h"
#include "j1Textures.h"

#include "Brofiler/Brofiler.h"

ent_NPC::ent_NPC(int x, int y, std::string name) : j1Entity(Types::NPC, x, y, name) {

	LoadEntityData("entities/Mudkip.tsx");

}

ent_NPC::~ent_NPC()
{
}

bool ent_NPC::Start()
{

	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;

	data.tileset.texture = App->tex->Load(data.tileset.imagePath.data());

	SetPivot(7, 11);

	return true;
}

bool ent_NPC::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	current_animation->GetCurrentFrame(dt);

	return true;
}

// Called before quitting
bool ent_NPC::CleanUp()
{
	return App->tex->UnLoad(data.tileset.texture);

}

void ent_NPC::OnCollision(Collider* c1, Collider* c2, float dt) {
}

void ent_NPC::PushBack() {

	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}

void ent_NPC::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void ent_NPC::LoadProperties(pugi::xml_node &node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}