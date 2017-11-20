//#define STB_IMAGE_IMPLEMENTATION
#include "src/utility/stb_image.h"
#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/camera.h"
#include "src/graphics/objects/shape_generator_2D.h"
#include "src/graphics/objects/shape_generator_3D.h"
#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/index_buffer.h"

#define DEBUG 1
#define VERT "src/shaders/shader.vert"
#define FRAG "src/shaders/shader.frag"

#define VERT_TEX "src/shaders/texture.vert"
#define FRAG_TEX "src/shaders/texture.frag"


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
	/*
	// Vertices for VBO. Attribute data interleaved in Vertex struct. Vertices passed in clockwise order.
	Vertex vertices[4];
	vertices[0].pos = vec3(-0.8f, -0.5f, 0.0f);
	vertices[0].uv = vec2(0.0f, 0.0f);
	//vertices[0].color = vec3(1.0f, 0.0f, 0.0f);
	vertices[0].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[1].pos = vec3(-0.5f, 0.5f, 0.0f);
	vertices[1].uv = vec2(0.0f, 1.0f);
	//vertices[1].color = vec3(0.0f, 1.0f, 0.0f);
	vertices[1].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[2].pos = vec3(0.5f, 0.5f, 0.0f);
	vertices[2].uv = vec2(1.0f, 1.0f);
	//vertices[2].color = vec3(0.0f, 0.0f, 1.0f);
	vertices[2].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices[3].pos = vec3(0.5f, -0.5f, 0.0f);
	vertices[3].uv = vec2(1.0f, 0.0f);
	//vertices[3].color = vec3(0.5f, 0.0f, 0.5f);
	vertices[3].normal = vec3(0.0f, 0.0f, 1.0f);
	
	Vertex vertices2[4];
	vertices2[0].pos = vec3(0.65f, -0.5f, 0.0f);
	vertices2[0].uv = vec2(0.0f, 0.0f);
	//vertices2[0].color = vec3(1.0f, 0.0f, 0.0f);
	vertices2[0].normal = vec3(0.0f, 0.0f, 1.0f);
	
	vertices2[1].pos = vec3(0.65f, 0.5f, 0.0f);
	vertices2[1].uv = vec2(0.0f, 1.0f);
	//vertices2[1].color = vec3(0.0f, 1.0f, 0.0f);
	vertices2[1].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices2[2].pos = vec3(0.95f, 0.5f, 0.0f);
	vertices2[2].uv = vec2(1.0f, 1.0f);
	//vertices2[2].color = vec3(0.0f, 0.0f, 1.0f);
	vertices2[2].normal = vec3(0.0f, 0.0f, 1.0f);

	vertices2[3].pos = vec3(0.95f, -0.5f, 0.0f);
	vertices2[3].uv = vec2(1.0f, 0.0f);
	//vertices2[3].color = vec3(0.5f, 0.0f, 0.5f);
	vertices2[3].normal = vec3(0.0f, 0.0f, 1.0f);

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6, 
		6, 7, 4
	};
	*/

	Shape triangle = ShapeGenerator2D::makeTriangle();
	Shape cube = ShapeGenerator3D::makeCube();

	Buffer vbo(triangle.vertices, triangle.vertexBufferSize());
	IndexBuffer ibo(triangle.indices, triangle.indexBufferSize());

	Buffer vbo2(cube.vertices, cube.vertexBufferSize());
	IndexBuffer ibo2(cube.indices, cube.indexBufferSize());
	
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// Vertex Buffer Object, vertices and indices stored in ARRAY BUFFER
	//GLuint vbo = 0;
	//glGenBuffers(1, &vbo);	// Generate vertex buffer id (name), just and unsigned int.
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Bind buffer "vbo" as current in context
	//glBufferData(GL_ARRAY_BUFFER, triangle.vertexBufferSize(), triangle.vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(vertices2), 0, GL_STATIC_DRAW);	// Specify size of buffer required to store all data
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(vertices2), vertices2);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(indices), indices);
	//printf("vbo name: %d\n", vbo); // name of vbo object in state
	//printf("size of vertices: %d", sizeof(vertices));
	//glBindVertexArray(vao);
	vbo.bind();
	glEnableVertexAttribArray(0); // enables generic vertex attribute array (attribute 0, position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0); // defines layout of buffer, "vbo", for attribute 0 (positions)
	
	glEnableVertexAttribArray(1); // enables attribute (attribute 1, color)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1()); // defines layout of buffer.
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 32, (const void *)12); // defines layout of buffer.
	
	glEnableVertexAttribArray(2); // enables attribute (attribute 2, normals)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2()); // defines layout of buffer.
	ibo.bind();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);

	//GLuint offset = sizeof(vertices);
	triangle.cleanUp();
	
	GLuint vao2 = 0;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	vbo2.bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset1());

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VertexC::vertexSize(), VertexC::offset2());
	ibo2.bind();
	cube.cleanUp();

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
	// Shader and Camera set up
	Shader shader(VERT, FRAG);
	//Shader shader(VERT_TEX, FRAG_TEX);

	int width = window.getWidth();
	int height = window.getHeight();
	math::mat4 translate;

	Camera cam(0.0f, 0.0f, 3.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.init(0.0f, 0.0f, 0.0f, 1.0f);	// vanilla start, 0 degrees about the y axis. 
	
	shader.enable();
	shader.setUniformMat4("view", cam.viewMatrix());
	shader.setUniformMat4("proj", cam.projMatrix(width, height));
	//shader.setUniformMat4("model_matrix", translate);

	vec3 ray_world;

	window.setClearColor(0.0f, 0.4f, 0.8f, 1.0f);

	while (!window.closed()) {

		window.frameCounter();
		window.clear();
		
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBindVertexArray(vao);
		// Index (Element) Buffer must be unsigned ints
		translate = math::mat4::translationMatrix(-4.0f, 1.0f, -2.0f);
		shader.setUniformMat4("model_matrix", translate);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr); // ibo currently bound, can just have nullptr as last parameter. 
		translate = math::mat4::translationMatrix(0.0f, 1.0f, 0.0f);
		shader.setUniformMat4("model_matrix", translate);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		//glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
		glBindVertexArray(vao2);
		// Index (Element) Buffer must be unsigned ints
		translate = math::mat4::translationMatrix(-4.0f, -1.0f, -2.0f);
		shader.setUniformMat4("model_matrix", translate);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // ibo currently bound, can just have nullptr as last parameter. 
		translate = math::mat4::translationMatrix(0.0f, 0.0f, 0.0f);
		shader.setUniformMat4("model_matrix", translate);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)(sizeof(GLuint) * 6)); // ibo currently bound, can just have nullptr as last parameter. 
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw in triangle mode starting from point 0, (index 0)
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

	//glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ibo);

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
			cam.setPos(0.0f, 0.0f, 3.0f);
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