# Intro to the problem

In the evolution of 2D games, there was an intance that we want to recreate a 3D world in 2D. We pass from side and top-down view games to 3/4 and isometric games. In the last two cases, it appears a problem, sprite overlaping. So we will need to be able to sort sprites dynamically. 
In games like _Super Mario Bros_ or  _Hotline Miami_ we can identify an sprite ordering without considering the depths, for example: in Mario we can blit turtle befor mario or vice versa, just follows the order of background->entities->pipes and blocks, the game does not require the sorting of the sprites.
In the other hand, we have games like _The Legend of Zelda: Minish Cap_ and _Pokémon_(from Ruby and Sapphire gba versions to Black and White versions) are a good example of the beginning of sorting sprites in video games.
