#include "j1App.h"
#include "ent_Player.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "p2List.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"

Player::Player(int x, int y) : j1Entity(Types::PLAYER, x, y) {

	LoadEntityData("player.tsx");

}

Player::~Player()
{
}

bool Player::Start()
{

	state = IDLE;

	current_animation = &anim_idle;
	current_animation->speed = animationSpeed;

	speed.SetToZero();

	AddColliders();

	return true;
}

bool Player::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	//Player collider update
	SetCollidersPos();

	CheckState();
	current_animation->GetCurrentFrame(dt);

	return true;
}

// Called before quitting
bool Player::CleanUp()
{
	bool ret = false;
	ret = App->tex->UnLoad(data.tileset.texture);
	collider.collider->to_delete = true;

	return ret;
}

void Player::Move(float dt) {
	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_REPEAT && speed.x < maxSpeed) {
			speed.x += incrementSpeedX;			
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_REPEAT && speed.x > -maxSpeed) {
			speed.x -= incrementSpeedX;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == j1KeyState::KEY_UP) {
		speed.x = 0.0f; 
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_REPEAT && speed.x < maxSpeed) {
		speed.y += incrementSpeedX;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_REPEAT && speed.x > -maxSpeed) {
		speed.y -= incrementSpeedX;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == j1KeyState::KEY_UP || App->input->GetKey(SDL_SCANCODE_S) == j1KeyState::KEY_UP) {
		speed.y = 0.0f;
	}

	position.x += ceil(speed.x * dt);
	position.y += ceil(speed.y * dt);
	SetCollidersPos();
}

bool Player::Save(pugi::xml_node &node) const
{
	pugi::xml_node e_node = node.append_child("entity");

	e_node.append_attribute("x") = (int)position.x;
	e_node.append_attribute("y") = (int)position.y;
	e_node.append_attribute("spawn_x") = (int)spawn_position.x;
	e_node.append_attribute("spawn_y") = (int)spawn_position.y;
	e_node.append_attribute("type") = (int)type;

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2, float dt) {
}

void Player::PushBack() {

	for (uint i = 0; i < data.num_animations; ++i) {
		for (uint j = 0; j < data.animations[i].num_frames; ++j) {
			switch (data.animations[i].animType) {
			case EntityState::IDLE:
				anim_idle.PushBack(data.animations[i].frames[j]);
				break;
			case EntityState::WALKING:
				anim_walking.PushBack(data.animations[i].frames[j]);
				break;
			default:
				break;
			}
		}
	}
}

void Player::AddColliders() {
	SDL_Rect r;
	
	r = { (int)position.x + collider.offset.x,	(int)position.y + collider.offset.y, collider.width, collider.height };
	collider.collider = App->collider->AddCollider(r, collider.type, this);

}

void Player::SetCollidersPos() {
	collider.collider->SetPos((int)position.x + collider.offset.x, (int)position.y + collider.offset.y);
}

void Player::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 16:
			data.animations[i].animType = EntityState::WALKING;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}

void Player::LoadProperties(pugi::xml_node &node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "animationSpeed")
			animationSpeed = node.attribute("value").as_float();

		else if (nameIdentificator == "maxSpeedX")
			maxSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}

void Player::LoadCollider(pugi::xml_node &node)
{
	p2SString nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "Collider") {
			collider.offset.x = node.attribute("x").as_int();
			collider.offset.y = node.attribute("y").as_int();
			collider.width = node.attribute("width").as_uint();
			collider.height = node.attribute("height").as_uint();
			collider.type = COLLIDER_TYPE::COLLIDER_PLAYER;
		}

		node = node.next_sibling();
	}
}

void Player::Draw(float dt)
{
	App->render->Blit(data.tileset.texture, (int)position.x, (int)position.y, &current_animation->GetCurrentFrame(dt), 1.0F, flip);
}

void Player::CheckState() {
	
	PlayerState prevState = state;
	switch (state) {
	case IDLE:
		if (speed.x != 0.0f || speed.y != 0.0f)
			state = WALKING;
		break;
	case WALKING:
		if (speed.x == 0.0f && speed.y == 0.0f)
			state = IDLE;
		break;
	default:
		break;
	}

	if (prevState != state) //only will change animation when change state
		ChangeState();

	if (speed.x > 0 && flip != SDL_FLIP_NONE)
		flip = SDL_FLIP_NONE;
	else if (speed.x < 0 && flip != SDL_FLIP_HORIZONTAL)
		flip = SDL_FLIP_HORIZONTAL;
}

void Player::ChangeState() {

	switch (state)
	{
	case IDLE:
		current_animation = &anim_idle;
		break;
	case WALKING:
		current_animation = &anim_walking;
		break;
	default:
		break;
	}

	current_animation->reset();
	current_animation->speed = animationSpeed;

}