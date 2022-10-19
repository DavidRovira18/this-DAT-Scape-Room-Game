#include "stages.h"


mapArea* hab1;
PrefabEntity* bed;
CharacterEntity* player;
IAEntity* robot;
eMapArea currentArea = eMapArea::HAB1;
LightEntity* light;

std::vector<mapArea*> mapAreas;
std::vector<LightEntity*> lights;

float currentVol;

void initMapAreas() 
{
	mapAreas.clear();
	hab1 = new mapArea();
	hab1->prefab.clear();
	hab1->interactive.clear();
	hab1->ID = eMapArea::HAB1;
	//std::vector<BaseEntity*> EntitiesBedroom1;
	LoadPropEditorScenePrefab("data/structure/structure.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/BedroomPrefab/bedroom1.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/Bedroom2Prefab/bedroom2.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/LivingroomPrefab/Livingroom.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/KitchenPrefab/Kitchen.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/GardenPrefab/GardenPrefab.scene", hab1->prefab);
	LoadPropEditorScenePrefab("data/BathroomPrefab/BathroomPrefab.scene", hab1->prefab);
	LoadPropEditorSceneInteractive("data/Interactive/Interactive.scene", hab1->interactive);
	light->load("data/lights/lights.txt", lights);

	mapAreas.push_back(hab1);
}


mapArea* getCurrentArea(eMapArea currentArea) 
{
	return mapAreas[(int)currentArea];
}

Stage::Stage() 
{
	isOver = false;
	toMenu = false;
	newGame = false;
	currentVol = 0.5;
};

void Stage::render() 
{
	
};

LoadStage::LoadStage() 
{
	loadingScreen1 = Texture::Get("data/GUI/logoThisAndd.png");
	loadingScreen2 = Texture::Get("data/GUI/logoThisAndda.png");
	loadingScreen3 = Texture::Get("data/GUI/logoThisAnddat.png");
	loadingScreen4 = Texture::Get("data/GUI/logoThisAnddatFinal.png");
	loadingScreens.reserve(4);
	loadingScreens.push_back(loadingScreen1);
	loadingScreens.push_back(loadingScreen2);
	loadingScreens.push_back(loadingScreen3);
	loadingScreens.push_back(loadingScreen4);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	blackTex = Texture::Get("data/GUI/blackScreen.png");
}

eStageID LoadStage::getID() 
{
	return eStageID::LOAD;
}

void LoadStage::render() 
{

	glDisable(GL_CULL_FACE);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, shader, blackTex, Vector4(1, 1, 1, 1));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < loadingScreens.size(); i++)
	{
		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2 - 50, 720, 150, shader, loadingScreens[i], Vector4(1, 1, 1, 0.9));
		SDL_GL_SwapWindow(Game::instance->window);
		Sleep(600);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

void LoadStage::update(double seconds_elapsed) 
{

}

IntroStage::IntroStage() 
{
	initMapAreas();

	audio = new Audio();
	startCameraPosition = Vector3(-1.775f, 1.4f, 1.48);  //On volem que comenci
	introPosition1 = Vector3(0.50f, 0.0f, 1.327f); //On volem que vagi per primer cop
	screenPosition = Vector3(0.75f, 0.0f, 1.445f);  //Desde on volem mirar la pantalla

	arrivedPos1 = false;
	arrivedScreen = false;
	finalizedRotation = false;

	soundsOn = false;

	showScreen = false;
	showControls = false;
	showOptions = false;

	robot = new IAEntity();
	player = new CharacterEntity();

	a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	initIntroTextures();

	Vector3 eye;
	Vector3 center;
	Vector3 up;

	camModelRotation = Vector3(0, -0.5, -1);

	Game::instance->camera->eye = startCameraPosition;
};

void IntroStage::initIntroTextures()
{
	spaceTex = Texture::Get("data/GUI/spaceButton.png");
	blackTex = Texture::Get("data/GUI/blackScreen.png");
	newGameTex = Texture::Get("data/GUI/newGame.png");
	loadTex = Texture::Get("data/GUI/loadGame.png");
	controlsTex = Texture::Get("data/GUI/controls.png");
	optionsTex = Texture::Get("data/GUI/options.png");
	exitTex = Texture::Get("data/GUI/exit.png");

	controlsGUITex = Texture::Get("data/GUI/controlsGUI.png");
	exitControlsTex = Texture::Get("data/GUI/exitControls.png");

	brightUpTex = Texture::Get("data/GUI/brightUp.png");
	brightDownTex = Texture::Get("data/GUI/brightDown.png");
	volumeUpTex = Texture::Get("data/GUI/volumeUp.png");
	volumeDownTex = Texture::Get("data/GUI/volumeDown.png");
}

eStageID IntroStage::getID() 
{

	return eStageID::INTRO;

}

void IntroStage::renderComputerScreen()
{
	glDisable(GL_CULL_FACE);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, a_shader, blackTex, Vector4(1, 1, 1, 1));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;

	if(renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 200, 700, 75, a_shader, newGameTex))
	{
		audio->PlaySample("data/sounds/computerStop.wav", false);
		audio->Stop("data/sounds/computerWorking.wav");
		audio->Stop("data/sounds/keyboardTyping.mp3");
		playAudio = true;
		newGame = true;
		isOver = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 100, 700, 75, a_shader, loadTex))
	{
		if (loadGame(player, mapAreas, robot))
		{
			audio->Stop("data/sounds/computerWorking.wav");
			audio->Stop("data/sounds/keyboardTyping.mp3");
			isOver = true;
			loadGame(player, mapAreas, robot);
			playAudio = true;
		}
		//SOUND COM DONANT FEEDBACK QUE NO HI HA CARREGAT RES
		else {
			audio->PlaySample("data/sounds/wrong.mp3", false);
		}
	
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2, 700, 75, a_shader, controlsTex))
	{
		showControls = true;
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 100, 700, 75, a_shader, optionsTex))
	{
		showOptions = true;
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 200, 700, 75, a_shader, exitTex))
	{
		Game::instance->must_exit = true;
		playAudio = true;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (playAudio) audio->PlaySample("data/sounds/click.wav", false);

	Game::instance->wasLeftMousePressed = false;
}

void IntroStage::renderControls()
{
	glDisable(GL_CULL_FACE);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, a_shader, blackTex, Vector4(1, 1, 1, 1));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderQuadMenu(Game::instance->window_width/2, Game::instance->window_height / 2 - 50, 400, 400, a_shader, controlsGUITex, Vector4(1, 1, 1, 0.9));

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 250, 700, 75, a_shader, exitControlsTex))
	{
		showControls = false;
		audio->PlaySample("data/sounds/click.wav", false);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;
}

void IntroStage::renderOptions()
{
	glDisable(GL_CULL_FACE);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, a_shader, blackTex, Vector4(1, 1, 1, 1));

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 200, 700, 75, a_shader, brightUpTex))
	{
		if (Game::instance->brightness < 0.3)
			Game::instance->brightness += 0.05;
	}	
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 100, 700, 75, a_shader, brightDownTex))
	{
		if (Game::instance->brightness -= 0.1 > 0.1)
			Game::instance->brightness -= 0.025;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2, 700, 75, a_shader, volumeUpTex))
	{
		currentVol += 0.1f;
		clamp(currentVol);
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 100, 700, 75, a_shader, volumeDownTex)) 
	{
		currentVol -= 0.1f;
		clamp(currentVol);
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 200, 700, 75, a_shader, exitControlsTex))
	{
		showOptions = false;
		playAudio = true;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (playAudio) audio->PlaySample("data/sounds/click.wav", false);
	Game::instance->wasLeftMousePressed = false;
}

