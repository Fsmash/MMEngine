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


#define DEBUG 1
#define VERT "src/shaders/light.vert"
#define FRAG "src/shaders/light.frag"

#define VERT_TEX "src/shaders/texture.vert"
#define FRAG_TEX "src/shaders/texture.frag"

#define VERT_INST "src/shaders/instanced.vert"
#define FRAG_BASIC "src/shaders/basic.frag"

#define MESH_FILE1 "res/suzanneTex.obj"
#define MESH_FILE2 "res/SpaceShip.obj"
//#define MESH_FILE2 "res/millenium-falcon.obj" 

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

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::ModelRenderer &shader,
	const char *vert, const char *frag);


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

	//Model model(MESH_FILE);
	//std::cout << "num_indices = " << model.num_indices << std::endl;
	//std::cout << "num_vertices = " << model.num_vertices << std::endl;
	
	Model *model = new Model[3];

	Model monkey1(MESH_FILE1);
	Model monkey2(MESH_FILE1);
	Model ship(MESH_FILE2);

	monkey1.Interleaved();
	monkey1.loadTexture(TEX_FILE);
	monkey2.model_matrix = mat4::translationMatrix(3.0f, 1.0f, 1.0f);
	monkey2.Interleaved();
	monkey2.loadTexture(TEX_FILE);
	monkey2.model_matrix = mat4::translationMatrix(6.0f, 1.0f, 1.0f);
	ship.Interleaved();
	ship.loadTexture(TEX_FILE);
	ship.model_matrix = mat4::translationMatrix(15.0f, -5.0f, 1.0f);
	model[0] = monkey1;
	model[1] = ship;
	model[2] = monkey2;

	GLsizeiptr buf = monkey1.vertexBufferSize() + ship.vertexBufferSize() + monkey2.vertexBufferSize();

	GLsizeiptr idx = monkey1.indexBufferSize() + ship.indexBufferSize() + monkey2.indexBufferSize();

	//ModelRenderer m(model);
	ModelRenderer m;
	m.submit(model, 3, buf, idx);

	monkey1.cleanUp();
	monkey2.cleanUp();
	ship.cleanUp();
	
	/*mat4 *matrices = new mat4[10];
	GLsizeiptr matBuf = sizeof(mat4) * 10;

	for (int i = 0; i < 10; i++) {

		matrices[i] = mat4::translationMatrix(3.0f * (float)i, 1.0f, 1.0f);
	}

	m.submitMat(matrices, 10, 3, matBuf);
	*/

	/*
	for (int i = 0; i < model.num_vertices; i++) {
		std::cout << "pos = " << model.vertices[i].pos << std::endl;
		std::cout << "tex coord = " << model.vertices[i].uv << std::endl;
		std::cout << "normal = " << model.vertices[i].normal << std::endl;
	}*/

	// Camera and Shader set up
	int width = window.getWidth();
	int height = window.getHeight();
	//mat4 translate;
	//translate = mat4::translationMatrix(0.0f, 0.0f, 0.0f);

	Camera cam(12.5f, 0.0f, 15.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(500.0f);
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 

	//Shader shader(VERT, FRAG);
	//shader.enable();
	//shader.setUniformMat4("view", cam.viewMatrix());
	//shader.setUniformMat4("proj", cam.projMatrix(width, height));
	//shader.setUniformMat4("model_matrix", translate);

	
	m.initShader(VERT_TEX, FRAG_TEX);
	m.enableShader();
	m.setUniformMat4("view", cam.viewMatrix());
	m.setUniformMat4("proj", cam.projMatrix(width, height));
	m.disableShader();

	vec3 ray_world;

	window.setClearColor(0.2f, 0.2f, 0.2f, 1.0f);


	while (!window.closed()) {

		window.frameCounter();
		window.update();
		window.clear();		

		//m.flush();
		//m.flushInstanced();
		m.flushDynamic("model_matrix");

		keyPresses(cam, window, m, VERT_TEX, FRAG_TEX);

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.worldRayVec(window.getX(), window.getY(), width, height);
			std::cout << "mouse world pos " << ray_world << std::endl;
		}

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			m.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			m.setUniformMat4("view", cam.viewMatrixUpdate());
		}
	}
	m.clean();
	delete[] model;
	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::ModelRenderer &shader,
	const char *vert, const char *frag) {
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

	if (window.isKeyPressed(GLFW_KEY_R)) {
		if (shader.reloadShader(vert, frag)) {
			int width = window.getWidth();
			int height = window.getHeight();

		
			shader.enableShader();
			cam.setPos(12.5f, 0.0f, 15.0f);
			cam.setOrientation(0.0f, 0.0f, 1.0f, 0.0f);	// vanilla start, 0 degrees about the y axis.
			shader.setUniformMat4("view", cam.viewMatrix());
			shader.setUniformMat4("proj", cam.projMatrix(width, height));
			printf("It Worked!");
		}
		else {
			printf("you done fucked up son");
		}
	}
}