// Vertex Shader
#version 410
layout(location = 0) in vec3 vp;	// vertex positions
layout(location = 1) in vec3 vc;	// vertex colour
layout(location = 2) in vec3 vn;	// vertex normals
layout(location = 3) in mat4 model_matrix; // mat4 model matrix per intance

uniform mat4 view, proj;			// view and projection matrices from camera

out vec3 Kd, eye_pos, eye_normal, normal;	// outputs to fragment shader after raserization for lighting calc.

void main() {
	normal = vn;
	eye_pos = vec3(view * model_matrix * vec4(vp, 1.0));		// vertex positions (vp) in eye space
	eye_normal = vec3(view * vec4(vn, 0.0));	// normals in eye space
	Kd = vc;									// surface reflection color
	gl_Position= proj * view * vec4(vp, 1.0);	

}