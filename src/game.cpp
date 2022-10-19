#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "stages.h"
#include "sonido.h"

#include <cmath>

//some globals

Mesh* skyMesh = NULL;
Mesh* floor_plane = NULL;

Texture* skyTexture = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;

std::vector<Stage*> stages;
eStageID currentStage = eStageID::LOAD;

std::vector<BaseEntity*> entities;


void initStages() {
	stages.push_back(new IntroStage());

	stages.push_back(new GameStage());
	stages.push_back(new EndStage());
	currentStage = eStageID::INTRO;
}

Stage* getStage(eStageID stageID) {
	return stages[(int)stageID];
}

Stage* getCurrentStage() {
	return getStage(currentStage);
}

Stage* getEndStage() {
	return stages[3];
}

void setCurrentStage(eStageID stageID) {
	currentStage = stageID;
}

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	brightness = 0.20;
	
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;
	wasLeftMousePressed = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	
	//create our camera
	camera = new Camera;
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 1000.f); //set the projection, we want to be perspective

	stages.reserve(5);
	stages.push_back(new LoadStage());
	getCurrentStage()->render();
	SDL_GL_SwapWindow(this->window); // filho do puttaaaaaa

	initStages();

	// example of shader loading using the shaders manager


	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		std::cout << "Error initzializing audio" << std::endl;
	}

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	camera->enable();

	getCurrentStage()->render();


	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window); 
}

void Game::update(double seconds_elapsed)
{
	SDL_ShowCursor(false);
	
	if ((currentStage == eStageID::GAME || currentStage == eStageID::END) && getCurrentStage()->toMenu)
	{
		getCurrentStage()->toMenu = false;
		setCurrentStage(eStageID::INTRO);
	}

	if (getCurrentStage()->isOver)
	{
		bool newGame = false;
		getCurrentStage()->isOver = false;
		if (currentStage == eStageID::INTRO && getCurrentStage()->newGame)
		{
			newGame = true;
			getCurrentStage()->newGame = false;
		}
		int current = (int)currentStage;
		current += 1;
		setCurrentStage((eStageID)current);
		getCurrentStage()->toMenu = false;
		if (newGame)
		{
			getCurrentStage()->reset();
			getEndStage()->reset();
		}
	}

	getCurrentStage()->update(seconds_elapsed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		/*case SDLK_ESCAPE: must_exit = true; break;*/ //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}

	if (event.button == SDL_BUTTON_LEFT)  //left button mouse
	{
		wasLeftMousePressed = true;
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

