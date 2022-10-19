#include "entities.h"

float noRenderDist = 100.0f;


BaseEntity::BaseEntity()
{
	mesh = NULL;
	texture = NULL;
	shader = NULL;
}

LightEntity::LightEntity()
{
	position = Vector3(0.0, 0.0, 0.0);
	color = Vector3(0.0, 0.0, 0.0);
	intensity = 1;
	max_dist = 100;
	light_type = eLightType::POINTLIGHT;
}

void LightEntity::setType(eLightType newType)
{
	this->light_type = newType;
}

void LightEntity::load(const char* path, std::vector<LightEntity*> lights)
{
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	float intensity;
	float max_dist;
	int type;

	while (!ss.eof())
	{
		Vector3 pos = ReadVector3(ss);
		Vector3 color = ReadVector3(ss);

		LightEntity* light = new LightEntity();
		light->position = pos;
		ss >> intensity;
		light->intensity = intensity;
		ss >> max_dist;
		light->max_dist = max_dist;
		ss >> type;
		light->setType((eLightType)type);
		lights.push_back(light);
	}
}

PrefabEntity::PrefabEntity()
{
	shader = NULL;
}

void PrefabEntity::setID(eTypeMesh typeMesh) {
	this->type = typeMesh;
}

void PrefabEntity::render() {

	renderMesh(prefabModel, mesh, texture, shader, Game::instance->camera);
}

InteractiveEntity::InteractiveEntity()
{
	shader = NULL;
	texture = NULL;
	status = false;
	hasBeenUsed = false;
	usedOnce = true;
	openAfterSlam = false;
	doorRotation = 0.0f;
	correctPlace = false;
	hasKey = true;
	slam = false;
}

void InteractiveEntity::reset()
{
	status = false;
	hasBeenUsed = false;
	usedOnce = true;
	openAfterSlam = false;
	doorRotation = 0.0f;
	correctPlace = false;
	hasKey = true;
	slam = false;
}

void InteractiveEntity::setType(eTypeInteractive newType)
{
	this->type = newType;
}

eTypeInteractive InteractiveEntity::getType()
{
	return this->type;
}

bool InteractiveEntity::getStatus()
{
	return status;
}

void InteractiveEntity::openDoor()
{
	if (hasBeenUsed && usedOnce && (type == eTypeInteractive::DOOR || type == eTypeInteractive::DOOR_PIN || type == eTypeInteractive::FRONT_DOOR))
	{
		float desiredRotation = 90.0f;
		float prevRotation = doorRotation;

		if (doorRotation == 0)
			interactiveModel.translate(0.050, 0.000, 0.000);

		doorRotation = lerpDoor(doorRotation, desiredRotation, 3.0f * Game::instance->elapsed_time);  //Animació de la porta utilitzant un lerp de floats

		float newRotation = doorRotation - prevRotation;  //restem perque volem que la suma total sigui 90, sino ens faria com un sumatori de 0 a 90.

		interactiveModel.rotate(newRotation * DEG2RAD, Vector3(0, 1, 0));

		if (abs(doorRotation - desiredRotation) < 1e-3)
		{
			doorRotation = 90.0f;
			usedOnce = false;  //Un cop s'ha acabat la rotació tenim usedOnce a false per el save load game
			correctPlace = true;
		}
	}

 	if (hasBeenUsed && !usedOnce && !correctPlace && (type == eTypeInteractive::DOOR || type == eTypeInteractive::DOOR_PIN || type == eTypeInteractive::FRONT_DOOR))
	{
		if (!compareVector(interactiveModel.getTranslation(), Vector3(0.050, 0.000, 0.000), 1e-3))
		{
			interactiveModel.translate(0.050, 0.000, 0.000);
			interactiveModel.rotate(90.0f * DEG2RAD, Vector3(0, 1, 0));
			correctPlace = true;
		}
	}

	if (hasBeenUsed && !usedOnce && openAfterSlam && (type == eTypeInteractive::DOOR || type == eTypeInteractive::DOOR_PIN || type == eTypeInteractive::FRONT_DOOR))
	{
		float desiredRotation = 90.0f;
		float prevRotation = doorRotation;

		if (doorRotation == 0)
			interactiveModel.translate(0.050, 0.000, 0.000);

		doorRotation = lerpDoor(doorRotation, desiredRotation, 3.0f * Game::instance->elapsed_time);  //Animació de la porta utilitzant un lerp de floats

		float newRotation = doorRotation - prevRotation;  //restem perque volem que la suma total sigui 90, sino ens faria com un sumatori de 0 a 90.

		interactiveModel.rotate(newRotation * DEG2RAD, Vector3(0, 1, 0));

		if (abs(doorRotation - desiredRotation) < 1e-3)
		{
			doorRotation = 90.0f;
			usedOnce = false;  //Un cop s'ha acabat la rotació tenim usedOnce a false per el save load game
			correctPlace = true;
		}
	}
}