void IntroStage::render() 
{
	Matrix44 viewModel;

	if (!arrivedPos1)
	{
		viewModel.translate(introPosition1.x, introPosition1.y, introPosition1.z);
		viewModel.rotate(90.0f * DEG2RAD, Vector3(0, 1, 0));
		Matrix44 camModel = viewModel;

		Vector3 desiredEye = viewModel * Vector3(0, 0.7, 0);

		eye = lerpCamera(Game::instance->camera->eye, desiredEye, 1.0f * Game::instance->elapsed_time);  //Portem la camera fins la posició 1
		center = eye + camModel.rotateVector(camModelRotation);
		up = camModel.rotateVector(Vector3(0, 1, 0));
		if (compareVector(eye, desiredEye, 1e-2f))
		{
			arrivedPos1 = true;  //Quan ha arribat setejem el flag a true
		}
	}

	if (arrivedPos1 && !showScreen)
	{
		SDL_ShowCursor(true);
		/*glDisable(GL_DEPTH_TEST);*/  //ENABLE AND DISABLE FLAGS
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height - 100, 300, 75, a_shader, spaceTex, Vector4(1, 1, 1, 0.7), true); 

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);

	}

	if (arrivedPos1 && showScreen && !arrivedScreen)
	{
		viewModel.translate(screenPosition.x, screenPosition.y, screenPosition.z);
		viewModel.rotate(90.0f * DEG2RAD, Vector3(0, 1, 0));
		Matrix44 camModel = viewModel;

		Vector3 desiredModelRotation = Vector3(0, -0.1, -1);
		Vector3 desiredEye = viewModel * Vector3(0, 0.53, 0);

		camModelRotation = (lerpCamera(camModelRotation, desiredModelRotation, 3.0f * Game::instance->elapsed_time));

		eye = lerpCamera(Game::instance->camera->eye, desiredEye, 1.0f * Game::instance->elapsed_time);
		center = eye + camModel.rotateVector(camModelRotation);  //Mentre movem la camera on la pantalla, rotem la inclinació de la camera
		up = camModel.rotateVector(Vector3(0, 1, 0));

		if (compareVector(camModelRotation, desiredModelRotation, 1e-3))
			finalizedRotation = true;


		if (compareVector(eye, desiredEye, 1e-2f))
		{
			audio->PlaySample("data/sounds/computerStartButton.wav", false);
			arrivedScreen = true;
		}
	}

	if (arrivedScreen)
	{
		if (!soundsOn)
		{
			audio->PlaySample("data/sounds/computerWorking.wav", true);
		
			audio->PlaySample("data/sounds/keyboardTyping.mp3", true);
			soundsOn = true;
		}
		SDL_ShowCursor(true);
		
		if (showControls)
		{
			renderControls();
		}

		if (showOptions)
		{
			renderOptions();
		}

		if(!showControls && !showOptions) renderComputerScreen();
	}
	

	mapArea* currentMapArea = getCurrentArea(currentArea);

	for (int i = 0; i < currentMapArea->prefab.size(); i++)
	{
		currentMapArea->prefab[i]->render();
		//hab1->prefab[i]->mesh->renderBounding(hab1->prefab[i]->prefabModel);

	}

	for (int i = 0; i < currentMapArea->interactive.size(); i++)
	{
		currentMapArea->interactive[i]->render();
	}

	Game::instance->camera->lookAt(eye, center, up); 

}

void IntroStage::update(double seconds_elapsed) 
{
	if (arrivedPos1 && (Input::wasKeyPressed(SDL_SCANCODE_SPACE) || Input::wasButtonPressed(A_BUTTON)))	showScreen = true;
}

GameStage::GameStage() 
{
	//player.hasKey = false;
	entities.clear();

	audio = new Audio();
	ambientOn = false;
	microSounds = false;
	soundCountDown = false;
	initMapAreas();

	a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs"); //TOTS ELS SHADERS IGUALS PER TOTES LES TEXTURES

	prevSelected = NULL;

	initMenuTextures();
	initPinTextures();
	initNoteTextures();

	spaceTex = Texture::Get("data/GUI/spaceButton.png");
	hintTex = Texture::Get("data/GUI/hint.png");
	stoneTex = Texture::Get("data/GUI/stoneFlower.png");
	posterTex = Texture::Get("data/GUI/ThisanddatCartel.png");
	morseCodeTex = Texture::Get("data/GUI/morseCode.png");
	pillsTex = Texture::Get("data/GUI/pills.png");
	noteWardroveStorage = Texture::Get("data/GUI/storageNote.png");
	noteWardroveLivingroom = NULL;

	robot = new IAEntity();
	robot->mapArea = hab1;

	statue = new IAStatue();
	
	player = new CharacterEntity();
	player->currentMapArea = getCurrentArea(currentArea);

	sky = new PrefabEntity();
	sky->mesh = Mesh::Get("data/cielo_noche/box.ase");
	sky->texture = Texture::Get("data/cielo_noche/stars.tga");
	sky->shader = a_shader;

	countDown = 60.0f;
	microPin = 1965;
	doorPin = 3658;  //BEIGE BLUE RED YELLOW
	bed2Pin = 1124;  //PIPERINE PROBIASOR BENAMINE FLUOXETINE
	finalPin = 2152; // S O S

	timesPinStorage = 0;
	timesPinBed2 = 0;
	timesPinFinalDoor = 0;

	inputPin.clear();

	hintStove = false;
	hintMicro = false;
	hintRadioControl = false;
	hintPinStorage = false;
	hintPinBed2 = false;
	hintPinFinalDoor = false;
	hintPinFinalDoor2 = false;

	microHasKey = true;

	isOver = false;
	toMenu = false;

	menuOn = false;
	optionsOn = false;
	pinOn = false;
	pinDesordenaoOn = false;
	graveOn = false;
	posterOn = false;
	mirrorOn = false;
	noteWardroveStorageOn = false;
	noteLivingRoomOn = false;
	pcOn = false;

	isDoorStorage = false;
	isDoorBedroom = false;
	isDoorFinal = false;

	initSongs();
	songID = 0;
	isPlaying = false;
	onLoop = true;
}

eStageID GameStage::getID()
{
	return eStageID::GAME;
}

void GameStage::reset()
{
	entities.clear();

	ambientOn = false;
	microSounds = false;
	soundCountDown = false;

	prevSelected = NULL;

	noteWardroveLivingroom = NULL;

	countDown = 60.0f;

	timesPinStorage = 0;
	timesPinBed2 = 0;
	timesPinFinalDoor = 0;

	inputPin.clear();

	hintStove = false;
	hintMicro = false;
	hintRadioControl = false;
	hintPinStorage = false;
	hintPinBed2 = false;
	hintPinFinalDoor = false;
	hintPinFinalDoor2 = false;

	microHasKey = true;

	isOver = false;
	toMenu = false;

	menuOn = false;
	optionsOn = false;
	pinOn = false;
	pinDesordenaoOn = false;
	graveOn = false;
	posterOn = false;
	mirrorOn = false;
	noteWardroveStorageOn = false;
	noteLivingRoomOn = false;
	pcOn = false;

	isDoorStorage = false;
	isDoorBedroom = false;
	isDoorFinal = false;

	songID = 0;
	isPlaying = false;
	onLoop = true;

	player->reset();


	hab1->interactive.clear();
	LoadPropEditorSceneInteractive("data/Interactive/Interactive.scene", hab1->interactive);
	for (int i = 0; i < hab1->interactive.size(); i++)
	{
		hab1->interactive[i]->reset();
	}
	
}

