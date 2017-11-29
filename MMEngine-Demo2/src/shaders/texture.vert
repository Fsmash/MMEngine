// Vertex Shader
#version 420
layout(location = 0) in vec3 vp;	// vertex positions
layout(location = 1) in vec2 vt;	// texture coordinates
layout(location = 2) in vec3 vn;	// vertex normals

uniform mat4 view, proj;			// view and projection matrices from camera

// outputs to fragment shader after raserization for lighting calc.
out vec2 tex_coord;
out vec3 eye_pos, eye_normal;		

void main() {
	
	eye_pos = vec3(view * vec4(vp, 1.0));		// vertex positions (vp) in eye space
	eye_normal = vec3(view * vec4(vn, 0.0));	// normals in eye space
	tex_coord = vt;									// surface reflection color
	gl_Position= proj * view * vec4(vp, 1.0);	

}