void InteractiveEntity::slamDoor()
{
	if (hasBeenUsed && slam && type == eTypeInteractive::DOOR_PIN)
	{
		//float desiredRotation = 0.0f;
		//float prevRotation = doorRotation;

		//if (doorRotation == 90)

		//doorRotation = lerpDoor(doorRotation, desiredRotation, 6.0f * Game::instance->elapsed_time);  //Animació de la porta utilitzant un lerp de floats

		//float newRotation = doorRotation - prevRotation;  //restem perque volem que la suma total sigui 90, sino ens faria com un sumatori de 0 a 90.

		//interactiveModel.rotate(newRotation * DEG2RAD, Vector3(0, 1, 0));

		//if (abs(doorRotation - desiredRotation) < 1e-3)
		//{
		//}
		interactiveModel.translate(-0.050, 0.000, 0.050);
		
		interactiveModel.rotate(-90.0 * DEG2RAD, Vector3(0.0, 1.0, 0.0));
		slam = false;
		hasBeenUsed = false;
		usedOnce = true;
		doorRotation = 0.0;
		setType(eTypeInteractive::DOOR);
	}
}

void InteractiveEntity::render()
{
	if (!hasBeenUsed && usedOnce || type == eTypeInteractive::DOOR || type == eTypeInteractive::DOOR_PIN || type == eTypeInteractive::FRONT_DOOR)
		renderMesh(interactiveModel, mesh, texture, shader, Game::instance->camera);

	openDoor();
	slamDoor();

}

void InteractiveEntity::update(double seconds_elapsed)
{

}

bool InteractiveEntity::changeStatus()
{
	status = status ? false : true;
	return status;
}

CharacterEntity::CharacterEntity()
{
	initialPos = Vector3(-0.5f, -0.2f, 0.8f); //bed position
	pos = modelPlayer * initialPos;
	playerSpeed = 2.5f;
	crouchedSpeed = 1.0f;
	sprintSpeed = 5.0f;
	playerSensibility = 20.0f;
	isCrouched = false;
	inBed = true;
	isWaking = false;
	height = 1.0f;
	crouchedHeight = 0.7f;
	limitSight = 50.0f;
	yaw = 0.0f;

	canMove = false;
	canSprint = true;
	canCrouch = true;

	isRobot = false;
	isDead = false;

	hasKey = false;
	hasFlashlight = false;
	hasBattery = false;

	firstTimeKitchen = false;
	firstTimeStove = false;
	firstTimeGarden = false;
	firstTimeStorage = false;

	inGarden = false;
	inStorage = false;

	startGas = false;
	solvedGas = false;

	hasNote = false;
	note = NULL;

	hasLever = false;
	brokeLever = false;

	hasAxe = false;

	a_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	leverLife = 3;

	initGUIInventario();

	keys.clear();
	batteries.clear();

}

void CharacterEntity::addKey(InteractiveEntity* key) {
	keys.push_back(key);
}

void CharacterEntity::addBattery(InteractiveEntity* battery) 
{
	batteries.push_back(battery);
}

void CharacterEntity::reset()
{
	initialPos = Vector3(-0.5f, -0.2f, 0.8f); //bed position
	pos = modelPlayer * initialPos;
	playerSpeed = 2.5f;
	crouchedSpeed = 1.0f;
	sprintSpeed = 5.0f;
	playerSensibility = 20.0f;
	isCrouched = false;
	inBed = true;
	isWaking = false;
	height = 1.0f;
	crouchedHeight = 0.7f;
	limitSight = 50.0f;
	yaw = 0.0f;

	canMove = false;
	canSprint = true;
	canCrouch = true;

	isRobot = false;
	isDead = false;

	hasKey = false;
	hasFlashlight = false;
	hasBattery = false;

	firstTimeKitchen = false;
	firstTimeStove = false;
	firstTimeGarden = false;
	firstTimeStorage = false;

	inGarden = false;
	inStorage = false;

	startGas = false;
	solvedGas = false;

	hasNote = false;
	note = NULL;

	hasLever = false;
	brokeLever = false;

	hasAxe = false;

	leverLife = 3;

	keys.clear();
	batteries.clear();
}


void CharacterEntity::initGUIInventario()
{
	axeTex = Texture::Get("data/GUI/axe.png");
	keyTex = Texture::Get("data/GUI/passkey.png");
	batTex = Texture::Get("data/GUI/battery.png");
	crowbarTex = Texture::Get("data/GUI/crowbar.png");
}

