// Vertex Shader
#version 410
layout(location = 0) in vec3 vp;	// vertex positions
layout(location = 1) in vec2 vt;	// vertex colour
layout(location = 2) in vec3 vn;	// vertex normals

uniform mat4 view, proj;			// view and projection matrices from camera
uniform mat4 model_matrix = mat4(1.0, 0.0, 0.0, 0.0,	
							 	 0.0, 1.0, 0.0, 0.0,
								 0.0, 0.0, 1.0, 0.0,
								 0.0, 0.0, 0.0, 1.0);

out vec3 Kd, eye_pos, eye_normal; // outputs to fragment shader after raserization for lighting calc.
out vec3 normal;
out vec2 texcoord;	

void main() {
	
	normal = vn;
	texcoord = vt;
	vec4 eye = view * model_matrix * vec4(vp, 1.0);	// vertex positions (vp) in eye space, vec4
	eye_pos = vec3(eye);							// vertex positions (vp) in eye space, vec3
	eye_normal = vec3(view * vec4(vn, 0.0));		// normals in eye space
	Kd = vec3(0.2, 0.0, 0.3);						// surface reflection color
	gl_Position= proj * eye;	

}