void GameStage::initMenuTextures()
{
	baseTex = Texture::Get("data/uipack_fixed/PNG/blue_button00.png");
	continueTexture = Texture::Get("data/GUI/button_continue.png");
	saveTexture = Texture::Get("data/GUI/button_save.png");
	loadTexture = Texture::Get("data/GUI/button_load.png");
	optionTexture = Texture::Get("data/GUI/button_options.png");
	exitTexture = Texture::Get("data/GUI/button_exit.png");
	backTexture = Texture::Get("data/GUI/button_back.png");
	brightUp = Texture::Get("data/GUI/brightUp1.png");
	brightDown = Texture::Get("data/GUI/brightDown1.png");
	volumeUp = Texture::Get("data/GUI/volumeUp1.png");
	volumeDown = Texture::Get("data/GUI/volumeDown1.png");
}

void GameStage::initPinTextures()
{
	baseTexPin = Texture::Get("data/GUI/panel.png");
	oneTexture = Texture::Get("data/GUI/buttonOne.png");
	twoTexture = Texture::Get("data/GUI/buttonTwo.png");
	threeTexture = Texture::Get("data/GUI/buttonThree.png");
	fourTexture = Texture::Get("data/GUI/buttonFour.png");
	fiveTexture = Texture::Get("data/GUI/buttonFive.png");
	sixTexture = Texture::Get("data/GUI/buttonSix.png");
	sevenTexture = Texture::Get("data/GUI/buttonSeven.png");
	eightTexture = Texture::Get("data/GUI/buttonEight.png");
	nineTexture = Texture::Get("data/GUI/buttonNine.png");
	zeroTexture = Texture::Get("data/GUI/buttonZero.png");
	outTexture = Texture::Get("data/GUI/buttonOut.png");
}

void GameStage::initNoteTextures()
{
	baseTexNote = Texture::Get("data/GUI/letterMicro.png");
}

void GameStage::initSongs()
{
	songs.push_back("data/sounds/static1.wav");
	songs.push_back("data/sounds/static2.wav");
	songs.push_back("data/sounds/radioSpot.mp3");
	songs.push_back("data/sounds/sos.wav");
	songs.push_back("data/sounds/NGGYU.mp3");
	songs.push_back("data/sounds/radioEmergency.wav");
	songs.push_back("data/sounds/turnOff.wav");
}