void CharacterEntity::printInventario() 
{

	std::cout << "Player has keys: " << hasKey << " Number of keys: " << keys.size() << std::endl;

	std::cout << "Player has flashlight: " << hasFlashlight << std::endl;

	std::cout << "Player has batteries: " << hasBattery << " Number of batteries: " << batteries.size() << std::endl;

}

std::string CharacterEntity::getKeys()
{
	std::string inventario;
	if (hasKey)
	{
		inventario = "x" + std::to_string(keys.size());
	}
	return inventario;
}

std::string CharacterEntity::getAxe()
{
	std::string inventario;
	if (hasAxe)
	{
		inventario = "x" + std::to_string(hasAxe);
	}
	return inventario;
}

std::string CharacterEntity::getBatteries()
{
	std::string inventario;
	if (hasBattery)
	{
		inventario = "x" + std::to_string(batteries.size());
	}
	return inventario;
}

std::string CharacterEntity::getLeverLifes()
{
	std::string inventario;
	if (hasLever)
	{
		inventario = std::to_string(leverLife) + " lifes";
	}
	return inventario;
}

void CharacterEntity::renderGUIInventario()
{
	drawText(Game::instance->window_width - 100, Game::instance->window_height - 200, getKeys(), Vector3(1, 1, 1), 2);
	drawText(Game::instance->window_width - 100, Game::instance->window_height - 150, getBatteries(), Vector3(1, 1, 1), 2);
	drawText(Game::instance->window_width - 100, Game::instance->window_height - 100, getLeverLifes(), Vector3(1, 1, 1), 2);
	drawText(Game::instance->window_width - 100, Game::instance->window_height - 50, getAxe(), Vector3(1, 1, 1), 2);

	glDisable(GL_DEPTH_TEST);  //ENABLE AND DISABLE FLAGS
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (hasKey)	renderQuad(Game::instance->window_width - 175, Game::instance->window_height - 200, 40, 40, a_shader, keyTex, Vector4(1, 1, 1, 1));
	if (hasBattery) renderQuad(Game::instance->window_width - 175, Game::instance->window_height - 150, 40, 40, a_shader, batTex, Vector4(1, 1, 1, 1));
	if (hasLever) renderQuad(Game::instance->window_width - 175, Game::instance->window_height - 100, 40, 40, a_shader, crowbarTex, Vector4(1, 1, 1, 1));
	if (hasAxe) renderQuad(Game::instance->window_width - 175, Game::instance->window_height - 50, 40, 40, a_shader, axeTex, Vector4(1, 1, 1, 1));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void CharacterEntity::render() 
{
	Matrix44 modelPlayer;
	modelPlayer.translate(pos.x, pos.y, pos.z);
	modelPlayer.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
	Matrix44 camModel = modelPlayer;

	camModel.rotate(pitch * DEG2RAD, Vector3(1, 0, 0));

	Vector3 eye = modelPlayer * Vector3(0, height, 0);
	Vector3 center = eye + camModel.rotateVector(Vector3(0, 0, -1));
	Vector3 up = camModel.rotateVector(Vector3(0, 1, 0));


	if (isCrouched) {
		eye = modelPlayer * Vector3(0, crouchedHeight, 0);
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));
	}

	if (isWaking)
	{
		Vector3 desiredEye = pos;  //La posicio ala que volem que el player inicii
		desiredEye.y = 1.0f;
		float desiredPitch = 0.0f;  //el pitch que volem

		pitch = lerpDoor(pitch, desiredPitch, 3.0f * Game::instance->elapsed_time); //Transició smooth del pitch
	
		camModel.rotate(pitch * DEG2RAD, Vector3(1, 0, 0));
		eye = lerpCamera(Game::instance->camera->eye, desiredEye, 3.0f * Game::instance->elapsed_time); //Transició smooth del player position
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));

		if (compareVector(eye, desiredEye, 1e-3))
		{
			yaw = 0.0f;
			isWaking = false;
			canMove = true;
		}	
	}
	
	/*if (hasFlashlight)
	{
		playerFlashlight->interactiveModel.translate(pos.x, pos.y, pos.z + 10);
		playerFlashlight->interactiveModel.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
		playerFlashlight->interactiveModel.rotate(pitch * DEG2RAD, Vector3(1, 0, 0));
		renderMesh(playerFlashlight->interactiveModel, playerFlashlight->mesh, playerFlashlight->texture, playerFlashlight->shader, Game::instance->camera);
	}*/

	if (firstTimeStove)
	{
		float desiredYaw = 30.0f;
		yaw = lerpDoor(yaw, desiredYaw, 1.0f * Game::instance->elapsed_time);
		modelPlayer.rotate( yaw * DEG2RAD, Vector3(0, 1, 0));

		Matrix44 camModel = modelPlayer;
		Vector3 desiredEye = Vector3(5.3f, 0.6f, -3.0f); //microwave position
		eye = lerpCamera(Game::instance->camera->eye, desiredEye, 1.0f * Game::instance->elapsed_time);
		center = eye + camModel.rotateVector(Vector3(0, 0, -1));
		up = camModel.rotateVector(Vector3(0, 1, 0));
		if (compareVector(eye, desiredEye, 1e-3))
		{
			canMove = true;
			firstTimeStove = false;
 			startGas = true;
		}
	}
	
	Game::instance->camera->lookAt(eye, center, up);
}

