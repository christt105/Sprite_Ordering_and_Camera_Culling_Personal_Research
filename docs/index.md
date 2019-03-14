# Intro to the problem

## Sprite Ordering

In the evolution of 2D games, there was an instance that we want to recreate a 3D world in 2D. We pass from side and top-down view games to 3/4 and isometric games. In the last two cases, it appears a problem, sprite overlapping. So we will need to be able to sort sprites dynamically.

In games like _Super Mario Bros_ or _Hotline Miami_ we can identify a sprite ordering without considering the depths, for example: in Mario we can render the turtle before Mario or vice versa, just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.

In the other hand, we have games like _The Legend of Zelda: Minish Cap_ and _Pokémon_ (from Ruby and Sapphire gba versions to Black and White versions) are a good example of the beginning of sorting sprites in video games.

## Camera Culling

Camera culling is a basic method that allows to develop to only work with objects and entities that are only on the camera viewport. This is used to save resources to the machine. It helps especially in games with large worlds and a lot of entities to render. We will see the effect in program later.

# Different approaches by different games

There are some systems to sorting sprites, it depends on the type of game, the resources of the machine and the code structure.

## Cut Sprites

This is the laziest way to solve the sorting sprites problem, but it can serve as leftovers in many cases. It consists in separate a sprite in two parts, the down part and the high part. So, the core of the system is to render first the down part, later all entities, and finally the high part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made to its game. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources.
[Sprite de Pocket City]

## Sorting layers

This is the most common system used, but it could be used with different approaches.

### By position

That consist in sort entities depending of the position of an entity. It is only focus on the vertical position (Y). In order to make sense of depth, all entities and objects will be sorting by Y position, from low Y to high Y (depending on the engine). Entities higher will render before entities below. We can see this example of _Chrono Trigger_ that uses this system.
[Chrono Trigger]

That could consume more resources than we expected, because we must sort a lot of objects. In order to optimize, we have also to implement a camera culling, to only sort and render entities on camera.

### Colliders

Sorting layers by colliders is not widely used but in some cases is the only way to get a good result. A good example for this, is a video made by [Guinxu](https://www.youtube.com/user/GuinxuVideos)(A Spanish indie game developer) that we can found [here](https://youtu.be/eMMnaUmWtnw?t=85)(In Spanish). In order to explain it, I will get some screenshots and I will explain how it works.

First, the problem is that the player will have to be able to pass under the bridge and to pass above. [foto ejemplo]

To do that, Guinxu solved the problem putting up two types of colliders. One type made player be under bridge, and the other vice versa, so, when player goes over the bridge, the last collider that touches is the red (up arrow) and the player layer moves higher than bridge, when he comes out, the player touches blue collider (down arrow) and moves player layer below bridge. Also, that colliders with arrows active or deactivate colliders that let the player pass or not. For example, if player is going below bridge, he cannot be able to pass for the left and right like if he is passing above bridge, and the same case when player is going above bridge, he cannot be able to jump across bridge.  [foto ejemploguinxu]


### Z position


