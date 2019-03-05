#ifndef __j1APP_H__
#define __j1APP_H__

#include <list>
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Colliders;
class j1Map;
class j1FadeToBlack;
class j1EntityManager;
class j1PathFinding;
class j1Fonts;
class j1Gui;
class j1MainMenu;


enum Levels
{
	MENU = 0,
	CREDITS,
	TUTORIAL,
	FIRST_LEVEL,
	SECOND_LEVEL,
	
};



class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;


private:

	
	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;
	j1Colliders*		collider;
	j1Map*				map;
	j1FadeToBlack*      fade;
	j1EntityManager*	entities;
	j1PathFinding*		pathfinding;
	j1Fonts*			fonts;
	j1Gui*				gui;
	j1MainMenu*         menu;

private:

	std::list<j1Module*>	modules;
	uint					frames;
	float					dt;
	int						argc;
	char**					args;

	std::string			title;
	std::string			organization;

	uint32				framerate_cap = 0;
	bool				cap_framerate = true;
	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	public:
		bool				quit_game = false;
};

extern j1App* App;

#endif