void CharacterEntity::update(double seconds_elapsed) {

	float speed;
	isCrouched = false;
	speed = playerSpeed * seconds_elapsed;
	
	if (inBed) {
		pitch = -50;
		if (Input::isKeyPressed(SDL_SCANCODE_SPACE)) {
			isWaking = true;
			pos = modelPlayer * Vector3(-1.78f, -0.2f, 1.440f);
			inBed = false;
		}
		return;
	}
	
	if (!isWaking) canMove ? pitch = pitch : pitch = 0;

	if (!canMove) return;

	if (canCrouch) {
		if (Input::isKeyPressed(SDL_SCANCODE_LCTRL)) {
			isCrouched = true;
			speed = crouchedSpeed * seconds_elapsed;
		}
	}

	isCrouched ? canSprint = false : canSprint = true;

	if (canSprint) {
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) {
			speed = sprintSpeed * seconds_elapsed;
		}
	}

	pitch -= Input::mouse_delta.y * playerSensibility * seconds_elapsed;

	pitch = pitch > limitSight ? limitSight : pitch;
	pitch = pitch < -limitSight ? -limitSight : pitch;

	yaw -= Input::mouse_delta.x * playerSensibility * seconds_elapsed;
	
	Input::centerMouse();

	Matrix44 playerRotation;
	playerRotation.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
	Vector3 forward = playerRotation.rotateVector(Vector3(0, 0, 1));
	Vector3 side = playerRotation.rotateVector(Vector3(1, 0, 0));

	Vector3 playerMovement;
	if (Input::isKeyPressed(SDL_SCANCODE_W)) playerMovement = playerMovement - (forward * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S)) playerMovement = playerMovement + (forward * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A)) playerMovement = playerMovement - (side * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D)) playerMovement = playerMovement + (side * speed);

	Vector3 nextPosition = pos + playerMovement;
	Vector3 character_center = nextPosition + Vector3(0, 0.5, 0);

	//pos = checkPlayercolisions(pos, nextPosition, currentMapArea, seconds_elapsed);
	pos = checkPlayerColisions(pos, nextPosition, currentMapArea, seconds_elapsed);

}

void CharacterEntity::takeKey()
{
	InteractiveEntity* key = NULL;
	hasKey ? hasKey : hasKey = true;
	addKey(key);
}

void CharacterEntity::useKey()
{
	keys.pop_back();
	if (keys.size() == 0) hasKey = false;
}

void CharacterEntity::useLever()
{
	if (leverLife > 0) leverLife -= 1;
	//SOUND LEVER TRENCAT
	if (leverLife == 0)
	{
		hasLever = false;
		brokeLever = true;
	}
}

IAEntity::IAEntity() {  //Una entity per la IA del nostre joc que serà com una roomba. 

	initialPos = Vector3(0.800f, 0.000f, -2.540f);
	pos = IAModel * initialPos;
	yaw = 90;
	IASpeed = 0.2f;
	controlledSpeed = 1.0f;

	isPlayer = false;  //En un punt del joc si tens un RC podras controlar el robot  
	changeView = false;
	playerCanMove = true;

	mesh = Mesh::Get("data/roomba/robot.obj");
	texture = Texture::Get("data/roomba/robot.png");
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/roomba.fs");

}

