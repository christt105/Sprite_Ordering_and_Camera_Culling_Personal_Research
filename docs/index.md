# Intro to the problem

## Sprite Ordering

In the evolution of 2D games, there was an intance that we want to recreate a 3D world in 2D. We pass from side and top-down view games to 3/4 and isometric games. In the last two cases, it appears a problem, sprite overlaping. So we will need to be able to sort sprites dynamically.

In games like _Super Mario Bros_ or  _Hotline Miami_ we can identify an sprite ordering without considering the depths, for example: in Mario we can blit turtle befor mario or vice versa, just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.

In the other hand, we have games like _The Legend of Zelda: Minish Cap_ and _Pokémon_(from Ruby and Sapphire gba versions to Black and White versions) are a good example of the beginning of sorting sprites in video games.

## Camera Culling

Camera culling is a basic method that allows to develop to only work with objects and entities that are only on the camera viewport. This is used to save resources to the machine. It helps especially in games with large worlds and a lot of entities to render. We will see the effect in program later.

# Different approaches by different games

There is a some systems to sorting sprites, it depends on the type of game, the resources of the machine and the code structure.

## Cut Sprites

This is the most lazy way to solve the sorting sprites problem, but it can serve as leftovers in many cases. It consist in separate a sprite in two parts, the down part and the high part. So, the core of the system is to render first the down part, later all entities, and finally the high part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made to its game. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources.
[Sprite de Pocket City]

## Sorting layers

This is the most common system used, but it could be used with differents approaches.

### Colliders



### By position
