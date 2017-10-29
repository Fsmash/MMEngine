// Vertex Shader
#version 410
layout(location = 0) in vec3 vp;	// vertex positions
layout(location = 1) in vec3 vc;	// vertex colour
layout(location = 2) in vec3 vn;	// vertex normals

uniform mat4 view, proj;			// view and projection matrices from camera

out vec3 Kd, eye_pos, eye_normal;

void main() {
	
	eye_pos = vec3(view * vec4(vp, 1.0));		// vertex positions (vp) in eye space
	eye_normal = vec3(view * vec4(vn, 0.0));	// normals in eye space
	Kd = vc;									// surface reflection color
	gl_Position= proj * view * vec4(vp, 1.0);	

}