void IAEntity::render() {

	Matrix44 IAModel;
	
	IAModel.translate(pos.x, pos.y, pos.z);
	IAModel.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
	IAModel.scale(0.05f, 0.05f, 0.05f);
	
	/*std::cout << "Pos robot x: " << pos.x << "Pos robot z: " << pos.z << std::endl;*/

	if (isPlayer)  //Si el jugador controla el robot
	{
		//pos.y = 0.100f; //Volem veure la camera una mica amunt si el jugador controla el robot

		//if (changeView)
		//{ 	//En el cas que volguem primera persona per el robot 

			Matrix44 camModel = IAModel;

			Vector3 eye = IAModel * Vector3(0, 1, 0);
			eye.y += 0.1;
			Vector3 center = eye + camModel.rotateVector(Vector3(0, -0.2, -1));
			Vector3 up = camModel.rotateVector(Vector3(0, 1, 0));

			Game::instance->camera->lookAt(eye, center, up);

		//}
		
		//else  //En el cas que volguem tercera persona per el robot
		//{
			//Matrix44 camModel = IAModel;

			//Vector3 eye = IAModel * Vector3(0, 4, 4);
			//Vector3 center = eye + camModel.rotateVector(Vector3(0, -0.5, -1));
			//Vector3 up = camModel.rotateVector(Vector3(0, 1, 0));

			//Game::instance->camera->lookAt(eye, center, up);

			//renderMesh(IAModel, mesh, texture, shader, Game::instance->camera);
		//}
		
	}

	else
	{
		renderMesh(IAModel, mesh, texture, shader, Game::instance->camera);  //Si el player no es el robot es renderitza la mesh 
	}
	
}

void IAEntity::update(double seconds_elapsed)
{
	Matrix44 robotRotation;
	Vector3 robotMovement;

	if (isPlayer && playerCanMove)  //Quan és el jugador qui controla el robot, deixem nomes que vaig endevant i que canvii el yaw per generar el moviment
	{
		yaw -= Input::mouse_delta.x * 10.0f * seconds_elapsed; 
		
		Input::centerMouse();
		robotRotation.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = robotRotation.rotateVector(Vector3(0, 0, 1));

		if(Input::isKeyPressed(SDL_SCANCODE_W)) 
			robotMovement = robotMovement - (forward * controlledSpeed * seconds_elapsed);  //Només deixem anar endevant que és realment com funciona un robot d'aquets
	}

	else if(!isPlayer)
	{
		robotRotation.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = robotRotation.rotateVector(Vector3(0, 0, 1));

		robotMovement = robotMovement - (forward * IASpeed * seconds_elapsed);  //Si no controla el jugador sempre va endevant amb la mateixa velocitat
	}
	
	Vector3 nextPos = pos + robotMovement;

	Vector3 newPos;
	float angle;
	std::tie(newPos, angle) = checkRobotColisions(pos, nextPos, mapArea, seconds_elapsed);

	if (newPos.x != nextPos.x || newPos.y != nextPos.y || newPos.z != nextPos.z)
	{
		pos = newPos;
		if (isPlayer)
		{
			return;
		}

		pos.y = 0.000f;

		if (angle * RAD2DEG > 90)
			yaw = yaw + (angle * RAD2DEG * 0.1f); //Sumem el "angle" calculat dintre la funció de colisió del robot al yaw actual d'aquesta manera mai es queda pillat
		else
		{
			yaw = yaw - (angle * RAD2DEG * 0.1f);
		}

		//TO BE VERY IMPROVED BC VERY RUDIMENTARY :S
		return;
	}

	pos = nextPos;

}

IAStatue::IAStatue()
{
	initialPos = Vector3(-0.950f, 0.000f, -7.615f);
	pos = statueModel * initialPos;
	yaw = 0.0f;
	statueSpeed = 1.0f;

	secondPos = false;
	isMoving = false;
	audio->LoadSample("data/sounds/dragStone.mp3", false);

	mesh = Mesh::Get("data/statue/statue.obj");
	texture = Texture::Get("data/statue/statue.png");
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/roomba.fs");
}

void IAStatue::render()
{
	Matrix44 statueModel;
	statueModel.translate(pos.x, pos.y, pos.z);
	statueModel.rotate(yaw * DEG2RAD, Vector3(0, 1, 0));
	statueModel.scale(0.003f, 0.003f, 0.003f);

	side = statueModel.rotateVector(Vector3(1, 0, 0)).normalize();
	forward = statueModel.rotateVector(Vector3(0, 0, -1)).normalize();

	renderMesh(statueModel, mesh, texture, shader, Game::instance->camera);
}

void IAStatue::update(double seconds_elapsed, CharacterEntity* player)
{
	if (!player->inGarden) return;

	player->pos.y = -0.2f;
	Vector3 toTarget = player->pos - pos;
	float dist = toTarget.length();

	toTarget.normalize();

	float sideDot = side.dot(toTarget);
	float forwardDot = forward.dot(toTarget);

	if (forwardDot < 0.98f)
	{
		yaw += 90.0f * sign(sideDot) * seconds_elapsed;
	}

	if (!player->firstTimeStorage) return;

	if (!secondPos)
	{
		pos = Vector3(4.0, 0.0, -5.0);
		secondPos = true;
	}

	if (player->inStorage) return;

	if (dist > 1.5f)
	{
		pos = pos + forward * statueSpeed * seconds_elapsed;
		isMoving = true;
		if (isMoving)
		{
			//audio->PlaySample("data/sounds/dragStone.mp3", false);
			isMoving = false;
		}
	}


}

