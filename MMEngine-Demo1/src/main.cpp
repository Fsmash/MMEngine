//#define STB_IMAGE_IMPLEMENTATION
#include "utility/stb_image.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/camera.h"
#include "graphics/objects/shape_generator_2D.h"
#include "graphics/objects/shape_generator_3D.h"
#include "graphics/shape_renderer.h"
#include "graphics/buffers/buffer.h"
#include "graphics/objects/model.h"

#define DEBUG 1
#define VERT "src/shaders/light.vert"
#define FRAG "src/shaders/light.frag"

#define VERT_TEX "src/shaders/texture.vert"
#define FRAG_TEX "src/shaders/texture.frag"

#define VERT_INST "src/shaders/instanced.vert"
#define FRAG_BASIC "src/shaders/basic.frag"

#define MESH_FILE "res/suzanne.obj"

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

	//model importing
	//GLuint monkey_vao;
	/*int monkey_point_count = 0;
	loadMesh(MESH_FILE, &monkey_vao, &monkey_point_count);
	*/

	Model model(MESH_FILE);
	std::cout << "num_indices = " << model.num_indices << std::endl;
	std::cout << "num_vertices = " << model.num_vertices << std::endl;

	/*
	for (int i = 0; i < model.num_vertices; i++) {
		std::cout << "pos = " << model.vertices[i].pos << std::endl;
		std::cout << "tex coord = " << model.vertices[i].uv << std::endl;
		std::cout << "normal = " << model.vertices[i].normal << std::endl;
	}*/

	GLuint vbo = 0;
	GLuint ibo = 0;

	GLsizeiptr bufferSz = model.vertexBufferSize();

	std::cout << "vertex buffer size = " << bufferSz << std::endl;
	std::cout << "vertex size = " << sizeof(VertexT) << std::endl;

	glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
	glEnableVertexAttribArray(1); // enables generic vertex attribute array (attribute 1, tex coord)
	glEnableVertexAttribArray(2); // enables generic vertex attribute array (attribute 2, normal)

	glGenBuffers(1, &vbo);	// Generate vertex buffer id (name), just and unsigned int.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Bind buffer "vbo" as current in context
	glBufferData(GL_ARRAY_BUFFER, bufferSz, model.vertices, GL_STATIC_DRAW);

	bufferSz = model.indexBufferSize();
	std::cout << "index buffer size = " << bufferSz << std::endl;
	std::cout << "index size = " << sizeof(GLuint) << std::endl;

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);	// Bind buffer "vbo" as current in context
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSz, model.indices, GL_STATIC_DRAW);

	//model.bufferModel();
	
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

	Camera cam(0.0f, 0.0f, 4.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(500.0f);
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 

	Shader shader(VERT, FRAG);
	shader.enable();
	shader.setUniformMat4("view", cam.viewMatrix());
	shader.setUniformMat4("proj", cam.projMatrix(width, height));
	//shader.setUniformMat4("model_matrix", translate);

	vec3 ray_world;

	window.setClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	while (!window.closed()) {

		window.frameCounter();
		window.clear();		

		//draw monkey guy
		//glBindVertexArray(monkey_vao);
		//glDrawArrays(GL_TRIANGLES, 0, monkey_point_count);
		//model.flush();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset1()); // defines layout of buffer.
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexT::vertexSize(), VertexT::offset2()); // defines layout of buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glDrawElements(GL_TRIANGLES, model.num_indices, GL_UNSIGNED_INT, nullptr);
		
		window.update();

		keyPresses(cam, window, shader);

		if (window.isMousePressed(GLFW_MOUSE_BUTTON_1)) {
			ray_world = cam.worldRayVec(window.getX(), window.getY(), width, height);
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

	//model.clean();
	return 0;
}

void keyPresses(mme::graphics::Camera &cam, mme::graphics::Window &window, mme::graphics::Shader &shader) {
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
		if (shader.reloadShader(VERT_INST, FRAG)) {
			int width = window.getWidth();
			int height = window.getHeight();

		
			shader.enable();
			cam.setPos(0.0f, 0.0f, 4.0f);
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