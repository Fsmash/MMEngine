//#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/objects/shape_generator_2D.h"
#include "graphics/objects/shape_generator_3D.h"
#include "graphics/shape_renderer.h"
#include "graphics/buffers/buffer.h"
#include "physics/physics.h"

#define DEBUG 1
#define VERT "src/shaders/light.vert"
#define FRAG "src/shaders/light.frag"

#define VERT_TEX "src/shaders/texture.vert"
#define FRAG_TEX "src/shaders/texture.frag"

#define VERT_INST "src/shaders/instanced.vert"
#define FRAG_BASIC "src/shaders/basic.frag"

#define PAGE_UP		266
#define PAGE_DOWN	267
#define UP_KEY		265
#define DOWN_KEY	264
#define LEFT_KEY	263
#define RIGHT_KEY	262

#if DEBUG
#include "utility/log.h"
#define INIT_LOG() restart_gl_log()
#define LOG() window.logContextInfo()
#else
#define INIT_LOG() true
#define LOG()
#endif

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::ShapeRenderer &shader,
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
		LOG();
	}
	else {
		system("PAUSE");
		return 1;
	}

	Shape box = ShapeGenerator3D::makeBox();
	Shape cube = ShapeGenerator3D::makeCube();
	Shape cube2 = ShapeGenerator3D::makeCube();
	Shape cube3 = ShapeGenerator3D::makeCube();
	Shape cube4 = ShapeGenerator3D::makeCube();
	Shape cube5 = ShapeGenerator3D::makeCube();
	Shape cube6 = ShapeGenerator3D::makeCube();
	Shape cube7 = ShapeGenerator3D::makeCube();
	Shape cube8 = ShapeGenerator3D::makeCube();
	Shape cube9 = ShapeGenerator3D::makeCube();
	Shape cube10 = ShapeGenerator3D::makeCube();
	Shape cube11 = ShapeGenerator3D::makeCube();
	Shape cube12 = ShapeGenerator3D::makeCube();
	Shape cube13 = ShapeGenerator3D::makeCube();
	Shape cube14 = ShapeGenerator3D::makeCube();

	cube.updateVertices(0.0f, -4.0f, -5.0f);
	cube2.updateVertices(3.0f, 4.0f, -10.0f);
	cube3.updateVertices(-4.0f, 4.0f, -5.0f);
	cube4.updateVertices(-1.0f, 4.0f, -10.0f);
	cube5.updateVertices(-2.0f, -2.0f, -15.0f);
	cube6.updateVertices(-3.0f, 4.0f, -15.0f);
	cube7.updateVertices(-5.0f, 4.0f, -15.0f);
	cube8.updateVertices(6.0f, 4.0f, -10.0f);
	cube9.updateVertices(1.0f, -4.0f, -5.0f);
	cube10.updateVertices(-2.0f, 2.0f, -10.0f);
	cube11.updateVertices(-5.0f, -3.0f, -15.0f);
	cube12.updateVertices(7.0f, 6.0f, -5.0f);
	cube13.updateVertices(-5.0f, 7.0f, -10.0f);
	cube14.updateVertices(-2.0f, 3.0f, -5.0f);
	
	cube.setColor(1.0f, 0.0f, 0.0f);
	cube2.setColor(0.0f, 1.0f, 0.0f);
	cube3.setColor(0.0f, 0.0f, 1.0f);
	cube4.setColor(0.0f, 1.0f, 1.0f);
	cube5.setColor(1.0f, 0.0f, 1.0f);
	cube6.setColor(1.0f, 0.2f, 0.0f);
	cube7.setColor(0.7f, 0.0f, 1.0f);
	cube8.setColor(0.6f, 0.3f, 0.0f);
	cube9.setColor(0.0f, 0.2f, 1.0f);
	cube10.setColor(0.0f, 0.1f, 1.0f);
	cube11.setColor(0.2f, 0.2f, 0.5f);
	cube12.setColor(1.0f, 0.5f, 0.7f);
	cube13.setColor(1.0f, 0.3f, 0.8f);
	cube14.setColor(0.5f, 0.2f, 0.0f);

	cube.vel = vec3(0.0f, -0.03f, 0.0f);
	cube2.vel = vec3(-0.03f, -0.03f, 0.0f);
	cube3.vel = vec3(-0.03f, -0.03f, 0.0f);
	cube4.vel = vec3(0.2f, -0.03f, 0.0f);
	cube5.vel = vec3(-0.05f, -0.01f, 0.0f);
	cube6.vel = vec3(-0.04f, -0.06f, 0.0f);
	cube7.vel = vec3(0.2f, -0.03f, 0.0f);
	cube8.vel = vec3(0.03f, 0.04f, 0.0f);
	cube9.vel = vec3(-0.05f, -0.03f, 0.0f);
	cube10.vel = vec3(0.2f, 0.07f, 0.0f);
	cube11.vel = vec3(-0.03f, 0.03f, 0.0f);
	cube12.vel = vec3(0.04f, -0.01f, 0.0f);
	cube13.vel = vec3(0.05f, -0.02f, 0.0f);
	cube14.vel = vec3(0.06f, -0.03f, 0.0f);

	box.scaleVertices(20.0f, 20.0f, 20.0f);
	
	physics::Plane planes[4];
	math::vec3 vec;
	
	// Bottom plane
	planes[0].normal = math::vec3(0.0f, 1.0f, 0.0f);
	vec = box.vertices[12].pos;
	vec.scale(-1);
	planes[0].dist = vec.dot(math::vec3(0.0f, 1.0f, 0.0f));
	// Top plane
	planes[1].normal = math::vec3(0.0f, -1.0f, 0.0f);
	vec = box.vertices[8].pos;
	vec.scale(-1);
	planes[1].dist = vec.dot(math::vec3(0.0f, -1.0f, 0.0f));
	// Left plane
	planes[2].normal = math::vec3(1.0f, 0.0f, 0.0f);
	vec = box.vertices[0].pos;
	vec.scale(-1);
	planes[2].dist = vec.dot(math::vec3(1.0f, 0.0f, 0.0f));
	// Right plane
	planes[3].normal = math::vec3(-1.0f, 0.0f, 0.0f);
	vec = box.vertices[4].pos;
	vec.scale(-1);
	planes[3].dist = vec.dot(math::vec3(-1.0f, 0.0f, 0.0f));

	Shape arr[] = { cube, cube2, cube3, cube4, cube5, cube6, cube7, cube8, cube9, cube10, cube11, cube12, cube13, cube14 };

	GLsizeiptr buf = 14 * cube.vertexBufferSize();
	GLsizeiptr idx = 14 * cube.indexBufferSize();

	ShapeRenderer a(box);
	ShapeRenderer b(arr, 14, buf, idx);

	// Camera and Shader set up
	int width = window.getWidth();
	int height = window.getHeight();
	
	Camera cam(0.0f, 0.0f, 10.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(500.0f);
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 

	b.initShader(VERT, FRAG);
	b.enableShader();
	b.setUniformMat4("view", cam.viewMatrix());
	b.setUniformMat4("proj", cam.projMatrix(width, height));
	b.disableShader();


	a.initShader(VERT, FRAG);
	a.enableShader();
	a.setUniformMat4("view", cam.viewMatrix());
	a.setUniformMat4("proj", cam.projMatrix(width, height));
	a.setUniform1f("alpha", 0.6f);
	a.disableShader();

	vec3 ray_world;

	float speed = 0.05f;

	physics::Physics game;
	game.submit(arr, 14);


	while (!window.closed()) {

		game.AABB3DCollision();
		game.planeCollision(planes[0]);
		game.planeCollision(planes[1]);
		game.planeCollision(planes[2]);
		game.planeCollision(planes[3]);
		game.applyForces();

		window.frameCounter();
		window.update();
		window.clear();

		b.flushDynamic("model_matrix");
		keyPresses(cam, window, b, VERT, FRAG);

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			b.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			b.setUniformMat4("view", cam.viewMatrixUpdate());
		}

		a.flush();
		keyPresses(cam, window, a, VERT, FRAG);

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			a.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			a.setUniformMat4("view", cam.viewMatrixUpdate());
		}

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.worldRayVec(window.getX(), window.getY(), width, height);
			std::cout << "mouse world pos " << ray_world << std::endl;
		}

	}

	a.clean();
	b.clean();

	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::ShapeRenderer &shader,
	const char *vert, const char * frag) {

	if (window.isKeyPressed(int('W'))) {
		//std::cout << "W is PRESSED" << std::endl;
		cam.forward();
	}

	if (window.isKeyPressed(int('A'))) {
		//std::cout << "A is PRESSED" << std::endl;
		cam.left();
	}

	if (window.isKeyPressed(int('S'))) {
		//std::cout << "S is PRESSED" << std::endl;
		cam.back();
	}

	if (window.isKeyPressed(int('D'))) {
		//std::cout << "D is PRESSED" << std::endl;
		cam.right();
	}

	if (window.isKeyPressed(int('Q'))) {
		cam.tiltLeft();
	}

	if (window.isKeyPressed(int('E'))) {
		cam.tiltRight();
	}

	if (window.isKeyPressed(PAGE_UP)) {
		cam.up();
	}

	if (window.isKeyPressed(PAGE_DOWN)) {
		cam.down();
	}

	if (window.isKeyPressed(UP_KEY)) {
		cam.lookUp();
	}

	if (window.isKeyPressed(DOWN_KEY)) {
		cam.lookDown();
	}

	if (window.isKeyPressed(LEFT_KEY)) {
		cam.turnLeft();
	}

	if (window.isKeyPressed(RIGHT_KEY)) {
		cam.turnRight();
	}

	if (window.isKeyPressed(int('R'))) {
		if (shader.reloadShader(vert, frag)) {

			int width = window.getWidth();
			int height = window.getHeight();

			shader.enableShader();
			cam.setPos(0.0f, 0.0f, 3.0f);
			cam.setOrientation(0.0f, 0.0f, 1.0f, 0.0f);	// vanilla start, 0 degrees about the y axis.
			shader.setUniformMat4("view", cam.viewMatrix());
			shader.setUniformMat4("proj", cam.projMatrix(width, height));
			//shader.setUniformMat4("model_matrix", translate);
			printf("It Worked!");
		}
		else {
			printf("you done fucked up son");
		}
	}
}