void GameStage::render() 
{
	mapArea* currentMapArea = getCurrentArea(currentArea);
	
	if (!ambientOn)
	{
		audio->PlaySample("data/sounds/ambient.wav", true);
		ambientOn = true;
	}

	sky->prefabModel.translate(Game::instance->camera->eye.x, Game::instance->camera->eye.y, Game::instance->camera->eye.z);
	//aixo s'hauria de fer a entities amb renderMesh pero de moment ho deixo per aqui
	if (sky->shader)
	{
		//enable shader
		sky->shader->enable();

		//upload uniforms
		sky->shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		sky->shader->setUniform("u_viewprojection", Game::instance->camera->viewprojection_matrix);
		sky->shader->setUniform("u_texture", sky->texture, 0);
		sky->shader->setUniform("u_model", sky->prefabModel);
		sky->shader->setUniform("u_time", time);

		//do the draw call
		sky->mesh->render(GL_TRIANGLES);

		//disable shader
		sky->shader->disable();
	}
	
	for (int i = 0; i < currentMapArea->prefab.size(); i++)
	{
		currentMapArea->prefab[i]->render();
		//hab1->prefab[i]->mesh->renderBounding(hab1->prefab[i]->prefabModel);

	}

	for (int i = 0; i < currentMapArea->interactive.size(); i++)
	{
		currentMapArea->interactive[i]->render();
		//currentMapArea->interactive[i]->mesh->renderBounding(currentMapArea->interactive[i]->interactiveModel);
	}

	if (player->inBed)
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height - 100, 300, 75, a_shader, spaceTex, Vector4(1, 1, 1, 0.7), true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	if (player->isRobot)  //Quan el jugador es el robot només volem fer el render del robot no ens interesa fer el del player
	{
		robot->render();

		hintMicro = false;
		hintStove = false;
		hintRadioControl = false;
		hintPinBed2 = false;
		hintPinStorage = false;
		hintPinFinalDoor2 = false;
		hintPinFinalDoor = false;

		renderHint();
		drawText(Game::instance->window_width/3 + 50, Game::instance->window_height - 150, "Find something in the garden to break the planks", Vector3(1, 1, 1), 2); //Algun text per ajudar al usuari saber que ha de fer 
	}
	else
	{
		player->render();

		robot->render();
	}

	statue->render();

	if (player->startGas && !player->solvedGas)
	{
		if ((int)countDown > 10) drawText(Game::instance->window_width - 150, 100, std::to_string((int)countDown), Vector3(1, 1, 1), 4);
		else if ((int)countDown <= 10) drawText(Game::instance->window_width - 150, 100, std::to_string((int)countDown), Vector3(1, 0, 0), 4);

	}
	if (menuOn)
	{
		SDL_ShowCursor(true);

		renderMenu();
	}

	if (optionsOn)
	{
		SDL_ShowCursor(true);

		renderOptions();
	}

	player->renderGUIInventario();

	if (player->hasNote)
	{
		SDL_ShowCursor(true);
		renderNote(baseTexNote);
	}

	if (mirrorOn)
	{
		SDL_ShowCursor(true);
		renderMirror(pillsTex);
	}

	if (graveOn)
	{
		SDL_ShowCursor(true);
		renderNote(stoneTex);
	}

	if (posterOn)
	{
		SDL_ShowCursor(true);
		renderPoster(posterTex);
	}

	if (pcOn)
	{
		SDL_ShowCursor(true);
		renderNote(morseCodeTex);
	}

	if (noteWardroveStorageOn)
	{
		SDL_ShowCursor(true);
		renderNote(noteWardroveStorage);
	}

	if (hintStove) {
		hintMicro = false;
		hintPinBed2 = false;
		hintPinStorage = false;
		hintPinFinalDoor2 = false;
		hintPinFinalDoor = false;

		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "Something is wrong with the stove, go check it", Vector3(1, 1, 1), 2);
	}

	if (player->solvedGas && microHasKey && hintMicro)
	{
		hintStove = false;
		hintPinBed2 = false;
		hintPinFinalDoor = false;
		hintPinFinalDoor2 = false;
		hintPinStorage = false;
		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "You should check inside the microwave", Vector3(1, 1, 1), 2);
	}

	if (pinDesordenaoOn)
	{
		SDL_ShowCursor(true);

		renderPinDesordenao();

		if (inputPin.size() == 4)
		{
			if (std::stoi(inputPin) == microPin)
			{
				player->solvedGas = true;
				audio->Stop("data/sounds/gasLeak.wav");
				audio->Stop("data/sounds/microwave.wav");
				audio->Stop("data/sounds/load_page.ogg");
				audio->Stop("data/sounds/heartBeat.wav");
				audio->PlaySample("data/sounds/correct.wav", false);
				audio->PlaySample("data/sounds/breathing.wav", false);
			}
			inputPin.clear();
			if (!player->solvedGas)audio->PlaySample("data/sounds/wrong.mp3", false);
			pinDesordenaoOn = false;
			player->canMove = true;
		}
	}

	if (pinOn)
	{
		SDL_ShowCursor(true);

		renderPin();

		if (inputPin.size() == 4)
		{
			if (std::stoi(inputPin) == doorPin && isDoorStorage)
			{
				audio->PlaySample("data/sounds/correct.wav", false);
				audio->PlaySample("data/sounds/door_opening.wav", false);
				
				if (prevSelected->type == eTypeInteractive::DOOR_PIN)
				{
					prevSelected->hasBeenUsed = true;
				}
				isDoorStorage = false;
			}
			else if (std::stoi(inputPin) != doorPin && isDoorStorage)
			{
				audio->PlaySample("data/sounds/wrong.mp3", false);
				timesPinStorage++;
			}

			else if (std::stoi(inputPin) == bed2Pin && isDoorBedroom)
			{
				audio->PlaySample("data/sounds/correct.wav", false);
				audio->PlaySample("data/sounds/door_opening.wav", false);
				
				if (prevSelected->type == eTypeInteractive::DOOR_PIN)
				{
					prevSelected->hasBeenUsed = true;
				}
				isDoorBedroom = false;
			}
			else if (std::stoi(inputPin) != bed2Pin && isDoorBedroom)
			{
				audio->PlaySample("data/sounds/wrong.mp3", false);
				timesPinBed2++;
			}

			else if (std::stoi(inputPin) == finalPin && isDoorFinal)
			{
				audio->PlaySample("data/sounds/correct.wav", false);
				audio->PlaySample("data/sounds/door_opening.wav", false);
				
				if (prevSelected->type == eTypeInteractive::FRONT_DOOR)
				{
					prevSelected->hasBeenUsed = true;
				}
				isOver = true;
			}
			else if (std::stoi(inputPin) != finalPin && isDoorFinal)
			{
				audio->PlaySample("data/sounds/wrong.mp3", false);
				timesPinFinalDoor++;
			}

			inputPin.clear();
			pinOn = false;
			player->canMove = true;

			if (timesPinStorage == 4)
			{
				hintPinStorage = true;
				audio->PlaySample("data/sounds/appearHint.wav", false);
			}

			if (timesPinBed2 == 4)
			{
				hintPinBed2 = true;
				audio->PlaySample("data/sounds/appearHint.wav", false);
			}

			if (timesPinFinalDoor == 4)
			{
				hintPinFinalDoor = true;
				audio->PlaySample("data/sounds/appearHint.wav", false);
			}

			if (timesPinFinalDoor == 8)
			{
				hintPinFinalDoor2 = true;
				audio->PlaySample("data/sounds/appearHint.wav", false);
			}
		}
	}

	if (hintRadioControl) {
		hintMicro = false;
		hintPinBed2 = false;
		hintPinStorage = false;
		hintPinFinalDoor = false;
		hintPinFinalDoor2 = false;

		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "It's not working... Maybe find something to make it work", Vector3(1, 1, 1), 2);
	}

	if (hintPinStorage)
	{
		hintMicro = false;
		hintPinFinalDoor = false;
		hintStove = false;
		hintPinFinalDoor2 = false;
		hintPinBed2 = false;
		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "You may check the grave", Vector3(1, 1, 1), 2);
	}

	if (hintPinBed2)
	{
		hintMicro = false;
		hintStove = false;
		hintPinFinalDoor = false;
		hintPinFinalDoor2 = false;
		hintPinStorage = false;
		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "Check the closet on the storage room", Vector3(1, 1, 1), 2);
	}


	if (hintPinFinalDoor2)
	{
		hintMicro = false;
		hintStove = false;
		hintPinFinalDoor = false;
		hintPinBed2 = false;
		hintPinStorage = false;
		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "Check the poster on the bedroom. They loved that band", Vector3(1, 1, 1), 2);
	}

	if (hintPinFinalDoor)
	{
		hintMicro = false;
		hintStove = false;
		hintPinFinalDoor2 = false;
		hintPinBed2 = false;
		hintPinStorage = false;
		renderHint();
		drawText(Game::instance->window_width / 3 + 50, Game::instance->window_height - 150, "Radio always gives valuable information", Vector3(1, 1, 1), 2);
	}

}

void GameStage::renderMenu()
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 400, 650, a_shader, baseTex, Vector4(0.5,0.5,0.5,0.5)); //render del quadrat de sota els botons
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 200, 250, 75, a_shader, continueTexture))
	{
		menuOn = false;
		player->canMove = true;
		robot->playerCanMove = player->canMove;
		playAudio = true;
	}//CONTINUE

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 100, 250, 75, a_shader, saveTexture))
	{
		saveGame(player, mapAreas, robot);
		menuOn = false;
		player->canMove = true;  //SEMPRE VOLEM CONTINUAR AMB EL JOC
		playAudio = true;
	}//SAVEGAME

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2, 250, 75, a_shader, loadTexture))
	{
		loadGame(player, mapAreas, robot);
		menuOn = false;
		player->canMove = true; //SEMPRE VOLEM CONTINUAR AMB EL JOC
		playAudio = true;
	}//LOADGAME

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 100, 250, 75, a_shader, optionTexture))
	{
		optionsOn = true;
		menuOn = false;
		playAudio = true;
	}//OPTIONS

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 200, 250, 75, a_shader, exitTexture))
	{
		toMenu = true;
		playAudio = true;
		audio->Stop("data/sounds/microwave.wav");
		audio->Stop("data/sounds/load_page.ogg");
		audio->Stop("data/sounds/heartBeat.wav");
		audio->Stop("data/sounds/gasLeak.wav");
		audio->Stop("data/sounds/ambient.wav");

		audio->PlaySample("data/sounds/computerWorking.wav", true);
		audio->PlaySample("data/sounds/keyboardTyping.mp3", true);


	} //EXIT

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	if (playAudio) audio->PlaySample("data/sounds/click.wav", false);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderOptions()
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;
	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 400, 650, a_shader, baseTex, Vector4(0.5, 0.5, 0.5, 0.5)); //render del quadrat de sota els botons

	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 200, 250, 75, a_shader, brightUp))
	{
		playAudio = true;
		if (Game::instance->brightness < 0.35)
			Game::instance->brightness += 0.05;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 100, 250, 75, a_shader, brightDown))
	{
		playAudio = true;
		if (Game::instance->brightness > 0.15)
			Game::instance->brightness -= 0.05;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2, 250, 75, a_shader, volumeUp))
	{
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 100, 250, 75, a_shader, volumeDown))
	{
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 200, 250, 75, a_shader, backTexture))
	{
		optionsOn = false;
		menuOn = true;
		playAudio = true;
	} //EXIT OPTIONS

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	if (playAudio) audio->PlaySample("data/sounds/click.wav", false);
	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse

}

