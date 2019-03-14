# Intro to the problem

## Sprite Ordering

In the evolution of 2D games, there was an instance that we want to recreate a 3D world in 2D. We pass from side and top-down view games to 3/4 and isometric games. In the last two cases, it appears a problem, sprite overlapping. So, we will need to be able to sort sprites dynamically.

In games like _Super Mario Bros_ or _Hotline Miami_ we can identify a sprite ordering without considering the depths, for example: in Mario we can render the turtle before Mario or vice versa, just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.

<img src="https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/super-mario-bros.gif" width="560" align="left"/>

With the same reason, _Hotline Miami_ does not to have to sort sprites. We can follow the order of sprites like this: background->furniture->enemies->guns->player.

![HotLine Miami](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/hotline_miami.png)

In the other hand, we have games like _The Legend of Zelda: Minish Cap_ and _Pokémon_ (from Ruby and Sapphire gba versions to Black and White versions) are a good example of the beginning of sorting sprites in video games.

In this example, I set player behind and front of that villager. We can see when the player is down the villager, the player hides the feet of the villager. And the same occurs when the player is upper the villager.
![Pokemon Example](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Pokemon_Village_Example.png)

Sprite ordering might be like this:

![Gif](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Sprite_ordering_gif.gif)

## Camera Culling

Camera culling is a basic method that allows to develop to only work with objects and entities that are only on the camera viewport. This is used to save resources to the machine. It helps especially in games with large worlds and a lot of entities to render. We will see the effect in program later.

# Different approaches by different games

There are some systems to sorting sprites, it depends on the type of game, the resources of the machine and the code structure.

## Cut Sprites

This is the laziest way to solve the sorting sprites problem, but it can serve as leftovers in many cases. It consists in separate a sprite in two parts, the down part and the high part. So, the core of the system is to render first the down part, later all entities, and finally the high part. That system is good to mix static and dynamic entities, for example a building isometric game. There is an example of _Pocket City_ made to its game. It is quite interesting and fits well in that project for the simplicity of the project, the isometric type map and the mobile resources. You can see the separated layers and the result, tinted to see where the cut is.

![Pocket City layers](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/PocketCity_layers.png)
![Pocket City gif](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/PocketCity_result.gif)

## Sorting layers

This is the most common system used, but it could be used with different approaches.

### By position

That consist in sort entities depending of the position of an entity. It is only focus on the vertical position (Y). In order to make sense of depth, all entities and objects will be sorting by Y position, from low Y to high Y (depending on the engine). Entities higher will render before entities below. We can see this example of _Chrono Trigger_ that uses this system.
![Chrono Trigger](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/chronoTrigger.png)

That could consume more resources than we expected, because we must sort a lot of objects. In order to optimize, we have also to implement a camera culling, to only sort and render entities on camera.

### Colliders

Sorting layers by colliders is not widely used but in some cases is the only way to get a good result. A good example for this, is a video made by [Guinxu](https://www.youtube.com/user/GuinxuVideos)(A Spanish indie game developer) that we can found [here](https://youtu.be/eMMnaUmWtnw?t=85)(In Spanish). In order to explain it, I will get some screenshots and I will explain how it works.

First, the problem is that the player will have to be able to pass under the bridge and to pass above. ![Problem example](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Example.png)

To do that, Guinxu solved the problem putting up two types of colliders. One type made player be under bridge, and the other vice versa, so, when player goes over the bridge, the last collider that touches is the red (up arrow) and the player layer moves higher than bridge, when he comes out, the player touches blue collider (down arrow) and moves player layer below bridge. Also, that colliders with arrows active or deactivate colliders that let the player pass or not. For example, if player is going below bridge, he cannot be able to pass for the left and right like if he is passing above bridge, and the same case when player is going above bridge, he cannot be able to jump across bridge.  ![Guinxu1](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Guinxu1.png) ![Guinxu2](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Bridge_Zelda_Guinxu2.png)

### Vector 3D

That is common in isometric map because it looks like a 3D environment. Also, is the most difficult way to implement. We must use 3 dimensions in order to get well the sorting of sprites. To render objects, we will have to project the vector3 to 2D. It is the most tricky and complex way to have sprite sorting. It also is the best way to sort sprites in isometric maps.

# Selected approach

In my case, I will sort layers by position, but with a modification. We won't operate with layers, we will operate directly with a list of entities. We will have an entity type that will be ```STATIC```, which will do nothing. I decided to do it like this because Tiled works only with objects with one tile, we will see what integration I have done from Tiled. Also, we will make a camera culling that prevent to render tiles that there are not on camera, sort entities and render that are only on the camera.

The result of this project that we want is something like this:

![Result gif](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/Result_Gif.gif)

As we can see, player moves around objects and the program sorts the render order. On the title we can see the information of how many tiles are being rendered and how many entities are being sorted and rendered. In my case, I used a fictional pivot to sort entities, entities with pivot above will render before entities with pivot below.

![debug_pivot](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research/blob/master/docs/web_images/debug_result.png)

Pivot is the green rectangle in every entity.

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



# Improvements

* Textures
* Sorting method
* Not iterate all map
