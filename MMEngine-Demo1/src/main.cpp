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

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define DEBUG 1
#define VERT "shaders/light.vert"
#define FRAG "src/shaders/light.frag"

#define VERT_TEX "shaders/texture.vert"
#define FRAG_TEX "shaders/texture.frag"

#define VERT_INST "src/shaders/instanced.vert"
#define FRAG_BASIC "shaders/basic.frag"

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
bool loadMesh(const char *file_name, GLuint *vao, int *point_count);

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

	Shape cube = ShapeGenerator3D::makeCube();

	//ShapeRenderer c(cube);
	//cube.cleanUp();
	mat4 *matrices = new mat4[500000];
	GLsizeiptr matBuf = sizeof(mat4) * 500000;

	float r1;
	float r2;
	float r3;

	for (int i = 0; i < 500000; i++) {

		r1 = 2.5f * float(i) / 4.0f;
		r2 = 2.5f * float(i) / 5.0f;
		r3 = 2.5f * float(i) / 6.0f;


		matrices[i] = mat4::rotationMatrixX(r1) * mat4::rotationMatrixY(r2)
			* mat4::rotationMatrixZ(r3) * mat4::translationMatrix(0.01f + (r1 / 1000.0f), 0.02f + (r2 / 1000.f), 0.03f + (r3 / 1000.f));
	}

	//c.submitMat(matrices, 500000, 3, matBuf);

	//model importing
	GLuint monkey_vao;
	int monkey_point_count = 0;
	loadMesh(MESH_FILE, &monkey_vao, &monkey_point_count);
	
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

	Camera cam(0.0f, 0.0f, -50.0f);
	cam.speed = 0.12f;
	cam.roll_speed = 1.5f;
	cam.yaw_speed = 1.5f;
	cam.pitch_speed = 1.5f;
	cam.setFar(500.0f);
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

		//c.flushInstanced();

		//draw monkey guy
		glBindVertexArray(monkey_vao);
		glDrawArrays(GL_TRIANGLES, 0, monkey_point_count);

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

	//c.clean();

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
			cam.setPos(0.0f, 0.0f, -50.0f);
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

bool loadMesh(const char *file_name, GLuint *vao, int *point_count) {
	//load a file with assimp and print scene information
	const aiScene *scene = aiImportFile(file_name, aiProcess_Triangulate);
	
	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}

	printf("  %i animations\n", scene->mNumAnimations);
	printf("  %i cameras\n", scene->mNumCameras);
	printf("  %i lights\n", scene->mNumLights);
	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	//get first mesh in file only
	const aiMesh *mesh = scene->mMeshes[0];
	printf("    %i vertices in mesh[0]\n", mesh->mNumVertices);

	*point_count = mesh->mNumVertices;
	
	glGenVertexArrays(1, vao);
	glBindVertexArray(*vao);

	//grab model data from assimp
	GLfloat *points = NULL;		 // array of vertex points
	GLfloat *normals = NULL;	 // array of vertex normals
	GLfloat *texcoords = NULL; // array of texture coordinates
	if (mesh->HasPositions()) {
		points = (GLfloat *)malloc(*point_count * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D *vp = &(mesh->mVertices[i]);
			points[i * 3] = (GLfloat)vp->x;
			points[i * 3 + 1] = (GLfloat)vp->y;
			points[i * 3 + 2] = (GLfloat)vp->z;
		}
	}
	if (mesh->HasNormals()) {
		normals = (GLfloat *)malloc(*point_count * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D *vn = &(mesh->mNormals[i]);
			normals[i * 3] = (GLfloat)vn->x;
			normals[i * 3 + 1] = (GLfloat)vn->y;
			normals[i * 3 + 2] = (GLfloat)vn->z;
		}
	}
	if (mesh->HasTextureCoords(0)) {
		texcoords = (GLfloat *)malloc(*point_count * 2 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) {
			const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2] = (GLfloat)vt->x;
			texcoords[i * 2 + 1] = (GLfloat)vt->y;
		}
	}

	//copy data into vbos
	if (mesh->HasPositions()) {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), points,
			GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		free(points);
	}
	if (mesh->HasNormals()) {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * *point_count * sizeof(GLfloat), normals,
			GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
		free(normals);
	}
	if (mesh->HasTextureCoords(0)) {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * *point_count * sizeof(GLfloat), texcoords,
			GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
		free(texcoords);
	}
	if (mesh->HasTangentsAndBitangents()) {
		// NB: could store/print tangents here
	}

	aiReleaseImport(scene);
	printf("mesh loaded\n");

	return true;
}