void GameStage::renderNote(Texture* texture)
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 600, 650, a_shader, texture, Vector4(1, 1, 1, 1)); //render del quadrat de sota els botons

	if (renderButton(200, 75, 75, 75, a_shader, outTexture))
	{
		player->hasNote = false;
		player->canMove = true;
		graveOn = false;
		pcOn = false;
		noteWardroveStorageOn = false;
		menuOn = false;
		optionsOn = false;
		pinDesordenaoOn = false;
		pinOn = false;
		posterOn = false;
		mirrorOn = false;
		noteLivingRoomOn = false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderMirror(Texture* texture)
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 720, 150, a_shader, texture, Vector4(1, 1, 1, 1)); //render del quadrat de sota els botons

	if (renderButton(200, 75, 75, 75, a_shader, outTexture))
	{
		player->canMove = true;
		player->hasNote = false;
		graveOn = false;
		pcOn = false;
		noteWardroveStorageOn = false;
		menuOn = false;
		optionsOn = false;
		pinDesordenaoOn = false;
		pinOn = false;
		posterOn = false;
		mirrorOn = false;
		noteLivingRoomOn = false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderPoster(Texture* texture)
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderQuadMenu(Game::instance->window_width / 2 + 50, Game::instance->window_height / 2, 950, 650, a_shader, texture, Vector4(1, 1, 1, 1)); //render del quadrat de sota els botons

	if (renderButton(200, 75, 75, 75, a_shader, outTexture))
	{
		player->hasNote = false;
		player->canMove = true;
		graveOn = false;
		pcOn = false;
		noteWardroveStorageOn = false;
		menuOn = false;
		optionsOn = false;
		pinDesordenaoOn = false;
		pinOn = false;
		posterOn = false;
		mirrorOn = false;
		noteLivingRoomOn = false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderPin()
{
	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 550, 650, a_shader, baseTexPin, Vector4(1, 1, 1, 1)); //render del quadrat de sota els botons
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 - 150, 75, 75, a_shader, oneTexture))
	{
		inputPin += "1";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 150, 75, 75, a_shader, twoTexture))
	{
		inputPin += "2";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 - 150, 75, 75, a_shader, threeTexture))
	{
		inputPin += "3";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 - 50, 75, 75, a_shader, fourTexture))
	{
		inputPin += "4";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 50, 75, 75, a_shader, fiveTexture))
	{
		inputPin += "5";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 - 50, 75, 75, a_shader, sixTexture))
	{
		inputPin += "6";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 + 50, 75, 75, a_shader, sevenTexture))
	{
		inputPin += "7";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 50, 75, 75, a_shader, eightTexture))
	{
		inputPin += "8";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 + 50, 75, 75, a_shader, nineTexture))
	{
		inputPin += "9";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 150, 75, 75, a_shader, zeroTexture))
	{
		inputPin += "0";
		playAudio = true;
	}

	if (playAudio) audio->PlaySample("data/sounds/pin.wav", false);

	if (renderButton(200, 75, 75, 75, a_shader, outTexture))
	{
		pinOn = false;
		player->canMove = true;
		inputPin.clear();
	}	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderPinDesordenao()
{
		glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bool playAudio = false;

	renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 550, 650, a_shader, baseTexPin, Vector4(1, 1, 1, 1)); //render del quadrat de sota els botons
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 - 150, 75, 75, a_shader, sevenTexture))
	{
		inputPin += "7";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 150, 75, 75, a_shader, twoTexture))
	{
		inputPin += "2";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 - 150, 75, 75, a_shader, fiveTexture))
	{
		inputPin += "5";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 - 50, 75, 75, a_shader, zeroTexture))
	{
		inputPin += "0";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 - 50, 75, 75, a_shader, threeTexture))
	{
		inputPin += "3";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 - 50, 75, 75, a_shader, eightTexture))
	{
		inputPin += "8";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 - 100, Game::instance->window_height / 2 + 50, 75, 75, a_shader, oneTexture))
	{
		inputPin += "1";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 50, 75, 75, a_shader, sixTexture))
	{
		inputPin += "6";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2 + 100, Game::instance->window_height / 2 + 50, 75, 75, a_shader, fourTexture))
	{
		inputPin += "4";
		playAudio = true;
	}
	if (renderButton(Game::instance->window_width / 2, Game::instance->window_height / 2 + 150, 75, 75, a_shader, nineTexture))
	{
		inputPin += "9";
		playAudio = true;
	}

	if (playAudio) audio->PlaySample("data/sounds/pin.wav", false);

	if (renderButton(200, 75, 75, 75, a_shader, outTexture))
	{
		pinOn = false;
		player->canMove = true;
		inputPin.clear();
	}	
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	Game::instance->wasLeftMousePressed = false;  //Treiem el estat del mouse
}

