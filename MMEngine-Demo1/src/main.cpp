//#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/objects/shape_generator_2D.h"
#include "graphics/objects/shape_generator_3D.h"
#include "graphics/shape_renderer.h"
#include "graphics/model_renderer.h"
#include "graphics/buffers/buffer.h"
#include "graphics/objects/model.h"
#include "physics/physics.h"


#define DEBUG 1
#define VERT "src/shaders/light.vert"
#define FRAG "src/shaders/light.frag"

#define VERT_TEX "src/shaders/texture.vert"
#define FRAG_TEX "src/shaders/texture.frag"

#define VERT_INST "src/shaders/instanced.vert"
#define FRAG_BASIC "src/shaders/basic.frag"

#define MESH_FILE1 "res/suzanneTex.obj"
#define MESH_FILE2 "res/SpaceShip.obj"

#define TEX_FILE "res/SpaceShip.png"

#if DEBUG
#include "utility/log.h"
#define INIT_LOG() restart_gl_log()
#define LOG(x, ...) gl_log(x, __VA_ARGS__)
#define INFO() log_gl_params()
#else
#define INIT_LOG() true
#define LOG(x, y)
#define INFO()
#endif

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window);
bool reloadShaders(mme::graphics::Window &window, mme::graphics::ModelRenderer &shader, const char *vert, const char *frag);
bool reloadShaders(mme::graphics::Window &window, mme::graphics::ShapeRenderer &shader, const char *vert, const char *frag);


