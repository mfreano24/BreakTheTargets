#include "Manager.h"

#include "ParticleSystem.h"

using namespace std;

using namespace glm;

vec3 heli_position;
vec3 heli_rotation;
vec3 base_forward, base_right, base_up;
vec3 heli_forward, heli_right, heli_up;
mat4 R;
shared_ptr<MatrixStack> heli_stack;
const float HVEL = 0.5f;

float heli_yaw = 0.0f;
float heli_pitch = 0.0f;
float heli_roll = 0.0f;

shared_ptr<Shape> heli, heli_peripheral, top_rotor, back_rotor;

unsigned int skyboxTex;

vec3 prev_forward; //this is for debugging

double t, dt, prev_t;

float prev_pitch = 0.0f, prev_yaw = 0.0f, prev_roll = 0.0f;

shared_ptr<ParticleSystem> ps;

void PrintHeliVectors() {
	cerr << "========================" << endl;
	cerr << "Heli ROTATION: " << heli_rotation.x << ", " << heli_rotation.y << ", " << heli_rotation.z << endl;
	cerr << "Heli FORWARD:  " << heli_forward.x << ", " << heli_forward.y << ", " << heli_forward.z << endl;
	cerr << "Heli RIGHT:    " << heli_right.x <<  ", " << heli_right.y << ", " << heli_right.z << endl;
	cerr << "Heli UP:       " << heli_up.x <<  ", " << heli_up.y << ", " << heli_up.z << endl;
	cerr << (heli_up.x == 0.0f) << endl;
	cerr << "========================" << endl;
}

void PrintRollPitchYaw() {
	cerr << "========================" << endl;
	cerr << "PITCH: " << heli_pitch << endl;
	cerr << "YAW: " << heli_yaw << endl;
	cerr << "========================" << endl;
}

void PrintRMatrix() {
	cerr << "========================" << endl;
	cerr << "R:" << endl;
	cerr << R[0][0] << " " << R[0][1] << " " << R[0][2] << " " << R[0][3] << "\n" <<
		R[1][0] << " " << R[1][1] << " " << R[1][2] << " " << R[1][3] << "\n" <<
		R[2][0] << " " << R[2][1] << " " << R[2][2] << " " << R[2][3] << "\n" <<
		R[3][0] << " " << R[3][1] << " " << R[3][2] << " " << R[3][3] << "\n";
	cerr << "========================" << endl;
}

void Manager::DebugRKey() {
	//repurposing this to a general "press R to debug" function
	ps->PlayAt(heli_position + 50.0f * heli_forward);
}


#pragma region RotationChecks

void Manager::CalculateTurnAcceleration(float _x, float _y, float _z) {
	
	if (abs(_x) > 0.0f && pitch_mag < 1.0f) {
		pitch_mag += dt;
		if (pitch_mag > 1.0f) {
			pitch_mag = 1.0f;
		}
	}
	else if (_x == 0.0f && pitch_mag > 0.0f) {
		pitch_mag -=  2.0f * dt;
		if (pitch_mag < 0.0f) {
			pitch_mag = 0.0f;
		}
	}

	if (abs(_y) > 0.0f && yaw_mag < 1.0f) {
		yaw_mag += dt;
		if (yaw_mag > 1.0f) {
			yaw_mag = 1.0f;
		}
	}
	else if (_y == 0.0f && yaw_mag > 0.0f) {
		yaw_mag -= 2.0f * dt;
		if (yaw_mag < 0.0f) {
			yaw_mag = 0.0f;
		}
	}

	if (abs(_z) > 0.0f && roll_mag < 1.0f) {
		roll_mag += dt;
		if (roll_mag > 1.0f) {
			roll_mag = 1.0f;
		}
	}
	else if (_z == 0.0f && roll_mag > 0.0f) {
		roll_mag -= 2.0f * dt;
		if (roll_mag < 0.0f) {
			roll_mag = 0.0f;
		}
	}

	//if the current rpy's sign doesnt match prev_rpy's sign, then we can reset the mag to 0 (switch in direction)
	if (_x * prev_pitch < 0.0f) {
		pitch_mag = 0.0f;
	}

	if (_y * prev_yaw < 0.0f) {
		yaw_mag = 0.0f;
	}

	if (_z * prev_roll < 0.0f) {
		roll_mag = 0.0f;
	}

}

void Manager::StorePreviousDirection(float _x, float _y, float _z) {
	if (_x != 0.0f) {
		prev_pitch = _x;
	}

	if (_y != 0.0f) {
		prev_yaw = _y;
	}

	if (_z != 0.0f) {
		prev_roll = _z;
	}
}