void GameStage::renderHint()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderQuadMenu(Game::instance->window_width / 3, Game::instance->window_height - 150, 50, 50, a_shader, hintTex, Vector4(1, 1, 1, 1), true, true);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void GameStage::update(double seconds_elapsed) 
{
	if (player->isRobot)   //El mateix que en el render
	{
		robot->update(seconds_elapsed);
	}
	else 
	{
		player->update(seconds_elapsed);
		robot->update(seconds_elapsed);
	}

	statue->update(seconds_elapsed, player);

	if (player->isDead)    //Si el player mor 
	{
		pinOn = false;
		Sleep(2000);
		if (loadGame(player, mapAreas, robot))  //Si existeix partida guardada
		{
			loadGame(player, mapAreas, robot);  //Carreguem partida i no esta mort
			player->isDead = false;
			return;
		}
		player->reset();  //Si no reinicialitzem el player
	}

	if (inArea(player->pos, 2.7f, 3.20f, -1.8f, -1.3f))  //Si el jugador entra ala cuina per primer cop es guarda la partida
	{
		if (!player->firstTimeKitchen)
		{
			saveGame(player, mapAreas, robot);
			player->firstTimeKitchen = true;
		}
		if (!player->startGas)
		{
			if (!hintStove) audio->PlaySample("data/sounds/appearHint.wav", false);
			hintStove = true;
		}
		if (player->solvedGas && microHasKey)
		{
			if (!hintMicro) audio->PlaySample("data/sounds/appearHint.wav", false);
			hintMicro = true;
		}
	}

	if (inArea(player->pos, -1.7f, 6.2f, -9.2f, -3.6f)) //Primer cop al jardi guardem
	{
		if (!player->firstTimeGarden)
		{
			saveGame(player, mapAreas, robot);
			player->firstTimeGarden = true;
		}

		player->inGarden = true;
	}
	else
		player->inGarden = false;

	if (inArea(player->pos, 3.6f, 5.2f, -8.6f, -7.5f))  //Primer cop al storage guardem
	{
		if (!player->firstTimeStorage)
		{
			saveGame(player, mapAreas, robot);
			player->firstTimeStorage = true;
			if (prevSelected && prevSelected->type == eTypeInteractive::DOOR_PIN)
			{
				prevSelected->hasBeenUsed = true;
				prevSelected->slam = true;
				audio->PlaySample("data/sounds/slamDoor.wav", false);
			}
		}
		player->inStorage = true;
	}
	else
		player->inStorage = false;
	
	if (Input::wasKeyPressed(SDL_SCANCODE_E) || Input::wasButtonPressed(A_BUTTON)) {
		mapArea* currentMapArea = getCurrentArea(currentArea);
		InteractiveEntity* selected;
		eTypeInteractive selectedType;
		int id;

		std::tie(selected, selectedType, id) = rayCheckInteractive(Game::instance->camera, currentMapArea);
		if (!selected) 
		{
			std::tie(selected, selectedType, id) = rayCheckInteractiveSmall(Game::instance->camera, currentMapArea);
		}
		if (selected)   // meterlo en takeKey?
		{
			if (selectedType == eTypeInteractive::KEY && selected->usedOnce) 
			{
				selected->hasBeenUsed = true;
				selected->usedOnce = false;
				player->takeKey();
				audio->PlaySample("data/sounds/key_taken.wav", false);
			}
			else if (selectedType == eTypeInteractive::POSTER)
			{
				hintPinFinalDoor2 = false;
				player->canMove = false;
				posterOn = true;
				audio->PlaySample("data/sounds/paper.wav", false);
			}
			else if (selectedType == eTypeInteractive::BATTERY && selected->usedOnce)
			{
				audio->PlaySample("data/sounds/takeItem.wav", false);
				player->hasBattery ? player->hasBattery : player->hasBattery = true;
				player->addBattery(selected);
				selected->hasBeenUsed = true;
				selected->usedOnce = false;
			}
			else if (selectedType == eTypeInteractive::DOOR)
			{
				if (!player->hasKey)
					audio->PlaySample("data/sounds/blockedDoor.wav", false);

				else if (player->hasKey && !selected->hasBeenUsed)
				{
					/*currentMapArea->interactive.erase(currentMapArea->interactive.begin() + id);*/
					selected->hasBeenUsed = true;
					player->useKey();
					audio->PlaySample("data/sounds/door_opening.wav", false);
					if (prevSelected && prevSelected->type == eTypeInteractive::DOOR)
					{
						selected->hasBeenUsed = true;
						selected->openAfterSlam = true;
						audio->PlaySample("data/sounds/finalScare.wav", false);
					}
				}

				/*currentMapArea->interactive.push_back(openDoor entity)*/
			}
			else if (selectedType == eTypeInteractive::DOOR_PIN && !selected->hasBeenUsed)
			{
				pinOn = true;
				player->canMove = false;
				prevSelected = selected;

				if (inArea(player->pos, -1.7f, 6.2f, -9.2f, -3.6f)) //Primer cop al jardi guardem
					isDoorStorage = true;
				else
					isDoorBedroom = true;
			}

			else if (selectedType == eTypeInteractive::RC && !selected->hasBeenUsed)  //Si el jugador agafa un RC i te batteries, controla el robot
			{
				if (player->hasBattery)
				{
					hintRadioControl = false;
					selected->hasBeenUsed = true;
					player->isRobot = true;
					robot->isPlayer = true;
					player->batteries.pop_back();
					audio->PlaySample("data/sounds/takeItem.wav", false);
					if (player->batteries.size() == 0) player->hasBattery = false;
				}
				else
				{
					if (!hintMicro) audio->PlaySample("data/sounds/appearHint.wav", false);
					hintRadioControl = true;
				}
			}
			else if (selectedType == eTypeInteractive::STOVE)
			{
				std::cout << "first time stove: " << player->firstTimeStove << std::endl;
				if (!player->firstTimeStove)
				{
					player->firstTimeStove = true;  //ALGO ESTA MALAMENT AIXO PERQUE ENS DEIXA FER LA CINEMATICA SEMPRE I NO VOLEM AIXO
					player->canMove = false;
					audio->PlaySample("data/sounds/gasLeak.wav", true);
					hintStove = false;
				}

			}
			else if (selectedType == eTypeInteractive::MICROWAVE)
			{
				if (player->startGas && !player->solvedGas)
				{
					pinDesordenaoOn = true;
					player->canMove = false;
				}
				
				else if (player->solvedGas)
				{
					if (microHasKey)
					{
						player->takeKey();
						audio->PlaySample("data/sounds/key_taken.wav", false);
						selected->hasKey = false;
						microHasKey = false;
					}
				}
			}
			else if (selectedType == eTypeInteractive::PAPER)
			{
				player->hasNote = true;
				player->canMove = false;
			}

			else if (selectedType == eTypeInteractive::LEVER && selected->usedOnce)
			{
				player->hasLever = true;
				selected->hasBeenUsed = true;
				selected->usedOnce = false;
				player->isRobot = false;
				robot->isPlayer = false;
				audio->PlaySample("data/sounds/takeItem.wav", false);
			}

			else if (selectedType == eTypeInteractive::PLANK && selected->usedOnce)
			{
				if (player->hasLever)
				{
					selected->hasBeenUsed = true;
					selected->usedOnce = false;
					audio->PlaySample("data/sounds/breakPlank.wav", false);
					player->useLever();
					if (player->brokeLever) 
						audio->PlaySample("data/sounds/breakCrowbar.wav", false);
				}
				else 
					audio->PlaySample("data/sounds/woodHit.wav", false);
			}

			else if (selectedType == eTypeInteractive::STONE)
			{
				hintPinStorage = false;
				graveOn = true;
				player->canMove = false;
			}

			else if (selectedType == eTypeInteractive::AXE)
			{
				selected->hasBeenUsed = true;
				player->hasAxe = true;
				audio->PlaySample("data/sounds/takeItem.wav", false);
			}

			else if (selectedType == eTypeInteractive::LOGS)
			{
				if (player->hasAxe)
				{
					player->hasAxe = false;
					audio->PlaySample("data/sounds/logs.wav", false);
					Sleep(2000);
					selected->hasBeenUsed = true;
				}
			}

			else if (selectedType == eTypeInteractive::RADIO)
			{
				hintPinFinalDoor = false;
				onLoop = true;

				if (isPlaying)
				{
					audio->Stop(songs[songID]);
					songID < songs.size() -1 ? songID++ : songID = 0;
					isPlaying = false;
				}

				songID == songs.size() - 1 ? onLoop = false : onLoop;

				const char* song = songs[songID];
				audio->PlaySample(song, onLoop);
				isPlaying = true;
			}

			else if (selectedType == eTypeInteractive::FRONT_DOOR)
			{
				pinOn = true;
				player->canMove = false;
				prevSelected = selected;
				isDoorFinal = true;
			}

			else if (selectedType == eTypeInteractive::MIRROR)
			{
				player->canMove = false;
				mirrorOn = true;
				audio->PlaySample("data/sounds/paper.wav", false);
			}

			else if (selectedType == eTypeInteractive::WARDROBE)
			{
				if (inArea(player->pos, -1.7f, 6.2f, -9.2f, -3.6f))
					noteWardroveStorageOn = true;
				else
					noteLivingRoomOn = true;

				player->canMove = false;
				audio->PlaySample("data/sounds/paper.wav", false);

			}

			else if (selectedType == eTypeInteractive::PC)
			{
				player->canMove = false;
				pcOn = true;
				//audio->PlaySample("data/sounds/paper.wav", false);

			}
		}
	}

	if (player->startGas && !player->solvedGas)
	{
		if (!microSounds) {
			microSounds = true;
			audio->PlaySample("data/sounds/microwaveStart.wav", false);
			audio->PlaySample("data/sounds/microwave.wav", true);
			audio->PlaySample("data/sounds/load_page.ogg", true);
			audio->PlaySample("data/sounds/heartBeat.wav", true);
		}
		
		countDown = countDown - seconds_elapsed;

		if (!soundCountDown && (int)countDown == 10)
		{
			audio->PlaySample("data/sounds/countDown.wav", false);
			soundCountDown = true;
		}

		if ((int)countDown == 0)
		{
			audio->PlaySample("data/sounds/explosion.mp3", false);
			player->isDead = true;
			player->startGas = false;
			player->firstTimeStove = false;
			microSounds = false;
			countDown = 60.0f;
			audio->Stop("data/sounds/microwave.wav");
			audio->Stop("data/sounds/load_page.ogg");
			audio->Stop("data/sounds/heartBeat.wav");
			audio->Stop("data/sounds/gasLeak.wav");
		}
	}

	//if(Input::wasKeyPressed(SDL_SCANCODE_R))
	//{
	//	player->isRobot ? player->isRobot = false : player->isRobot = true;
	//	robot->isPlayer ? robot->isPlayer = false : robot->isPlayer = true;   
	//}

	//if(Input::wasKeyPressed(SDL_SCANCODE_TAB))
	//{
	//	robot->changeView ? robot->changeView = false : robot->changeView = true;

	//}

	//if (Input::wasKeyPressed(SDL_SCANCODE_I))
	//{
	//	player->printInventario();
	//}

	//if (Input::wasKeyPressed(SDL_SCANCODE_G))
	//{
	//	saveGame(player, mapAreas, robot);
	//}

	//if (Input::wasKeyPressed(SDL_SCANCODE_L))
	//{
	//	loadGame(player, mapAreas, robot);
	//}

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE) || Input::wasButtonPressed(START_BUTTON))  //Si apretem ESC es veu el menu 
	{
		if (optionsOn || pinDesordenaoOn || pinOn || graveOn || posterOn ||
			mirrorOn || noteWardroveStorageOn || noteLivingRoomOn || pcOn || player->hasNote)
		{
			optionsOn = false;
			pinOn = false;
			pinDesordenaoOn = false;
			graveOn = false;
			posterOn = false;
			mirrorOn = false;
			noteWardroveStorageOn = false;
			noteLivingRoomOn = false;
			pcOn = false;
			player->hasNote = false;
		}
		else
			menuOn ? menuOn = false : menuOn = true; // Si el menu esta mostrantse senva sino es mostra
		if (menuOn) player->canMove = false;
		if (!menuOn) player->canMove = true;  //Quan el menu es mostra el player no es pot moure
		if (player->isRobot)
		{
			robot->playerCanMove = player->canMove; //El robot no es pot moure si esta controlat per el player i el player no es pot moure (per controlar menu)
			
		}
	}
}

