#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/camera.h"

#define DEBUG 1
#define VERT "src/shaders/shader.vert"
#define FRAG "src/shaders/shader.frag"

#if DEBUG
#include "src/utility/log.h"
#define INIT_LOG() restart_gl_log()
#define LOG(x, ...) gl_log(x, __VA_ARGS__)
#define INFO() log_gl_params()
#else
#define INIT_LOG() true
#define LOG(x, y)
#define INFO()
#endif

struct Vertex {
	mme::math::vec3 pos;
	mme::math::vec3 color;
	mme::math::vec3 normal;
};

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

		glEnable(GL_DEPTH_TEST);	// enable depth testing
		glDepthFunc(GL_LESS);	// depth testing interprets a smaller values as "closer"
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set up clear color for glfw
	}
	else {
		system("PAUSE");
		return 1;
	}

	// Vertices for VBO. Attribute data interleaved in Vertex struct. Vertices passed in clockwise order.
	Vertex vertices[4];
	vertices[0].pos = vec3(-0.5f, -0.5f, 0.0f);
	vertices[0].color = vec3(1.0f, 0.0f, 0.0f);
	vertices[0].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[1].pos = vec3(0.0f, 0.5f, 0.0f);
	vertices[1].color = vec3(0.0f, 1.0f, 0.0f);
	vertices[1].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[2].pos = vec3(0.5f, -0.5f, 0.0f);
	vertices[2].color = vec3(0.0f, 0.0f, 1.0f);
	vertices[2].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[3].pos = vec3(0.5f, 0.5f, 0.0f);
	vertices[3].color = vec3(0.5f, 0.0f, 0.5f);
	vertices[3].normal = vec3(0.0f, 0.0f, 1.0f);

	// Vertex Buffer Object
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);	// Generate vertex buffer id (name), just and unsigned int.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Bind buffer "vbo" as current in context
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Copy all data into buffer on vram
	printf("vbo name: %d\n", vbo); // name of vbo object in state
	printf("size of vertices: %d", sizeof(vertices));

	glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
	
	glEnableVertexAttribArray(1); // enables attribute (attribute 1, color)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (const void *)12); // defines layout of buffer.
	
	glEnableVertexAttribArray(2); // enables attribute (attribute 2, normals)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (const void *)24); // defines layout of buffer.

	Shader shader(VERT, FRAG);

	int width = window.getWidth();
	int height = window.getHeight();

	Camera cam(0.0f, 0.0f, 3.0f);
	cam.speed = 0.25f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.init(45.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 
	
	shader.enable();
	shader.setUniformMat4("view", cam.viewMatrix());
	shader.setUniformMat4("proj", cam.projMatrix(width, height));

	vec3 ray_world;

	//glBindVertexArray(vao); // bind again cause the vbo was bound last. need to make vao current

	while (!window.closed()) {

		window.frameCounter();
		window.clear();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw in triangle mode starting from point 0, (index 0)
												// for 4 indices (1 index consist of 3 points as defined by glVertexAttribPointer)

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

	glDeleteBuffers(1, &vbo);

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
		if (shader.reloadShader(VERT, FRAG)) {
			int width = window.getWidth();
			int height = window.getHeight();

			shader.enable();
			shader.setUniformMat4("view", cam.viewMatrixUpdate());
			shader.setUniformMat4("proj", cam.projMatrix(width, height));
			printf("It Worked!");
		}
		else {
			printf("you done fucked up son");
		}
	}
}