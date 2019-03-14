# Intro to the problem

## Sprite Ordering

In the evolution of 2D games, there was an instance that we want to recreate a 3D world in 2D. We pass from side and top-down view games to 3/4 and isometric games. In the last two cases, it appears a problem, sprite overlapping. So, we will need to be able to sort sprites dynamically.

In games like _Super Mario Bros_ or _Hotline Miami_ we can identify a sprite ordering without considering the depths, for example: in Mario we can render the turtle before Mario or vice versa, just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/super-mario-bros.gif?raw=true" width="560" align="left"/>

With the same reason, _Hotline Miami_ does not to have to sort sprites. We can follow the order of sprites like this: background->furniture->enemies->guns->player.

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/hotline_miami.png?raw=true" width="560" align="right"/>

In the other hand, we have games like _The Legend of Zelda: Minish Cap_ and _Pokémon_ (from Ruby and Sapphire gba versions to Black and White versions) are a good example of the beginning of sorting sprites in video games.

In this example, I set player behind and front of that villager. We can see when the player is down the villager, the player hides the feet of the villager. And the same occurs when the player is upper the villager.
<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Pokemon_Village_Example.png?raw=true" width="560"/>

Sprite ordering might be like this:

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Sprite_ordering_gif.gif?raw=true" width="560"/>

## Camera Culling

Camera culling is a basic method that allows to develop to only work with objects and entities that are only on the camera viewport. This is used to save resources to the machine. It helps especially in games with large worlds and a lot of entities to render. We will see the effect in program later.

# Different approaches by different games

There are some systems to sorting sprites, it depends on the type of game, the resources of the machine and the code structure.

## Cut Sprites

This is the laziest way to solve the sorting sprites problem, but it can serve as leftovers in many cases. It consists in separate a sprite in two parts, the down part and the high part. So, the core of the system is to render first the down part, later all entities, and finally the high part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made to its game. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources. You can see the separated layers and the result, tinted to see where the cut is.

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/PocketCity_layers.png?raw=true" width="560"/>
<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/PocketCity_result.gif?raw=true" width="560"/>

## Sorting layers

This is the most common system used, but it could be used with different approaches.

### By position

That consist in sort entities depending of the position of an entity. It is only focus on the vertical position (Y). In order to make sense of depth, all entities and objects will be sorting by Y position, from low Y to high Y (depending on the engine). Entities higher will render before entities below. We can see this example of _Chrono Trigger_ that uses this system.
<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/chronoTrigger.png?raw=true" width="560"/>

That could consume more resources than we expected, because we must sort a lot of objects. In order to optimize, we have also to implement a camera culling, to only sort and render entities on camera.

### Colliders