EndStage::EndStage() 
{
	audio = new Audio();
	ambientOn = false;

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	blackTex = Texture::Get("data/GUI/blackTex.png");
	spaceTex = Texture::Get("data/GUI/spaceButton.png");
	endTex = Texture::Get("data/GUI/theEnd.png");

	startPosition = Vector3(-0.5f, 1.0f, 0.8f);;

	pitch = -30.0f;
	yaw = 0.0f;

	arrivedPos1 = false;
	arrivedPos2 = false;
	firstTime = true;
	theEnd = false;
	pressedScape = false;

	loadScene = true;

	audio = NULL;
	ambientOn = false;
}

void EndStage::reset()
{
	ambientOn = false;

	startPosition = Vector3(-0.5f, 1.0f, 0.8f);;

	pitch = -30.0f;
	yaw = 0.0f;

	arrivedPos1 = false;
	arrivedPos2 = false;
	firstTime = true;
	theEnd = false;
	pressedScape = false;

	audio = NULL;
	ambientOn = false;
}

void EndStage::render() 
{
	Matrix44 camModel;
	
	mapArea* currentMapArea = getCurrentArea(currentArea);

	if (loadScene)
	{
		hab1->interactive.clear();
		LoadPropEditorSceneInteractive("data/Interactive/Interactive.scene", hab1->interactive);
		loadScene = false;
	}

	if (!ambientOn)
	{
		audio->PlaySample("data/sounds/ambient.wav", true);
		ambientOn = true;
	}

	for (int i = 0; i < currentMapArea->prefab.size(); i++)
	{
		currentMapArea->prefab[i]->render();
	}

	for (int i = 0; i < currentMapArea->interactive.size(); i++)
	{
		currentMapArea->interactive[i]->render();
	}

	camModel.rotate(pitch * DEG2RAD, Vector3(1, 0, 0));
	eye = startPosition;
	center = eye + camModel.rotateVector(Vector3(0, 0, -1));
	up = camModel.rotateVector(Vector3(0, 1, 0));

	if (!pressedScape)
	{
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height - 100, 300, 75, shader, spaceTex, Vector4(1, 1, 1, 0.7), true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}

	if (!firstTime && !arrivedPos1)
	{
		float desiredPitch = 0.0f;  

		pitch = lerpDoor(pitch, desiredPitch, 3.0f * Game::instance->elapsed_time);

		camModel.rotate(pitch * DEG2RAD, Vector3(1, 0, 0));
		eye = startPosition;
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));

		if (abs(pitch-desiredPitch) < 1e-3)
		{
			pitch = 0.0f;
			arrivedPos1 = true;
			firstTime = true;
			audio->PlaySample("data/sounds/breathing.wav", false);
		}
	}

	if (arrivedPos1)
	{
		float desiredYaw = -90.0f;
		yaw = lerpDoor(yaw, desiredYaw, 2.0f * Game::instance->elapsed_time);
		camModel.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
		eye = startPosition;
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));
		if (abs(yaw - desiredYaw) < 1e-2)
		{
			arrivedPos1 = false;
			arrivedPos2 = true;
		}
	}

	if (arrivedPos2)
	{
		float desiredYaw = 90.0f;
		yaw = lerpDoor(yaw, desiredYaw, 2.0f * Game::instance->elapsed_time);
		camModel.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
		eye = startPosition;
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));
		if (abs(yaw - desiredYaw) < 1e-2)
		{
			arrivedPos2 = false;
			theEnd = true;
			audio->PlaySample("data/sounds/finalScream.wav", false);
			Sleep(4000);
		}
	}

	Game::instance->camera->lookAt(eye, center, up);

	if (theEnd)
	{
		glDisable(GL_CULL_FACE);

		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, Game::instance->window_width, Game::instance->window_height, shader, blackTex, Vector4(1, 1, 1, 1));

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderQuadMenu(Game::instance->window_width / 2, Game::instance->window_height / 2, 720, 150, shader, endTex, Vector4(1, 1, 1, 1));

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		SDL_GL_SwapWindow(Game::instance->window);

		//audio->PlaySample("data/sounds/finalScare.wav", false);
		audio->Stop("data/sounds/ambient.wav");

		Sleep(8000);

		toMenu = true;
	}
}

void EndStage::update(double seconds_elapsed)
{
	if (firstTime && Input::isKeyPressed(SDL_SCANCODE_SPACE))
	{
		firstTime = false;
		pressedScape = true;
	}
}

