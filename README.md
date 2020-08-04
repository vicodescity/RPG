# RPG
A single player Role playing game made with UE4 with C++.

You can build the project on any OS by right clicking on the uproject file, and clicking generate project files (it will show generate visual studio project files on windows.

And by configuring the DefaultEngine.ini file if you are not on windows.

It is recommended and saves you alot of time to use blueprints for AI behaviour trees, blackboards and also for Widgets, as these were built around nodes and therefore are more 
optimized for use with blueprints.

STATS
The Health and Stamina are just floats which are stored in an actor component called RStatsComponent.
The Stamina gets depeleted while sprinting or while Attacking(Planning to fix some bugs on the next commit).

AI

The ai behaviour is handled exclusively from blueprints with a behaviour tree, for now it has just two states.
1) for chasing the player if seen and attacks the player with a simple light attack. 
2) Roams randomly around the environment.

I plan to redo this later with just c++ when the entire project is completed.



The naming conventions used for the function are very discriptive so as to prevent unnecessary comments that just makes 
the code harder to read.