void renderMesh(Matrix44& model, Mesh* a_mesh, Texture* a_texture, Shader* a_shader, Camera* cam) {
	assert(a_mesh != NULL, "mesh in renderMesh null");
	if (!a_shader) return;
	//enable shader
	a_shader->enable();

	//upload uniforms
	a_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	a_shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	a_shader->setUniform("u_texture", a_texture, 0);
	a_shader->setUniform("u_time", time);
	a_shader->setUniform("u_brightness", Game::instance->brightness);

	Vector3 halfSize = a_mesh->box.halfsize;
	a_shader->setUniform("u_model", model);

	Vector3 meshPos = model.getTranslation();
	Vector3 camPos = cam->eye;
	float dist = meshPos.distance(camPos);
	if (!cam->testSphereInFrustum(meshPos, a_mesh->radius * 7.5)) return;
	if (dist < noRenderDist) a_mesh->render(GL_TRIANGLES);
	//disable shader
	a_shader->disable();

}

Vector3 ReadVector3(std::stringstream& ss)
{
	Vector3 vec;
	ss >> vec.x;
	if (ss.peek() == ',') ss.ignore();
	ss >> vec.y;
	if (ss.peek() == ',') ss.ignore();
	ss >> vec.z;
	return vec;
}

void LoadPropEditorScenePrefab(const char* path, std::vector<PrefabEntity*>& prefabEntities)
{
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	int layer;

	while (!ss.eof())
	{
		std::string meshPath;
		//std::string mesh;
		std::string texturePath;
		ss >> meshPath;
		ss >> texturePath;
		//ss >> texturePath;

		std::cout << "meshPath: " << meshPath << std::endl;

		const char* meshPathChar = &meshPath[0];
		const char* texturePathChar = &texturePath[0];

		Vector3 pos = ReadVector3(ss);
		Vector3 rot = ReadVector3(ss);
		Vector3 scale = ReadVector3(ss);

		PrefabEntity* entity = new PrefabEntity();
		entity->mesh = Mesh::Get(meshPathChar);
		entity->texture = Texture::Get(texturePathChar);
		entity->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/ambient.fs");
		entity->prefabModel.translate(pos.x, pos.y, pos.z);
		entity->prefabModel.rotate((rot.x) * DEG2RAD, Vector3(0, 1, 0));
		entity->prefabModel.rotate(rot.y * DEG2RAD, Vector3(1, 0, 0));
		entity->prefabModel.rotate(rot.z * DEG2RAD, Vector3(0, 0, 1));
		entity->prefabModel.scale(scale.x, scale.y, scale.z);
		prefabEntities.push_back(entity);
		ss >> layer;
	}
}

void LoadPropEditorSceneInteractive(const char* path, std::vector<InteractiveEntity*>& interactive) {
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	int type;

	while (!ss.eof())
	{
		std::string meshPath;
		//std::string mesh;
		std::string texturePath;
		ss >> meshPath;
		ss >> texturePath;
		//ss >> texturePath;

		std::cout << "meshPath: " << meshPath << std::endl;

		const char* meshPathChar = &meshPath[0];
		const char* texturePathChar = &texturePath[0];

		Vector3 pos = ReadVector3(ss);
		Vector3 rot = ReadVector3(ss);
		Vector3 scale = ReadVector3(ss);

		InteractiveEntity* entity = new InteractiveEntity();
		entity->mesh = Mesh::Get(meshPathChar);
		entity->texture = Texture::Get(texturePathChar);
		entity->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/ambient.fs");
		entity->pos = pos;
		entity->interactiveModel.translate(pos.x, pos.y, pos.z);
		entity->interactiveModel.rotate(rot.x * DEG2RAD, Vector3(0, 1, 0));
		entity->interactiveModel.rotate(rot.y * DEG2RAD, Vector3(1, 0, 0));
		entity->interactiveModel.rotate(rot.z * DEG2RAD, Vector3(0, 0, 1));
		entity->interactiveModel.scale(scale.x, scale.y, scale.z);

		ss >> type;
		entity->setType((eTypeInteractive)type);
		interactive.push_back(entity);
	}
}