void renderQuadMenu(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture, Vector4 color, bool animation, bool animateColor)
{
	
	Mesh quad;
	quad.createQuad(x, y, w, h, false);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 2.5);

	if (!a_shader) return;
	//enable shader
	a_shader->enable();

	//upload uniforms

	if (animateColor) {
		color.w = abs(sin(Game::instance->time * 1.5f));
	}

	a_shader->setUniform("u_color", color);
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	a_shader->setUniform("u_texture", a_texture, 0);
	a_shader->setUniform("u_time", time);


	Vector3 halfSize = quad.box.halfsize;

	Matrix44 quadModel;
	if (animation)
	{
		quadModel.translate(0, sin(Game::instance->time*5.0f) * 5, 0);
	}

	a_shader->setUniform("u_model", quadModel);

	quad.render(GL_TRIANGLES);
	//disable shader
	a_shader->disable();
}


bool renderButton(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture)
{
	Vector2 mousePos = Input::mouse_position;
	float halfWidth = w * 0.5;
	float halfHeight = h * 0.5;
	float minx = x - halfWidth;
	float maxx = x + halfWidth;
	float miny = y - halfHeight;
	float maxy = y + halfHeight;

	bool hover = mousePos.x >= minx && mousePos.x <= maxx && mousePos.y >= miny && mousePos.y <= maxy;
	Vector4 color = hover? Vector4(1,1,1,1):Vector4(1,1,1,0.7);

	renderQuadMenu(x, y, w, h, a_shader, a_texture, color);
	return Game::instance->wasLeftMousePressed && hover;
}

bool inArea(Vector3 pos, float minx, float maxx, float minz, float maxz) 
{
	if (pos.x < minx || pos.z < minz) return false;
	if (pos.x > maxx || pos.z > maxz) return false;
	return true;
}

void saveGame(CharacterEntity* player, std::vector<mapArea*> mapAreas, IAEntity* robot)
{
	std::ofstream ofs ("SaveGame.txt", std::ofstream::out);

	// Guardem tota la info del player relevant per després poder cargarla

	ofs << "PLAYER " << player->pos.x << " " << player->pos.y << " " << player->pos.z
		<< " " << player->yaw << " " << player->isRobot << " " << player->hasKey << " " << player->hasFlashlight << " "
		<< player->hasBattery << " " << player->keys.size() << " " << player->batteries.size() << " " << player->inBed << " "
		<< player->canMove << " " << player->solvedGas << " " << player->firstTimeKitchen << " " << player->firstTimeStove << " " << player->firstTimeGarden << " " 
		<< player->firstTimeStorage << " " << player->inGarden << " " << player->inStorage << " " << player->hasLever << " " 
		<< player->leverLife << " " << player->hasAxe << std::endl;

	// Guardem la info dels objectes interactius de cada zona del mapa perque es guardi en el mateix estat que l'hem deixat

	for (int i = 0; i < mapAreas.size(); i++)
	{
		std::vector<InteractiveEntity*> actualRoom = mapAreas[i]->interactive;
		for (int j = 0; j < actualRoom.size(); j++)
		{
			InteractiveEntity* actual = actualRoom[j];
			ofs << "INTERACTIVE_ENTITY " << actual->hasBeenUsed << " " << actual->usedOnce
				<< " " << actual->openAfterSlam << " " << actual->hasKey << " " << actual->slam << std::endl;
		}
	}

	//Guardem la info relevant del robot

	ofs << "ROBOT " << robot->pos.x << " " << robot->pos.y << " " << robot->pos.z << " " << robot->yaw << " " 
		<< robot->isPlayer << " " << robot->changeView << std::endl;

	ofs.close();
}


bool loadGame(CharacterEntity* player, std::vector<mapArea*> mapAreas, IAEntity* robot)
{
	//RESET PLAYER INVENTORY
	player->reset();

	std::ifstream inFile;

	inFile.open("SaveGame.txt");

	if (!inFile)
	{
		std::cout << "Start a new game, save game does not exist!" << std::endl;
		return false;
	}

	std::string info;
	Vector3 pos;
	float yaw;
	bool isRobot;
	bool hasKey;
	bool hasFlashlight;
	bool hasBattery;
	int numKeys;
	int numBatteries;
	bool inBed;
	bool canMove;
	bool solvedGas;
	bool firstTimeKitchen;
	bool firstTimeStove;
	bool firstTimeGarden;
	bool firstTimeStorage;
	bool inGarden;
	bool inStorage;
	bool hasLever;
	int leverLife;
	bool hasAxe;

	bool beenUsed;
	bool usedOnce;
	bool openAfterSlam;
	bool ehasKey;
	bool slam;

	Vector3 robotPos;
	float robotYaw;
	bool isPlayer;
	bool view;


	inFile >> info;
	if (info == "PLAYER")
	{
		inFile >> pos.x;
		inFile >> pos.y;
		inFile >> pos.z;
		inFile >> yaw;
		inFile >> isRobot;
		inFile >> hasKey;
		inFile >> hasFlashlight;
		inFile >> hasBattery;
		inFile >> numKeys;
		inFile >> numBatteries;
		inFile >> inBed;
		inFile >> canMove;
		inFile >> solvedGas;
		inFile >> firstTimeKitchen;
		inFile >> firstTimeStove;
		inFile >> firstTimeGarden;
		inFile >> firstTimeStorage;
		inFile >> inGarden;
		inFile >> inStorage;
		inFile >> hasLever;
		inFile >> leverLife;
		inFile >> hasAxe;
	}


	for (int i = 0; i < mapAreas.size(); i++)
	{
		for (int j = 0; j < mapAreas[i]->interactive.size(); j++)
		{
			InteractiveEntity* actual = mapAreas[i]->interactive[j];
			inFile >> info;
			inFile >> beenUsed;
			inFile >> usedOnce;
			inFile >> openAfterSlam;
			inFile >> ehasKey;
			inFile >> slam;
			actual->hasBeenUsed = beenUsed;
			actual->usedOnce = usedOnce;
			actual->openAfterSlam = openAfterSlam;
			actual->hasKey = ehasKey;
			actual->slam = slam;
		}
	}


	inFile >> info;
	inFile >> robotPos.x;
	inFile >> robotPos.y;
	inFile >> robotPos.z;
	inFile >> robotYaw;
	inFile >> isPlayer;
	inFile >> view;

	inFile.close();

	player->pos = pos;
	player->yaw = yaw;
	player->isRobot = isRobot;
	player->hasKey = hasKey;
	player->hasFlashlight = hasFlashlight;
	player->hasBattery = hasBattery;
	player->inBed = inBed;
	player->canMove = canMove;
	player->solvedGas = solvedGas;
	player->solvedGas ? player->startGas = true : player->startGas = false;
	player->firstTimeKitchen = firstTimeKitchen;
	player->firstTimeStove = firstTimeStove;
	player->firstTimeGarden = firstTimeGarden;
	player->firstTimeStorage = firstTimeStorage;
	player->inGarden = inGarden;
	player->inStorage = inStorage;
	player->hasLever = hasLever;
	player->leverLife = leverLife;
	leverLife == 0 ? player->brokeLever = true : player->brokeLever = false;
	player->hasAxe = hasAxe;

	for (int i = 0; i < numKeys; i++)
	{
		InteractiveEntity* key = new InteractiveEntity();
		key->setType(eTypeInteractive::KEY);
		player->addKey(key);
	}

	for (int i = 0; i < numBatteries; i++)
	{
		InteractiveEntity* battery = new InteractiveEntity();
		battery->setType(eTypeInteractive::BATTERY);
		player->addBattery(battery);
	}

	robot->pos = robotPos;
	robot->yaw = robotYaw;
	robot->isPlayer = isPlayer;
	robot->changeView = view;

	return true;
}
