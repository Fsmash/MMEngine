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

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window);

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
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);	// set up clear color for glfw
	}
	else {
		system("PAUSE");
		return 1;
	}

	// Points for triangle. Points passed in clockwise order to vbo.
	GLfloat points[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	// vertex color 
	GLfloat colours[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f
	};

	// Vertex Buffer Object
	GLuint points_vbo = 0;	// vbo that will hold vertices
	glGenBuffers(1, &points_vbo); // generates 1 empty buffer, (generates a "name")
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // set as current buffer in openGL's state machine by "binding"
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); // copies over points to currenltly bound buffer.
	printf("points_vbo name: %d\n", points_vbo); // name of vbo object in state

	GLuint colours_vbo = 0;	// vbo that will hold interpolated colors for each vertex
	glGenBuffers(1, &colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);
	printf("colours_vbo name: %d\n", colours_vbo); // name of vbo object in state

	// Vertex Array Object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao); // generates 1 empty vertex attribute object (generates a name)
	glBindVertexArray(vao); // set vao as current in openGL's state machine by "binding"
	glEnableVertexAttribArray(0); // enable first attribute in vao (attribute 0) which is the vbo
									// will be at attribute 0 since only one vbo will be used
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // sets buffer as current again, see comment for glBindBuffer above
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // defines layout of buffer object in vao.
																// 0 means define layout for attribute number 0
																// 3 means that the variables are vec3 made from every 3 floats in buffer
	glEnableVertexAttribArray(1); // enable attribute 1 for colour_vbo
	glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	printf("vao name: %d\n", vao); // name of vao object in state

	Shader shader(VERT, FRAG);

	int width = window.getWidth();
	int height = window.getHeight();

	Camera cam(0.0f, 0.0f, 2.0f);
	cam.speed = 0.25f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.init(0.0f, 0.0f, 1.0f, 0.0f);
	shader.enable();
	shader.setUniformMat4("view", cam.viewMatrix());
	shader.setUniformMat4("proj", cam.projMatrix(width, height));
	shader.disable();

	//mat4 bruh(vec4(4,0,0,1), vec4(0,0,1,0), vec4(0,2,2,0), vec4(0,0,0,1));
	//std::cout << mat4::inverseMatrix(bruh) * bruh << std::endl;

	while (!window.closed()) {

		window.frameCounter();
		window.clear();

		shader.enable();
		// Drawing first VAO, vao
		glBindVertexArray(vao); // bind again cause the vbo was bound last. need to make vao current
								// set vao as input variable for all further drawing (in this case just some vertex points)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw in triangle mode starting from point 0, (index 0)
												// for 4 indices (1 index consist of 3 points as defined by glVertexAttribPointer)

		window.update();

		keyPresses(cam, window);

		// update projection matrix with new width and height on resize.
		if (window.resized()) {
			width = window.getWidth();
			height = window.getHeight();
			shader.setUniformMat4("proj", cam.projMatrix(width, height));
			std::cout << "updated projection matrix" << std::endl;
		}

		if (cam.update()) {
			std::cout << "view updated" << std::endl;
			shader.setUniformMat4("view", cam.viewMatrixUpdate());
		}

	}

	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window) {
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