void LoadLights(const char* path, std::vector<LightEntity*>& lights)
{
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	int type;
	float intensity;
	float maxDist;

	while (!ss.eof())
	{
		Vector3 pos = ReadVector3(ss);
		Vector3 color = ReadVector3(ss);

		LightEntity* entity = new LightEntity();
		entity->position = pos;
		entity->color = color;
		ss >> intensity;
		ss >> maxDist;
		ss >> type;
		entity->intensity;
		entity->max_dist;
		entity->setType((eLightType)type);
		lights.push_back(entity);
	}
}

std::tuple<InteractiveEntity*, eTypeInteractive, int> rayCheckInteractive(Camera* cam, mapArea* map) {
	Vector2 mouse = Input::mouse_position;
	Vector3 dir = (cam->center - cam->eye).normalize();
	Vector3 origin = cam->eye;
	Vector3 lowerDist = Vector3(1000.0f, 1000.0f, 1000.0f);
	InteractiveEntity* selectedEntity = NULL;
	int i = 0;
	for (size_t i = 0; i < map->interactive.size(); i++) {
		InteractiveEntity* entity = map->interactive[i];
		if (entity->type == eTypeInteractive::KEY && entity->type == eTypeInteractive::PAPER && entity->type == eTypeInteractive::LEVER) 
			continue;
		if (entity->hasBeenUsed && entity->type == eTypeInteractive::LOGS) continue;  //Si la interactive entity lhem fet servir ( i per tant no es renderitza en alguns casos) no volem colisionar

		Vector3 pos;
		Vector3 normal;
		if (entity->mesh->testRayCollision(entity->interactiveModel, origin, dir, pos, normal))
		{
			Vector3 dist = origin - pos;
			if (abs(dist.x) > 0.8f || abs(dist.y) > 1.0f || abs(dist.z) > 0.8f) continue;
			if(abs(dist.x) < abs(lowerDist.x) || abs(dist.y) < abs(lowerDist.y) || abs(dist.z) < abs(lowerDist.z))
			{
				lowerDist = dist;
				selectedEntity = entity;  
			}
		}
	}
	if(selectedEntity)	
		return std::make_tuple(selectedEntity,selectedEntity->getType(), i); // return std::make_tuple(selectedEntity, i, type);
	return std::make_tuple(selectedEntity, (eTypeInteractive)0, i);
}

std::tuple<InteractiveEntity*, eTypeInteractive, int> rayCheckInteractiveSmall(Camera* cam, mapArea* map) {
	Vector2 mouse = Input::mouse_position;
	Vector3 dir = (cam->center - cam->eye).normalize();
	Vector3 origin = cam->eye;
	Vector3 lowerDist = Vector3(10.0f, 10.0f, 10.0f);
	InteractiveEntity* selectedEntity = NULL;
	int i;
	for (size_t i = 0; i < map->interactive.size(); i++) {
		InteractiveEntity* entity = map->interactive[i];
		if (!entity->type == eTypeInteractive::KEY && !entity->type == eTypeInteractive::PAPER && !entity->type == eTypeInteractive::LEVER && !entity->type == eTypeInteractive::BATTERY) continue;
		Vector3 pos = entity->pos;
		Vector3 coll;
		float t;
		if (RaySphereCollision(entity->pos, 0.1f, origin, dir, coll, t))
		{
			Vector3 dist = origin - pos;
			if (abs(dist.x) > 0.8f || abs(dist.y) > 1.0f || abs(dist.z) > 0.8f) continue;
			if (abs(dist.x) < abs(lowerDist.x) && abs(dist.y) < abs(lowerDist.y) && abs(dist.z) < abs(lowerDist.z))
			{
				lowerDist = dist;
				selectedEntity = entity;
			}
		}
	}
	
	if(selectedEntity) 
		return std::make_tuple(selectedEntity, selectedEntity->getType(), i); // return std::make_tuple(selectedEntity, i, type);
	return std::make_tuple(selectedEntity, (eTypeInteractive)0, 0);
}