void Manager::UpdateRotation(float _x, float _y, float _z) {

	CalculateTurnAcceleration(_x, _y, _z);
	StorePreviousDirection(_x, _y, _z);
	//maintain speed until we hit 0
	//override a lack of input?
	if (_x == 0.0f && pitch_mag > 0.0f) {
		_x = prev_pitch;
	}

	if (_y == 0.0f && yaw_mag > 0.0f) {
		_y = prev_yaw;
	}

	if (_z == 0.0f && roll_mag > 0.0f) {
		_z = prev_roll;
	}

	_x *= pitch_mag;
	_y *= yaw_mag;
	_z *= roll_mag;

	//update the helicopter's total pitch
	heli_pitch += _x;

	//update the helicopter's total yaw
	heli_yaw += _y;

	//update the helicopter's total roll
	heli_roll += _z;

	
	//angle wrapping so they remain in the range 0, 2pi
	if (heli_pitch < 0) {
		heli_pitch += 2 * pi < float >();
	}
	else if (heli_pitch > 2 * pi < float >()) {
		heli_pitch -= 2 * pi < float >();
	}

	if (heli_yaw < 0) {
		heli_yaw += 2 * pi < float >();
	}
	else if (heli_yaw > 2 * pi < float >()) {
		heli_yaw -= 2 * pi < float >(); 
	}

	if (heli_roll < 0) {
		heli_roll += 2 * pi < float >();
	}
	else if (heli_roll > 2 * pi < float >()) {
		heli_roll -= 2 * pi < float >();
	}
	
	//pitch
	heli_forward = normalize(heli_forward * cos(_x) + heli_up * sin(_x));
	heli_up = cross(heli_right, heli_forward);

	//yaw
	heli_right = normalize(heli_right * cos(_y) + heli_forward * sin(_y));
	heli_forward = cross(heli_up, heli_right);

	//roll
	heli_right = normalize(heli_right * cos(_z) + heli_up * sin(_z));
	heli_up = cross(heli_right, heli_forward);

	R[0] = vec4(-heli_right, 0.0);
	R[1] = vec4(heli_up, 0.0);
	R[2] = vec4(heli_forward, 0.0);
	R[3] = vec4(0.0, 0.0, 0.0, 1.0);
}

#pragma endregion

void Manager::DrawHelicopter(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, shared_ptr<Program> prog, double t)
{
	//From A1
	prog->bind();
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);
	heli->draw(prog);
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 0.0f);
	heli_peripheral->draw(prog);

	MV->pushMatrix();
	MV->translate(vec3(0.0, 0.4819, 0.0));
	MV->rotate(5.0 * t, vec3(0.0, 1.0, 0.0));
	MV->translate(vec3(0.0, -0.4819, 0.0));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 1.0f);
	top_rotor->draw(prog);
	MV->popMatrix();

	MV->pushMatrix();
	MV->translate(vec3(0.6228, 0.1179, 0.1365));
	MV->rotate(5.0 * t, vec3(0.0, 0.0, 1.0));
	MV->translate(vec3(-0.6228, -0.1179, -0.1365));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	back_rotor->draw(prog);

	MV->popMatrix();
	prog->unbind();
}

void Manager::init_helicopter(){
    GLSL::checkVersion();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);

	keyPresses[(unsigned)'c'] = 1;
	camera = make_shared<Camera>();

	//initialize helicopter
	heli_position = vec3(100.0f, 25.0f, 0.0f);
	heli_rotation = vec3(0.0f, 0.0f, 0.0f);

	base_forward = vec3(0.0f, 0.0f, 1.0f); //+Z is forward initially
	base_right = vec3(-1.0f, 0.0f, 0.0f); //-X is right initially
	base_up = vec3(0.0f, 1.0f, 0.0f);//+Y is up initially

	heli_forward = vec3(0.0f, 0.0f, 1.0f);
	heli_right = vec3(-1.0f, 0.0f, 0.0f);
	heli_up = vec3(0.0f, 1.0f, 0.0f);

	// Initialize time.
	glfwSetTime(0.0);

	//initialize helicopter
	heli = make_shared<Shape>();
	heli->loadMesh(RESOURCE_DIR + "helicopter_body1.obj");
	heli->init();

	heli_peripheral = make_shared<Shape>();
	heli_peripheral->loadMesh(RESOURCE_DIR + "helicopter_body2.obj");
	heli_peripheral->init();

	top_rotor = make_shared<Shape>();
	top_rotor->loadMesh(RESOURCE_DIR + "helicopter_prop1.obj");
	top_rotor->init();

	back_rotor = make_shared<Shape>();
	back_rotor->loadMesh(RESOURCE_DIR + "helicopter_prop2.obj");
	back_rotor->init();
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	GLSL::checkError(GET_FILE_LINE);

	heli_stack = make_shared<MatrixStack>(); //init this here so we dont do it every frame

	t = 0.0;
	dt = 0.0;
	prev_t = 0.0;


	//particle system init
	ps = make_shared<ParticleSystem>();
}

