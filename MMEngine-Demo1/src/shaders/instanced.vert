// Vertex Shader
#version 410
layout(location = 0) in vec3 vp;	// vertex positions
layout(location = 1) in vec2 vt;	// vertex colour
layout(location = 2) in vec3 vn;	// vertex normals
layout(location = 3) in mat4 model_matrix; // mat4 model matrix per intance

uniform mat4 view, proj;			// view and projection matrices from camera

out vec3 Kd, eye_pos, eye_normal, normal;	// outputs to fragment shader after raserization for lighting calc.
out vec2 tex_coord;
void main() {
	normal = vn;
	tex_coord = vt;
	eye_pos = vec3(view * model_matrix * vec4(vp, 1.0));		// vertex positions (vp) in eye space
	eye_normal = vec3(view * vec4(vn, 0.0));	// normals in eye space
	//Kd = vt;									// surface reflection color
	gl_Position= proj * view * model_matrix * vec4(vp, 1.0);	

}