Vector3 checkPlayerColisions(Vector3 pos, Vector3 nextPos, mapArea* currentMapArea, double seconds_elapsed)
{
	//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
	Vector3 character_center = nextPos + Vector3(0, 0.5, 0);

	//para cada objecto de la escena...
	for (size_t i = 0; i < currentMapArea->prefab.size(); i++) {
		Vector3 coll;
		Vector3 collnorm;
		PrefabEntity* currentEntity = currentMapArea->prefab[i];
		if (currentMapArea->prefab[i]->mesh->testSphereCollision(currentEntity->prefabModel, character_center, 0.2f, coll, collnorm) == false)
			continue; //si no colisiona, pasamos al siguiente objeto


		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * seconds_elapsed;  // PONER LA Y A ZERO Y NORMALIZAR DE NUEVO
		nextPos = pos - push_away*0.4; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = -0.2;

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		/*velocity = reflect(velocity, collnorm) * 0.95;*/
	}

	for (size_t i = 0; i < currentMapArea->interactive.size(); i++) {
		Vector3 coll;
		Vector3 collnorm;
		InteractiveEntity* currentEntity = currentMapArea->interactive[i];
		if (currentEntity->hasBeenUsed) continue;  //Si la interactive entity lhem fet servir ( i per tant no es renderitza en alguns casos) no volem colisionar
		if (currentMapArea->interactive[i]->mesh->testSphereCollision(currentEntity->interactiveModel, character_center, 0.2f, coll, collnorm) == false)
			continue; //si no colisiona, pasamos al siguiente objeto


		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * seconds_elapsed;
		nextPos = pos - push_away*0.4; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = -0.2;

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		/*velocity = reflect(velocity, collnorm) * 0.95;*/
	}


	return(nextPos);
}

std::tuple<Vector3, float> checkRobotColisions(Vector3 pos, Vector3 nextPos, mapArea* mapArea, double seconds_elapsed)
{
	//calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
	Vector3 character_center = nextPos + Vector3(0, 0.2, 0);
	float angle;
	//para cada objecto de la escena...
	for (size_t i = 0; i < mapArea->prefab.size(); i++) {
		Vector3 coll;
		Vector3 collnorm;
		PrefabEntity* currentEntity = mapArea->prefab[i];
		if (mapArea->prefab[i]->mesh->testSphereCollision(currentEntity->prefabModel, character_center, 0.12f, coll, collnorm) == false)
			continue; //si no colisiona, pasamos al siguiente objeto
		
		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * seconds_elapsed;
		nextPos = pos - push_away*0.4; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = 0.000f;

		float magColl = sqrt(pow(coll.x, 2) + pow(coll.y, 2) + pow(coll.z, 2));
		float magNorm = sqrt(pow(collnorm.x, 2) + pow(collnorm.y, 2) + pow(collnorm.z, 2));  //Les dos magnituds
		angle = acos(dot(coll, collnorm) / magColl * magNorm);    //Calculem l'angle amb arcos(coll dot collnorm/magnituds) 

		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		/*velocity = reflect(velocity, collnorm) * 0.95;*/
	}

	for (size_t i = 0; i < mapArea->interactive.size(); i++) {
		Vector3 coll;
		Vector3 collnorm;
		InteractiveEntity* currentEntity = mapArea->interactive[i];
		if (mapArea->interactive[i]->mesh->testSphereCollision(currentEntity->interactiveModel, character_center, 0.12f, coll, collnorm) == false)
			continue; //si no colisiona, pasamos al siguiente objeto


		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * seconds_elapsed;
		nextPos = pos - push_away*0.4; //move to previous pos but a little bit further

		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		nextPos.y = 0.000f;

		float magColl = sqrt(pow(coll.x, 2) + pow(coll.y, 2) + pow(coll.z, 2));
		float magNorm = sqrt(pow(collnorm.x, 2) + pow(collnorm.y, 2) + pow(collnorm.z, 2));
		angle = acos(dot(coll, collnorm) / magColl * magNorm);
		//reflejamos el vector velocidad para que de la sensacion de que rebota en la pared
		/*velocity = reflect(velocity, collnorm) * 0.95;*/
	}

	return std::make_tuple(nextPos, angle);
}

Vector3 lerpCamera(Vector3 a, Vector3 b, float t)
{
	Vector3 ab = b - a;
	return a + (ab * t);
}

float lerpDoor(float a, float b, float t)
{
	float ab = b - a;
	return a + (ab * t);
}

float sign(float a)
{
	return a >= 0.0f ? 1.0f : -1.0f;
}

bool compareVector(Vector3 a, Vector3 b, float err) 
{
	if (float(abs(a.x - b.x)) < err && float(abs(a.y - b.y)) < err && float(abs(a.z - b.z)) < err) return true;
	else return false;
}

void renderQuad(int x, int y, int w, int h, Shader* a_shader, Texture* a_texture, Vector4 color)
{

	Mesh quad;
	quad.createQuad(x, y, w, h, false);

	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 2.5);

	if (!a_shader) return;
	//enable shader
	a_shader->enable();

	//upload uniforms
	a_shader->setUniform("u_color", color);
	a_shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	a_shader->setUniform("u_texture", a_texture, 0);
	a_shader->setUniform("u_time", time);


	Vector3 halfSize = quad.box.halfsize;

	a_shader->setUniform("u_model", Matrix44());

	quad.render(GL_TRIANGLES);
	//disable shader
	a_shader->disable();
}