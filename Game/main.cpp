//#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/objects/shape_generator_2D.h"
#include "graphics/objects/shape_generator_3D.h"
#include "graphics/shape_renderer.h"
#include "graphics/buffers/buffer.h"
#include "graphics/buffers/index_buffer.h"

#define DEBUG 1
#define VERT "shaders/light.vert"
#define FRAG "shaders/light.frag"

#define VERT_TEX "shaders/texture.vert"
#define FRAG_TEX "shaders/texture.frag"

#define VERT_INST "shaders/instanced.vert"
#define FRAG_BASIC "shaders/basic.frag"

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

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::Shader &shader);

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
	
	Shape triangle = ShapeGenerator2D::makeTriangle();
	Shape square = ShapeGenerator2D::makeSquare();
	Shape cube = ShapeGenerator3D::makeCube();

	//square.updatePos(-1.0f, -1.0f, 0.0f);
	//cube.updatePos(0.0f, -1.0f, -2.0f);
	//triangle.updatePos(0.0f, 1.0f, -1.0f);
	
	//GLuint bufferSize = triangle.vertexBufferSize() + square.vertexBufferSize() + 7 * cube.vertexBufferSize();
	//GLuint ibufferSize = triangle.indexBufferSize() + square.indexBufferSize() + 7 * cube.indexBufferSize();

	//Shape shapes[] = { cube, triangle };
	//Shape shapes2[] = { square };

	//ShapeRenderer a(shapes, 7, bufferSize, ibufferSize);
	//ShapeRenderer b(shapes2, 2, bufferSize, ibufferSize);
	ShapeRenderer c(cube);
	
	/*
	mat4 matrix1;
	mat4 matrix2;
	mat4 matrix3;
	mat4 matrix4;
	mat4 matrix5;
	mat4 matrix6;
	mat4 matrix7;

	matrix1 = mat4::translationMatrix(0.0f, -1.0f, -2.0f);
	matrix2 = mat4::translationMatrix(0.5f, -1.0f, -2.5f);
	matrix3 = mat4::translationMatrix(1.0f, -1.0f, -3.0f);
	matrix4 = mat4::translationMatrix(1.5f, -1.0f, -3.5f);
	matrix5 = mat4::translationMatrix(2.0f, -1.0f, -4.0f);
	matrix6 = mat4::translationMatrix(2.5f, -1.0f, -4.5f);
	matrix7 = mat4::translationMatrix(3.0f, -1.0f, -5.0f);

	GLsizeiptr matBuf = sizeof(mat4) * 7;
	*/
	//mat4 matrices[] = { matrix1, matrix2, matrix3, matrix4, matrix5, matrix6, matrix7 };
	mat4 *matrices = new mat4[1000];
	GLsizeiptr matBuf = sizeof(mat4) * 1000;
	float r1;
	float r2;
	float r3;

	for (int i = 0; i < 1000; i++) {

		r1 = 50.0f * float(i) / 4.0f;
		r2 = 50.0f * float(i) / 5.0f;
		r3 = 50.0f * float(i) / 6.0f;


		matrices[i] = mat4::rotationMatrixX(r1) * mat4::rotationMatrixY(r2)
			* mat4::rotationMatrixZ(r3) * mat4::translationMatrix(0.01f + (r1 / 100.0f), 0.04f + (r2 / 100.f), 0.05f + (r3 / 100.f));
	}

	//a.submitMat(matrices, 1000, 3, matBuf);
	//b.submitMat(matrices, 1000, 3, matBuf);
	c.submitMat(matrices, 1000, 3, matBuf);


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
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 
	
	Shader shader(VERT_INST, FRAG);
	shader.enable();
	shader.setUniformMat4("view", cam.viewMatrix());
	shader.setUniformMat4("proj", cam.projMatrix(width, height));
	//shader.setUniformMat4("model_matrix", translate);

	vec3 ray_world;

	//window.setClearColor(0.0f, 0.4f, 0.8f, 1.0f);

	while (!window.closed()) {

		window.frameCounter();
		window.clear();
		
		//a.flush();
		//b.flush();
		//c.flush();
		//a.flushInstanced();
		//b.flushInstanced();
		c.flushInstanced();
												 
		window.update();

		keyPresses(cam, window, shader);
		
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.wolrdRayVec(window.getX(), window.getY(), width, height);
			std::cout << "mouse world pos " << ray_world << std::endl;
		}

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			shader.setUniformMat4("proj", cam.projMatrix(width, height));
		}

		if (cam.update()) {
			shader.setUniformMat4("view", cam.viewMatrixUpdate()); 
		}

	}

	//a.clean();
	//b.clean();
	c.clean();

	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::Shader &shader) {
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
		if (shader.reloadShader(VERT_INST, FRAG)) {
			int width = window.getWidth();
			int height = window.getHeight();

			//mme::math::mat4 translate;
			//translate = mme::math::mat4::translationMatrix(0.0f, 0.0f, 0.0f);

			shader.enable();
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