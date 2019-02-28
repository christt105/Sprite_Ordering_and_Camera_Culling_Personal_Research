#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL_timer.h"
#include "p2SString.h"
#include "j1Module.h"	
#include "p2List.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct Collider;

struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	p2SString name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	p2SString imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

enum class EntityState {
	IDLE = 0,
	WALKING,
	HIT,
	DETECTING,
	DEAD,
	JUMP,
	FALL,
	LAND,
	HADOUKEN,
	PUNCH,
	IDLE_GHOST,
	WALKING_GHOST,
	JUMP_GHOST,
	FALL_GHOST,
	LAND_GHOST,
	UNKNOWN
};

struct EntityAnim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;
	EntityState animType;

	uint FrameCount(pugi::xml_node&);
};

struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint num_animations = 0;
};

struct COLLIDER_INFO {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	COLLIDER_TYPE type;
};

class j1Entity
{
public:

	enum class Types
	{
		PLAYER,
		GLADIATOR,
		FLYING_TONGUE,
		COIN,

		UNKNOWN
	};

public:

	j1Entity(Types type, int x, int y);
	virtual ~j1Entity();

	const Collider* GetCollider() const;

	bool LoadEntityData(const char*);
	virtual void LoadProperties(pugi::xml_node&);
	virtual void LoadCollider(pugi::xml_node&);
	virtual void IdAnimToEnum();
	virtual void PushBack() {};
	virtual void AddColliders(j1Entity* c = nullptr);

	virtual bool Start() { return true; }
	virtual bool Update(float dt);
	virtual void Move(float dt) {}
	virtual void Draw();
	virtual void CreatePath() {}
	virtual void OnCollision(Collider* collider1, Collider * collider2, float dt) {}
	virtual bool CleanUp();

	virtual bool Save(pugi::xml_node&) const;
	virtual bool Load(pugi::xml_node&);

	j1Entity* GetEntityPosition(j1Entity::Types type);
	
public:

	fPoint position;
	const fPoint spawn_position;
	fPoint		speed;

	Types type;

	Animation anim_idle;

	const char* AttackSound = nullptr;

	COLLIDER_INFO collider;

	EntityInfo data;

	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;

	pugi::xml_document	entity_file;
	
	Animation* current_animation = nullptr;
	float animationSpeed;
};
#endif // __ENTITY_H__