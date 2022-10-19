#pragma once
#include "framework.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "game.h"
#include "input.h"
#include "sonido.h"
#include <bass.h>

class BaseEntity; // FORWARD DECLARATION OF BASE ENTITY
class PrefabEntity;
class InteractiveEntity;

enum eMapArea 
{
	HAB1,
	CORRIDOR
};

enum eTypeMesh 
{
	BED
};

enum eLightType
{
	DIRECTIONAL = 0,
	SPOTLIGHT = 1,
	POINTLIGHT = 2
};

enum eTypeInteractive {
	KEY,
	DOOR,
	POSTER,
	BATTERY,
	RC,
	STOVE,
	MICROWAVE,
	PAPER,
	LEVER,
	PLANK,
	STONE,
	DOOR_PIN,
	AXE,
	LOGS,
	RADIO,
	FRONT_DOOR,
	MIRROR,
	WARDROBE,
	PC
};

struct mapArea {
	BoundingBox limits;
	eMapArea ID;
	std::vector<PrefabEntity*> structure; //Entidades dinamicas  
	std::vector<PrefabEntity*> prefab;  //Entidades no dinamicas
	std::vector<InteractiveEntity*> interactive; //Entidades interactivas
	//Las colisiones se chekean entre no dinamicas vs dinamicas y dinamicas vs dinamicas
};   //Para dividir el mapa en secciones, asi podemos comprobar sobre los objetos de esa zona y no sobre todo el mapa 
//(faltara ver como la inicializamos por cada zona del mapa rollo MapArea bedroom1 limits tal entities(cama, armario, ...))


class BaseEntity
{
public:

	std::string name;
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Matrix44 model;

	BaseEntity();
	virtual void render() {};
	virtual void update(double elapsed_time) {};
};

class LightEntity : public BaseEntity
{
public:
	Vector3 position;
	Vector3 color;
	float intensity;
	float max_dist;
	eLightType light_type;

	LightEntity();
	void setType(eLightType newType);
	void load(const char* path, std::vector<LightEntity*> lights);
};


class PrefabEntity : public BaseEntity  // decor meshes, solo renderiza las meshes i prou, no modifica res
{
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	eTypeMesh type;
	Matrix44 prefabModel;

	PrefabEntity();
	void setID(eTypeMesh type);
	void render();
};

class InteractiveEntity : public BaseEntity
{
public:

	bool status;
	bool hasBeenUsed;
	bool usedOnce;
	bool correctPlace;
	bool openAfterSlam;
	bool hasKey;
	bool slam;
	bool finalDoorOpened;

	float doorRotation;

	Vector3 pos;

	eTypeInteractive type;

	Shader* shader;
	Texture* texture;
	Matrix44 interactiveModel;

	InteractiveEntity();

	void reset();

	void setType(eTypeInteractive type);
	eTypeInteractive getType();
	bool getStatus();
	bool changeStatus();

	void openDoor();
	void slamDoor();

	void render();
	void update(double seconds_elapsed);
};

class CharacterEntity : public BaseEntity
{
public:

	Matrix44 modelPlayer;
	Vector3 pos;
	Vector3 initialPos;
	float yaw;
	float pitch;
	float playerSpeed;
	float crouchedSpeed;
	float sprintSpeed;
	float playerSensibility;
	float height;
	float crouchedHeight;
	float limitSight;
	mapArea* currentMapArea;

	InteractiveEntity* playerFlashlight;
	bool canMove;
	bool canSprint;
	bool canCrouch;

	bool isRobot;
	bool isDead;

	bool inBed;
	bool isWaking;
	bool isCrouched;

	bool firstTimeKitchen;
	bool firstTimeStove;
	bool firstTimeGarden;
	bool firstTimeStorage;

	bool inGarden;
	bool inStorage;

	bool startGas;
	bool solvedGas;

	bool hasNote;
	InteractiveEntity* note;

	//Inventario
	Shader* a_shader;
	Texture* axeTex;
	Texture* keyTex;
	Texture* batTex;
	Texture* crowbarTex;
	bool hasKey;
	std::vector<InteractiveEntity*> keys;

	bool hasLever;
	bool brokeLever;
	int leverLife;

	bool hasFlashlight;
	bool hasBattery;
	std::vector<InteractiveEntity*> batteries;

	bool hasAxe;

	CharacterEntity();

	void addKey(InteractiveEntity* key);
	void addBattery(InteractiveEntity* battery);

	void reset(); //Tornem al player als valors inicials

	void takeKey();
	void useKey();
	void useLever();

	void initGUIInventario();
	void printInventario(); //TO DEBUG
	std::string getKeys();
	std::string getAxe();
	std::string getBatteries();
	std::string getLeverLifes();
	void renderGUIInventario();

	void render();
	void update(double seconds_elapsed);
};

class IAEntity :public BaseEntity 
{
public:
	Matrix44 IAModel;
	Vector3 pos;
	Vector3 initialPos;

	float yaw;
	float IASpeed;
	float controlledSpeed;

	mapArea* mapArea;

	bool isPlayer;
	bool playerCanMove;
	bool changeView;

	IAEntity();

	void render();
	void update(double seconds_elapsed);
};

class IAStatue : public BaseEntity
{
public:
	Matrix44 statueModel;
	Vector3 pos;
	Vector3 initialPos;

	Vector3 side;
	Vector3 forward;

	Audio* audio;
	bool isMoving;

	bool secondPos;

	float yaw;
	float statueSpeed;

	IAStatue();

	void render();
	void update(double seconds_elapsed, CharacterEntity* player);
};

void renderMesh(Matrix44& model, Mesh* a_mesh, Texture* a_texture, Shader* a_shader, Camera* cam);
void renderMeshFloor(Matrix44& model, Mesh* a_mesh, Texture* a_texture, Shader* a_shader, Camera* cam);

Vector3 ReadVector3(std::stringstream& ss);

void LoadPropEditorScenePrefab(const char* path, std::vector<PrefabEntity*>& prefabEntities);
void LoadPropEditorSceneInteractive(const char* path, std::vector<InteractiveEntity*>& interactive);
void LoadLights(const char* path, std::vector<LightEntity*>& lights);

std::tuple<InteractiveEntity*,eTypeInteractive, int> rayCheckInteractive(Camera* cam, mapArea* map);
std::tuple<InteractiveEntity*,eTypeInteractive, int> rayCheckInteractiveSmall(Camera* cam, mapArea* map);

Vector3 checkPlayerColisions(Vector3 pos, Vector3 nextPos, mapArea* currentMapArea, double seconds_elapsed);
std::tuple<Vector3,float> checkRobotColisions(Vector3 pos, Vector3 nextPos, mapArea* currentMapArea, double seconds_elapsed);

Vector3 lerpCamera(Vector3 a, Vector3 b , float t);
float lerpDoor(float a, float b, float t);

float sign(float a);

bool compareVector(Vector3 a, Vector3 b, float err);

void renderQuad(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture, Vector4 color);
