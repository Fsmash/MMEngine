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

#define MESH_FILE1 "res/suzanne.obj"
#define MESH_FILE2 "res/ship.3ds"
#define MESH_FILE3 "res/ship2.3ds"
#define MESH_FILE4 "res/ship3.3ds"
#define MESH_FILE5 "res/ship4.3ds"

#define TEX_FILE1 "res/stripe.jpg"
#define TEX_FILE2 "res/ship.jpg"
#define TEX_FILE3 "res/ship2.jpg"
#define TEX_FILE4 "res/ship3.jpg"
#define TEX_FILE5 "res/ship4.jpg"

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

	Model model[7];
	Model monkey1(MESH_FILE1);
	Model monkey2(MESH_FILE1);
	Model ship(MESH_FILE2);
	Model ship2(MESH_FILE3);
	Model ship3(MESH_FILE4);
	Model ship4(MESH_FILE5);
	Model ship5(MESH_FILE5);

	monkey1.Interleaved();
	monkey1.loadTexture(TEX_FILE1);
	monkey1.updateVertices(1.0f, 1.0f, 0.0f);
	monkey2.rotateVertices(0.0f, -180.0f, 0.0f);
	monkey2.Interleaved();
	monkey2.loadTexture(TEX_FILE1);
	monkey2.updateVertices(6.0f, 1.0f, 0.0f);
	monkey2.rotateVertices(0.0f, -180.0f, 0.0f);
	
	ship.Interleaved();
	ship.loadTexture(TEX_FILE2);
	ship.scaleVertices(0.05f, 0.05f, 0.05f);
	ship.updateVertices(-55.0f, 11.0f, 1.0f);
	ship.rotateVertices(90.0f, 180.0f, 0.0f);

	ship2.Interleaved();
	ship2.loadTexture(TEX_FILE3);
	ship2.scaleVertices(0.05f, 0.05f, 0.05f);
	ship2.updateVertices(5.0f, 2.0f, 3.0f);
	ship2.rotateVertices(0.0f, 0.0f, 0.0f);

	ship3.Interleaved();
	ship3.loadTexture(TEX_FILE4);
	ship3.scaleVertices(0.05f, 0.05f, 0.05f);
	ship3.updateVertices(10.0f, 10.0f, -10.0f);
	ship3.rotateVertices(-90.0f, 0.0f, 0.0f);

	ship4.Interleaved();
	ship4.loadTexture(TEX_FILE5);
	ship4.scaleVertices(0.05f, 0.05f, 0.05f);
	ship4.updateVertices(2.0f, -10.0f, 10.0f);
	ship4.rotateVertices(-90.0f, 0.0f, 0.0f);

	ship5.Interleaved();
	ship5.loadTexture(TEX_FILE5);
	ship5.scaleVertices(0.05f, 0.05f, 0.05f);
	ship5.updateVertices(20.0f, -14.0f, 1.0f);
	ship5.rotateVertices(-90.0f, 0.0f, 0.0f);
	
	monkey1.vel = vec3(0.0f, 0.0f, 0.04f);
	ship.vel = vec3(0.0f, 0.0f, 0.05f);
	ship2.vel = vec3(0.0f, 0.0f, 0.15f);
	ship3.vel = vec3(0.0f, 0.0f, 0.35f);
	ship4.vel = vec3(0.0f, 0.0f, 0.25f);
	ship5.vel = vec3(0.0f, 0.0f, 0.25f);
	monkey2.vel = vec3(0.0, 0.0, 0.14f);
	
	model[0] = monkey1;
	model[1] = monkey2;
	model[2] = ship;
	model[3] = ship2;
	model[4] = ship3;
	model[5] = ship4;
	model[6] = ship5;

	//std::cout << model[0].texFilePath << std::endl;
	
	GLsizeiptr buf = monkey1.vertexBufferSize() + ship.vertexBufferSize() + ship2.vertexBufferSize() 
		+ ship3.vertexBufferSize() + ship4.vertexBufferSize()+ ship5.vertexBufferSize() + monkey2.vertexBufferSize();
	GLsizeiptr idx = monkey1.indexBufferSize() + ship.indexBufferSize() + ship2.indexBufferSize()
		+ ship3.indexBufferSize() + ship4.indexBufferSize() + ship5.indexBufferSize() + monkey2.indexBufferSize();

	ModelRenderer m;
	m.submit(model, 7, buf, idx);

	Shape triangle = ShapeGenerator2D::makeTriangle();
	ShapeRenderer a(triangle);

	mat4 *matrices = new mat4[1000000];
	GLsizeiptr matBuf = sizeof(mat4) * 1000000;

	float r1;
	float r2;
	float r3;

	for (int i = 0; i < 1000000; i++) {

		r1 = 15.5f * float(i) / 2.0f;
		r2 = 20.5f * float(i) / 4.0f;
		r3 = 35.5f * float(i) / 6.0f;


		matrices[i] = mat4::rotationMatrixZ(r1) * mat4::rotationMatrixY(r2)
			* mat4::rotationMatrixX(r3) * mat4::translationMatrix(0.01f + (r1 / 100.0f), 0.02f + (r2 / 100.f), 0.03f + (r3 / 100.f));
	}

	a.submitMat(matrices, 1000000, 3, matBuf);

	triangle.cleanUp();
	model[0].cleanUp();
	model[1].cleanUp();
	model[2].cleanUp();
	model[3].cleanUp();
	model[4].cleanUp();
	model[5].cleanUp();
	model[6].cleanUp();

	// Camera and Shader set up
	int width = window.getWidth();
	int height = window.getHeight();

	Camera cam(50.0f, 5.0f, 120.0f);
	cam.speed = 0.6f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(800.0f);
	cam.init(35.0f, 0.0f, 1.0f, 0.0f);
	
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

	g.submit(model, 7);

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

		g.applyForces();

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