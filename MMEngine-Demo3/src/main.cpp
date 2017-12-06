//#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/objects/shape_generator_2D.h"
#include "graphics/objects/shape_generator_3D.h"
#include "graphics/shape_renderer.h"
#include "graphics/buffers/buffer.h"

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

	Shape cube = ShapeGenerator3D::makeCube();
	Shape triangle = ShapeGenerator2D::makeTriangle();
	Shape square = ShapeGenerator2D::makeSquare();

	square.updatePos(2.0f, 1.0f, -2.0f);
	cube.updatePos(-1.0f, -1.0f, 0.0f);
	triangle.updatePos(3.0f, 2.0f, -4.0f);

	Shape arr[] = { triangle, square, cube };
	GLsizeiptr buf = triangle.vertexBufferSize() + square.vertexBufferSize() + cube.vertexBufferSize();
	GLsizeiptr idx = triangle.indexBufferSize() + square.indexBufferSize() + cube.vertexBufferSize();

	ShapeRenderer b(arr, 3, buf, idx);

	//Renderable arr2[] = { triangle, square, cube };

	cube.cleanUp();
	triangle.cleanUp();
	square.cleanUp();


	/*
	// Texture Data
	int x, y, n;
	int force_channels = 4;
	const char *img = "./res/bricks.jpg";
	//const char *img = "./res/bball.png";
	unsigned char *img_data = stbi_load(img, &x, &y, &n, force_channels);
	if (!img_data) {
	fprintf(stderr, "ERROR: could not load image data %s\n", img);
	}
	else {
	fprintf(stdout, "Image width %d\nImage height %d\n# of 8 bit components per pixel %d\n", x, y, n);
	}
	if (x & (x - 1) != 0 || y & (y - 1) != 0) {
	fprintf(stderr, "Image %s not a power of two. Could potentially be not supported by older graphics cards.", img);
	}
	// image loaded in upside down most of the time. images difine 0 of y axis at the to left corner.
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;
	for (int row = 0; row < half_height; row++) {
	top = img_data + row * width_in_bytes;
	bottom = img_data + (y - row - 1) * width_in_bytes;
	for (int col = 0; col < width_in_bytes; col++) {
	temp = *top;
	*top = *bottom;
	*bottom = temp;
	top++;
	bottom++;
	}
	}
	// Texture Buffer
	GLuint tex = 0;
	glGenTextures(1, &tex);	// generate texture id (name) used to reference texture
	glActiveTexture(GL_TEXTURE0);	// set active texture slot to be texture 0, by default 0 anyways
	glBindTexture(GL_TEXTURE_2D, tex);	// bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	*/

	// Camera and Shader set up

	int width = window.getWidth();
	int height = window.getHeight();
	//mat4 translate;
	//translate = mat4::translationMatrix(0.0f, 0.0f, 0.0f);

	Camera cam(0.0f, 0.0f, 3.0f);
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

	vec3 ray_world;

	//arr[0].model_matrix = mat4::translationMatrix(0.1f, 0.0f, 0.0f);
	//arr[1].model_matrix = mat4::translationMatrix(0.1f, 0.2f, 0.0f);
	//arr[2].model_matrix = mat4::translationMatrix(0.1f, 0.0f, 0.3f);

	//window.setClearColor(0.0f, 0.4f, 0.8f, 1.0f);

	Renderable *ptr;

	float speed = 0.15f;
	float angle = 1.0f;
	float scale_speed = 0.05f;
	vec3 scale(1.0f, 1.0f, 1.0f);

	while (!window.closed()) {

		ptr = &arr[0];
		ptr->model_matrix.incrementX(speed);

		if (fabs(ptr->model_matrix.columns[3].x) >= 10.0f) {
			speed = -speed;
		}

		ptr = &arr[1];
		scale.x += scale_speed;
		scale.y += scale_speed;
		scale.z += scale_speed;
		ptr->model_matrix.scale(scale.x, scale.y, scale.z);

		if (fabs(scale.x) >= 5.0f) {
			scale_speed = -scale_speed;
		}

		ptr = &arr[2];
		ptr->model_matrix.rotationX(angle);
		angle += 2.0f;

		if (fabs(angle) > 360.0f) {
			angle = 0;
		}

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

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.worldRayVec(window.getX(), window.getY(), width, height);
			std::cout << "mouse world pos " << ray_world << std::endl;
		}

	}

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