void Manager::render_helicopter(){
    // Update timers
	prev_t = t;
	t = glfwGetTime();
	dt = t - prev_t;
	
	#pragma region SettingUp
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyPresses[(unsigned)'c'] % 2) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyPresses[(unsigned)'z'] % 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();

	UpdateRotation(deltaXRot, deltaYRot, deltaZRot);

	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();

	//PrintHeliVectors();

	heli_position += HVEL * heli_forward; //move the helicopter

	//set up the camera position from the helicopter
	vec3 camera_position = heli_position - 5.0f * heli_forward + heli_up; //position + F where F is forward. we probably need to predefine F(t=0).
	vec3 camera_target = heli_position + 1.35f * heli_up;
	vec3 camera_up = heli_up;

	vec3 billboard_dir = -(camera_target - camera_position);

	mat4 camera_rot = glm::lookAt(camera_position, camera_target, camera_up);
	MV->multMatrix(camera_rot);

	MV->pushMatrix();

    #pragma endregion

	#pragma region RenderSkybox
	skyboxprog->bind();
	MV->pushMatrix();
	MV->translate(heli_position);
	MV->scale(500.0f);
	
	//MV->rotate(glm::pi<float>(), vec3(1.0, 0.0, 0.0));
	if (keyPresses[(unsigned)'c'] % 2) {
		glDisable(GL_CULL_FACE);
	}
	//glDepthMask(GL_FALSE);
	glUniformMatrix4fv(skyboxprog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(skyboxprog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	glUniform1i(skyboxprog->getUniform("skybox"), 0);
	GLSL::checkError(GET_FILE_LINE);
	skybox->draw(skyboxprog);
	//glDepthMask(GL_TRUE);
	if (keyPresses[(unsigned)'c'] % 2) {
		glEnable(GL_CULL_FACE);
	}
	skyboxprog->unbind();
	MV->popMatrix();
	

	#pragma endregion

	#pragma region RenderMesh
	worldprog->bind();
	MV->pushMatrix();

	MV->translate(glm::vec3(-10.0f,-5.0f,-10.0f));
	MV->scale(100.0f, 25.0f, 100.0f);

	glUniformMatrix4fv(worldprog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(worldprog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(worldprog->getUniform("kd"), 0.5f, 0.5f, 1.0f);

	//cerr << "checking noiseMesh->draw... ";
	noiseMesh->draw(worldprog);
	//cerr << "clear!" << endl;

	MV->popMatrix();
	worldprog->unbind();
	#pragma endregion

	#pragma region DrawHelicopter
	//prog->bind();

	MV->pushMatrix();

	MV->translate(heli_position);
	MV->multMatrix(R);

	//model rotations
	MV->rotate(glm::pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));

	//visual flair rotations
	float deg = pi<float>() / 12.0f;
	float pitch_sign = 0.0f, yaw_sign = 0.0f, roll_sign = 0.0f;
	if (prev_pitch != 0.0f) {
		pitch_sign = prev_pitch / abs(prev_pitch);
	}
	if (prev_yaw != 0.0f) {
		yaw_sign = prev_yaw / abs(prev_yaw);
	}
	if (prev_roll != 0.0f) {
		roll_sign = prev_roll / abs(prev_roll);
	}
	
	
	MV->rotate(pitch_mag * pitch_sign * deg, vec3(0.0f, 0.0f, -1.0f));
	MV->rotate(yaw_mag * yaw_sign * deg, vec3(0.0f, 1.0f, 0.0f));
	MV->rotate(roll_mag * roll_sign * deg, vec3(1.0f, 0.0f, 0.0f));


	DrawHelicopter(P, MV, prog, t);

	MV->popMatrix();

	//prog->unbind();
	#pragma endregion

	
	

	//particle system stepping
	glEnable(GL_PROGRAM_POINT_SIZE);
	// Enable quad creation from sprite
	glEnable(GL_POINT_SPRITE);

	if (ps->isActive) {
		cerr << "Particles Active: dt = " << dt << endl;
		ps->step(dt, P, MV, quadPS, camera_position);
	}

	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);

	MV->pushMatrix();
	MV->translate(heli_position);
	#pragma region DebugLines
	// Setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(glm::value_ptr(P->topMatrix()));

	// Setup the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadMatrixf(glm::value_ptr(MV->topMatrix()));

	// Draw frame
	glLineWidth(2);
	glBegin(GL_LINES);
	//forward vector
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(heli_forward.x, heli_forward.y, heli_forward.z);

	//up vector
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(heli_up.x, heli_up.y, heli_up.z);

	//right vector
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(heli_right.x, heli_right.y, heli_right.z);
	glEnd();

	// Pop modelview matrix
	glPopMatrix();

	// Pop projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	#pragma endregion

	

	MV->popMatrix();

	

	
	GLSL::checkError(GET_FILE_LINE);
}