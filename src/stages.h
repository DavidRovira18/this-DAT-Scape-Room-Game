#pragma once
#include "includes.h"
#include "framework.h"
#include "mesh.h"
#include "camera.h"
#include "entities.h"
#include "sonido.h"
#include <fstream>  //Lo he incluido pero seli hauria de preguntar si podem

enum eStageID {
	LOAD,
	INTRO,
	GAME,
	END
};

void initMapAreas();

class Stage 
{
public:

	float t = 0.0f;
	bool isOver;
	bool toMenu;
	bool newGame;

	Stage();

	virtual void reset() {};
	virtual void render();
	virtual void update(double seconds_elapsed) {};
};

class LoadStage : public Stage 
{
public:

	using Stage::Stage;
	int id = 0;

	Audio* audio;
	std::vector<Texture*> loadingScreens;

	Texture* loadingScreen1;
	Texture* loadingScreen2;
	Texture* loadingScreen3;
	Texture* loadingScreen4;
	Texture* blackTex;
	Shader* shader;

	LoadStage();

	eStageID getID();

	void render();
	void update(double seconds_elapsed);
};

class IntroStage : public Stage 
{
public:

	using Stage::Stage;
	int id = 1;

	Vector3 startCameraPosition;
	Vector3 introPosition1;
	Vector3 screenPosition;

	Vector3 eye;
	Vector3 center;
	Vector3 up;

	Vector3 camModelRotation;

	Audio* audio;
	bool soundsOn;

	Shader* a_shader;

	Texture* spaceTex;
	Texture* blackTex;
	Texture* newGameTex;
	Texture* loadTex;
	Texture* controlsTex;
	Texture* optionsTex;
	Texture* exitTex;

	Texture* controlsGUITex;
	Texture* exitControlsTex;

	Texture* brightUpTex;
	Texture* brightDownTex;
	Texture* volumeUpTex;
	Texture* volumeDownTex;

	bool arrivedPos1;
	bool arrivedScreen;
	bool finalizedRotation;

	bool showControls;
	bool showOptions;

	bool showScreen;	

	IntroStage();

	eStageID getID();

	void initIntroTextures();

	void renderComputerScreen();
	void renderControls();
	void renderOptions();
	void render();
	void update(double seconds_elapsed);
};

class GameStage : public Stage 
{
public:

	using Stage::Stage;

	CharacterEntity* player;
	IAStatue* statue;
	int id = 2;
	std::vector<PrefabEntity*> entities;
	Shader* a_shader;
	//MENU TEXTURES
	Texture* baseTex;
	Texture* continueTexture;
	Texture* saveTexture;
	Texture* loadTexture;
	Texture* optionTexture;
	Texture* exitTexture;
	Texture* backTexture;
	Texture* brightUp;
	Texture* brightDown;
	Texture* volumeUp;
	Texture* volumeDown;
	//PIN TEXTURES
	Texture* baseTexPin;
	Texture* oneTexture;
	Texture* twoTexture;
	Texture* threeTexture;
	Texture* fourTexture;
	Texture* fiveTexture;
	Texture* sixTexture;
	Texture* sevenTexture;
	Texture* eightTexture;
	Texture* nineTexture;
	Texture* zeroTexture;
	Texture* outTexture;
	//NOTE TEXTURES
	Texture* baseTexNote;
	//SPACE TEXTURE
	Texture* spaceTex;
	//HINT TEXTURE
	Texture* hintTex;
	Texture* stoneTex;
	Texture* posterTex;
	Texture* morseCodeTex;
	Texture* pillsTex;
	Texture* noteWardroveStorage;
	Texture* noteWardroveLivingroom;

	float countDown;

	int microPin;
	int doorPin;
	int bed2Pin;
	int finalPin;

	int timesPinStorage;
	int timesPinBed2;
	int timesPinFinalDoor;

	bool menuOn;
	bool optionsOn;
	bool pinDesordenaoOn;
	bool pinOn;
	bool graveOn;
	bool posterOn;
	bool mirrorOn;
	bool noteWardroveStorageOn;
	bool noteLivingRoomOn;
	bool pcOn;

	bool isDoorStorage;
	bool isDoorBedroom;
	bool isDoorFinal;

	PrefabEntity* sky;

	InteractiveEntity* prevSelected;

	Audio* audio;
	bool ambientOn;
	bool soundCountDown;
	bool microSounds;
	std::vector <const char*> songs;
	int songID;
	bool isPlaying;
	bool onLoop;

	bool hintStove;
	bool hintMicro;
	bool hintRadioControl;
	bool hintPinStorage;
	bool hintPinBed2;
	bool hintPinFinalDoor;
	bool hintPinFinalDoor2;

	bool microHasKey;

	std::string inputPin; 

	GameStage();

	eStageID getID();

	void reset();
	void initMenuTextures();
	void initPinTextures();
	void initNoteTextures();

	void initSongs();

	void render();
	void renderMenu();
	void renderOptions();
	void renderNote(Texture* texture);
	void renderMirror(Texture* texture);
	void renderPoster(Texture* texture);
	void renderPin();
	void renderPinDesordenao();
	void renderHint();
	void update(double seconds_elapsed);
};

class EndStage : public Stage 
{
public:

	using Stage::Stage;
	int id = 3;

	Audio* audio;

	Texture* blackTex;
	Texture* endTex;
	Texture* spaceTex;
	Shader* shader;

	bool ambientOn;

	Vector3 startPosition;

	Matrix44 camModel;
	Vector3 eye;
	Vector3 center;
	Vector3 up;

	float pitch;
	float yaw;

	bool pressedScape;

	bool firstTime;
	bool arrivedPos1;
	bool arrivedPos2;
	bool theEnd;

	bool loadScene;

	Vector3 camModelRotation;

	EndStage();

	eStageID getID();

	void reset();

	void render();
	void update(double seconds_elapsed);

};

void renderQuadMenu(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture, Vector4 color, bool animation = false, bool animateColor = false);
bool renderButton(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture);

bool inArea(Vector3 pos, float minx, float maxx, float minz, float maxz);

void saveGame(CharacterEntity* player, std::vector<mapArea*> mapAreas, IAEntity* robot);
bool loadGame(CharacterEntity* player, std::vector<mapArea*> mapAreas, IAEntity* robot);