int main() {

	using namespace mme;
	using namespace graphics;
	using namespace math;

	if (!INIT_LOG()) { // if unable to open and start new log file for every program run, exit
		system("PAUSE");
		return 1;
	}

	Window window("test", 960, 540);

	if (!window.isClosed()) {
		// Log created context info
		LOG("GLFW Version %s\n", glfwGetVersionString());	// log version of glfw starting
		LOG("Renderer: %s\n", glGetString(GL_RENDERER));	// get gc model);
		LOG("OpenGL version supported: %s\n", glGetString(GL_VERSION));	// get opengl version	
		INFO();	//	Get hardware info
	}
	else {
		system("PAUSE");
		return 1;
	}

	Model model[3];
	Model monkey1(MESH_FILE1);
	Model monkey2(MESH_FILE1);
	Model ship(MESH_FILE2);

	monkey1.Interleaved();
	monkey1.loadTexture(TEX_FILE);
	monkey1.updatePos(1.0f, 1.0f, 1.0f);
	monkey2.Interleaved();
	monkey2.loadTexture(TEX_FILE);
	monkey2.updatePos(6.0f, 1.0f, 1.0f);
	ship.Interleaved();
	ship.loadTexture(TEX_FILE);
	ship.updatePos(15.0f, 1.0f, 1.0f);

	monkey1.vel = vec3(0.05, 0.0, 0.0);
	ship.vel = vec3(0.04, 0.01, 0.02);
	monkey2.vel = vec3(0.02, 0.0, 0.01);
	
	model[0] = monkey1;
	model[1] = ship;
	model[2] = monkey2;
	
	GLsizeiptr buf = monkey1.vertexBufferSize() + ship.vertexBufferSize() + monkey2.vertexBufferSize();
	GLsizeiptr idx = monkey1.indexBufferSize() + ship.indexBufferSize() + monkey2.indexBufferSize();

	ModelRenderer m;
	m.submit(model, 3, buf, idx);

	Shape triangle = ShapeGenerator2D::makeTriangle();
	ShapeRenderer a(triangle);

	mat4 *matrices = new mat4[1000000];
	GLsizeiptr matBuf = sizeof(mat4) * 1000000;

	float r1;
	float r2;
	float r3;

	for (int i = 0; i < 1000000; i++) {

		r1 = 10.5f * float(i) / 2.0f;
		r2 = 15.5f * float(i) / 3.0f;
		r3 = 20.5f * float(i) / 4.0f;


		matrices[i] = mat4::rotationMatrixY(r1) * mat4::rotationMatrixZ(r2)
			* mat4::rotationMatrixX(r3) * mat4::translationMatrix(0.01f + (r1 / 100.0f), 0.02f + (r2 / 100.f), 0.03f + (r3 / 100.f));
	}

	a.submitMat(matrices, 1000000, 3, matBuf);

	triangle.cleanUp();
	//model[0].cleanUp();
	//model[1].cleanUp();
	//model[2].cleanUp();

	// Camera and Shader set up
	int width = window.getWidth();
	int height = window.getHeight();

	Camera cam(12.5f, 0.0f, 15.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(500.0f);
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 
	
	m.initShader(VERT_TEX, FRAG_TEX);
	m.enableShader();
	m.setUniformMat4("view", cam.viewMatrix());
	m.setUniformMat4("proj", cam.projMatrix(width, height));
	m.disableShader();

	a.initShader(VERT_INST, FRAG);
	a.enableShader();
	a.setUniformMat4("view", cam.viewMatrix());
	a.setUniformMat4("proj", cam.projMatrix(width, height));
	a.disableShader();

	vec3 ray_world;

	physics::Physics g;

	g.submit(model, 3);

	//window.setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	while (!window.closed()) {

		// General stuff
		window.frameCounter();
		window.update();
		window.clear();	
		
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.worldRayVec(window.getX(), window.getY(), width, height);
			std::cout << "mouse world pos " << ray_world << std::endl;
		}

		//g.applyForces();

		// Model stuff
		m.flushDynamic("model_matrix"); 
		keyPresses(cam, window);

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			m.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			m.setUniformMat4("view", cam.viewMatrixUpdate());
		}

		if (reloadShaders(window, m, VERT_TEX, FRAG_TEX)) {
			cam.setPos(12.5f, 0.0f, 15.0f);
			cam.setOrientation(0.0f, 0.0f, 1.0f, 0.0f);	// vanilla start, 0 degrees about the y axis.
			a.setUniformMat4("view", cam.viewMatrix());
			a.setUniformMat4("proj", cam.projMatrix(width, height));
		}
		
		cam.lookAt(ship.getCenter());
		// Shape stuff
		a.flushInstanced();
		keyPresses(cam, window);

		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			a.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			a.setUniformMat4("view", cam.viewMatrixUpdate());
		}

		if (reloadShaders(window, a, VERT_INST, FRAG)) {
			cam.setPos(12.5f, 0.0f, 15.0f);
			cam.setOrientation(0.0f, 0.0f, 1.0f, 0.0f);	// vanilla start, 0 degrees about the y axis.
			a.setUniformMat4("view", cam.viewMatrix());
			a.setUniformMat4("proj", cam.projMatrix(width, height));
		}

	}

	m.clean();
	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window) {
	//std::cout << "Camera pos: " << cam.getPos() << std::endl;

	if (window.isKeyPressed(GLFW_KEY_W)) {
		//std::cout << "W is PRESSED" << std::endl;
		cam.forward();
	}

	if (window.isKeyPressed(GLFW_KEY_A)) {
		//std::cout << "A is PRESSED" << std::endl;
		cam.left();
	}

	if (window.isKeyPressed(GLFW_KEY_S)) {
		//std::cout << "S is PRESSED" << std::endl;
		cam.back();
	}

	if (window.isKeyPressed(GLFW_KEY_D)) {
		//std::cout << "D is PRESSED" << std::endl;
		cam.right();
	}

	if (window.isKeyPressed(GLFW_KEY_Q)) {
		cam.tiltLeft();
	}

	if (window.isKeyPressed(GLFW_KEY_E)) {
		cam.tiltRight();
	}

	if (window.isKeyPressed(GLFW_KEY_PAGE_UP)) {
		cam.up();
	}

	if (window.isKeyPressed(GLFW_KEY_PAGE_DOWN)) {
		cam.down();
	}

	if (window.isKeyPressed(GLFW_KEY_UP)) {
		cam.lookUp();
	}

	if (window.isKeyPressed(GLFW_KEY_DOWN)) {
		cam.lookDown();
	}

	if (window.isKeyPressed(GLFW_KEY_LEFT)) {
		cam.turnLeft();
	}

	if (window.isKeyPressed(GLFW_KEY_RIGHT)) {
		cam.turnRight();
	}

}

bool reloadShaders(mme::graphics::Window &window, mme::graphics::ModelRenderer &shader, const char *vert, const char *frag) {
	if (window.isKeyPressed(GLFW_KEY_R)) {
		if (shader.reloadShader(vert, frag)) {
			
			int width = window.getWidth();
			int height = window.getHeight();

			shader.enableShader();
			printf("It Worked!");
			return true;
		}

		else {
			printf("you done fucked up son");
			return false;
		}

	}
}

bool reloadShaders(mme::graphics::Window &window, mme::graphics::ShapeRenderer &shader, const char *vert, const char *frag) {
	if (window.isKeyPressed(GLFW_KEY_R)) {
		if (shader.reloadShader(vert, frag)) {

			int width = window.getWidth();
			int height = window.getHeight();

			shader.enableShader();
			printf("It Worked!");
			return true;
		}

		else {
			printf("you done fucked up son");
			return false;
		}

	}
}