Sorting layers by colliders is not widely used but in some cases is the only way to get a good result. A good example for this, is a video made by [Guinxu](https://www.youtube.com/user/GuinxuVideos)(A Spanish indie game developer) that we can found [here](https://youtu.be/eMMnaUmWtnw?t=85)(In Spanish). In order to explain it, I will get some screenshots and I will explain how it works.

First, the problem is that the player will have to be able to pass under the bridge and to pass above. <img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Example.png?raw=true" width="560"/>

To do that, Guinxu solved the problem putting up two types of colliders. One type made player be under bridge, and the other vice versa, so, when player goes over the bridge, the last collider that touches is the red (up arrow) and the player layer moves higher than bridge, when he comes out, the player touches blue collider (down arrow) and moves player layer below bridge. Also, that colliders with arrows active or deactivate colliders that let the player pass or not. For example, if player is going below bridge, he cannot be able to pass for the left and right like if he is passing above bridge, and the same case when player is going above bridge, he cannot be able to jump across bridge.
<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Guinxu1.png?raw=true" width="560"/>
<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Guinxu2.png?raw=true" width="560"/>

### Vector 3D

That is common in isometric map because it looks like a 3D environment. Also, is the most difficult way to implement. We must use 3 dimensions in order to get well the sorting of sprites. To render objects, we will have to project the vector3 to 2D. It is the most tricky and complex way to have sprite sorting. It also is the best way to sort sprites in isometric maps.

# Selected approach

In my case, I will sort layers by position, but with a modification. We won't operate with layers, we will operate directly with a list of entities. We will have an entity type that will be ```STATIC```, which will do nothing. I decided to do it like this because Tiled works only with objects with one tile, we will see what integration I have done from Tiled. Also, we will make a camera culling that prevent to render tiles that there are not on camera, sort entities and render that are only on the camera.

The result of this project that we want is something like this:

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Result_Gif.gif?raw=true" width="560"/>

As we can see, player moves around objects and the program sorts the render order. On the title we can see the information of how many tiles are being rendered and how many entities are being sorted and rendered. In my case, I used a fictional pivot to sort entities, entities with pivot above will render before entities with pivot below.

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/debug_result.png?raw=true" width="560"/>

Pivot is the green rectangle in every entity.

In order to work with Tiled easily, I have implemented code to import entities to the game. I will explain how it works.

## Importing dynamic entities from Tiled

We can work with tilesets in Tiled. It allows us some functionalities. Only we have to do is to study what it gives and incorporate to our code.
First, there is the main information of the tileset that we can see on Properties window.
<img src="imagen de las propiedades"/>
Here we have some general information about the tileset. The most important are:
* Name
* Orientation
* Columns
* Image
  * Source
  * Tile Width
  * Tile Height
  * Margin
  * Spacing

All of this variables will be important to import to the program. The is something strighly important: Custom Properties. There we can assign every variable we want to the code and edit so fast. In my example I use AnimationSpeed but it can be used to many things. It can be of different types: bool, float, int, string...

Also is a powerfull tool to implement animations easily. All we have to do is pick the camera icon, set a reference tile and drag it to the box to set the animation of an action. Each tile has an id that we will use later to assign the animation.

<img src/>

We can also set many colliders and load after in code, but it won't affect to the research, so we won't touch that utility.

After we save the file we will get something like that:

<img src xml />

Here we have in a XML the general information, properties and animations.

Now, in the code we will create some structs to save the data. We will create a Entity class and then all entities than has an special behaviour will heretate from it.

```cpp
struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint num_animations = 0;
};
```

We will create a basic structure that saves the tileset of the entity and the animations.

```cpp
struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	std::string name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	std::string imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};
```

For the tileset we will save some information to load the texture. ```SDL_Rect GetTileRect(int id) const;``` returns a rect given an id. For example, id 0 is the first tile, the player looking down, so, will fill the rect with the tileset information given the width and height, and the position where is in the texture.

```cpp
//Get the rect info of an id of tileset
SDL_Rect TileSetEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}
```

On animations we will save the id where is come from, number of frames, the position of frames on the texture and the type of the animation. ```uint FrameCount();``` is a simple function that iterates all xml nodes and return how many frames animation has.

```cpp
struct EntityAnim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;
	EntityState animType;

	uint FrameCount(pugi::xml_node&);
};
```

We won't use colliders but I will give the struct to save information. We must save the collider, the offset from the entity position, the size and the type.

```cpp
struct COLLIDER_INFO {
	Collider* collider = nullptr;
	iPoint offset;
	int width = 0;
	int height = 0;
	COLLIDER_TYPE type;
};
```

In order to load all this information we will have some functions, some of that will be virtual because every entity will have its animations and properties.

```cpp
bool LoadEntityData(const char*); //Loads entity by tsx file
	//Virtual functions because every entity has its properties, variables, animations...------------------------
	virtual void LoadProperties(pugi::xml_node&);
	virtual void LoadCollider(pugi::xml_node&);
	virtual void IdAnimToEnum();
	virtual void PushBack() {};
	virtual void AddColliders(j1Entity* c = nullptr);
	//-----------------------------------------------------------------------------------------------------------
```

```cpp LoadEntityData()``` will contain all others functions. First we save tileset information:

```cpp
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
```

Later we count how many animations there are and reserve memory for all of them.
```cpp
	//count how many animations are in file
	node = node.child("tile");
	data.num_animations = 0;
	while (node != NULL) {
		data.num_animations++;
		node = node.next_sibling("tile");
	}

	//reserve memory for all animations
	data.animations = new EntityAnim[data.num_animations];
```

Now, we want to save frames of animations.

```cpp
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
```

Later we save properties.

```cpp
LoadProperties(entity_file.child("tileset").child("properties").child("property")); //Load properties, is a virtual function because every entity has its variables
```
An example of Load properties is:
```cpp
void Player::LoadProperties(pugi::xml_node &node)
{
	std::string nameIdentificator;
	while (node) {
		nameIdentificator = node.attribute("name").as_string();

		if (nameIdentificator == "AnimationSpeed")
			animationSpeed = node.attribute("value").as_float();

		node = node.next_sibling();
	}
}
```
Later Load collider.
```cpp
LoadCollider(entity_file.child("tileset").child("tile").child("objectgroup").child("object")); //Load collider
```
And there is an example:
```cpp
void Player::LoadCollider(pugi::xml_node &node)
{
	std::string nameIdentificator;
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
```
Now, we have to convert id animations to enum animations. To do that we use a virtual function. An example may be:
```cpp
void Player::IdAnimToEnum()
{
	for (uint i = 0; i < data.num_animations; ++i) {
		switch (data.animations[i].id) {
		case 0:
			data.animations[i].animType = EntityState::IDLE;
			break;
		case 3:
			data.animations[i].animType = EntityState::WALKING;
			break;
		default:
			data.animations[i].animType = EntityState::UNKNOWN;
			break;
		}
	}
}
```
After getting all animations, we have to make the pushback of the frames. An example:
```cpp
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
```

To finish, we have to delete all reserved memory that we won't use.
```cpp
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
```

With that we have finished the load of a entity setted with Tiled. You can find the code of load entity [here](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/Solution/Motor2D/j1Entity.h) for the header and [here](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/Solution/Motor2D/j1Entity.cpp) for the .cpp.

## Importing static entities from Tiled

For static entities it is a little different. It could not be that authomatic. But it is not difficult.

You can download the release [here](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/releases/tag/1.5).
And if you want the code, you can get it [here](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research).

# Links to more documentation

If you want to know more about Sprite Ordering, here do you have some links of interest:

* [Cut Sprites solution](https://trederia.blogspot.com/2014/08/z-ordering-of-sprites-in-tiled-maps.html)
* [Different approaches to Ordering by position](https://eliasdaler.wordpress.com/2013/11/20/z-order-in-top-down-2d-games/)
* [Sprite ordering and tiles](https://love2d.org/forums/viewtopic.php?t=77149)
* [Pocket City solution](https://blog.pocketcitygame.com/cheating-at-z-depth-sprite-sorting-in-an-isometric-game/amp/)

In that case, I will separate links in two sections, because sorting in isometric view could be tricky in some cases:

* [Isometric with objects with more than one tile](https://gamedev.stackexchange.com/questions/103442/how-do-i-determine-the-draw-order-of-isometric-2d-objects-occupying-multiple-til)
* [Isometric sprite ordering with Tiled](https://discourse.mapeditor.org/t/isometric-depth-sorting/736)
* [Drawing isometric boxes in the correct order](https://shaunlebron.github.io/IsometricBlocks/)
* [Isometric depth sorting with big objects](https://stackoverflow.com/questions/11166667/isometric-depth-sorting-issue-with-big-objects)
* [Optimization of sorting sprites in isometric](https://gamedev.stackexchange.com/questions/97442/sorting-sprites-layers-in-isometric-view)
* [Sort with Z-buffer and anchor point, isometric map](https://gamedev.stackexchange.com/questions/69851/isometric-map-draw-sort-with-z-buffer-and-anchor-point)
* [Isometric sort algorithm](https://gamedev.stackexchange.com/questions/8151/how-do-i-sort-isometric-sprites-into-the-correct-order)

# TODOs and Solution

## TODO 1:

# Issues

objetos con mas de un tile en isometrico

# Improvements

* Textures
* Sorting method
* Not iterate all map
