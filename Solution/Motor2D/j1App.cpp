#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	frames = 0;

	input		= new j1Input();
	win			= new j1Window();
	render		= new j1Render();
	tex			= new j1Textures();
	audio		= new j1Audio();
	scene		= new j1Scene();
	collider	= new j1Colliders();
	map			= new j1Map();
	entities	= new j1EntityManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(scene);
	AddModule(collider);
	AddModule(entities);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
j1App::~j1App()
{
	// release modules

	for (std::list<j1Module*>::reverse_iterator item = modules.rbegin(); item != modules.rend(); ++item)
	{
		RELEASE(*item);
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool j1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());
		framerate_cap = 1000 / app_config.attribute("framerate_cap").as_uint();
	}

	if(ret == true)
	{
		std::list<j1Module*>::iterator item = modules.begin();

		while (item != modules.end() && ret)
		{
			ret = (*item)->Awake(config.child((*item)->name.data()));
			item++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	std::list<j1Module*>::iterator item = modules.begin();

	while (item != modules.end() && ret)
	{
		if ((*item)->active == false) {
			item++;
			continue;
		}
		ret = (*item)->Start();
		item++;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("Update", Profiler::Color::Red);
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true || quit_game)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F11) == j1KeyState::KEY_DOWN)
		cap_framerate = !cap_framerate;

	frame_count++;
	last_sec_frame_count++;
	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("FinishUpdate", Profiler::Color::Magenta);

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	//static char title[256];
	//if (cap_framerate) {
	//	sprintf_s(title, 256, "SpriteOrdering and Camera Culling | FPS %i | average FPS: %.2f | Last Frame Ms: %02u | Cap ON | VSync OFF ",
	//							frames_on_last_update, avg_fps, last_frame_ms);
	//}
	//else {
	//	sprintf_s(title, 256, "SpriteOrdering and Camera Culling | FPS %i | average FPS: %.2f | Last Frame Ms: %02u | Cap OFF | VSync OFF ",
	//							frames_on_last_update, avg_fps, last_frame_ms);
	//}

	//App->win->SetTitle(title);

	if (cap_framerate) {
		if (framerate_cap >= last_frame_ms)
			SDL_Delay(framerate_cap - last_frame_ms);
	}
	
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate", Profiler::Color::Yellow);

	bool ret = true;
	j1Module* pModule = NULL;

	for(std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("DoUpdate", Profiler::Color::Red);

	bool ret = true;
	j1Module* pModule = NULL;

	for (std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = *item;

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate", Profiler::Color::Green);

	bool ret = true;
	j1Module* pModule = NULL;

	for (std::list<j1Module*>::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = *item;

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;

	for (std::list<j1Module*>::reverse_iterator item = modules.rbegin(); item != modules.rend() && ret == true; ++item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.data();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.data();
}