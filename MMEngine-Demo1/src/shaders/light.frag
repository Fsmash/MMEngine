// Fragment Shader
#version 410

// uniform when view updates
uniform mat4 view;

// input from vertex shader for lighting calculations
in vec3 Kd, eye_pos, eye_normal, normal; // should probably rename eye_pos and eye_normal

// Light properties
vec3 light_pos_world = vec3(0.0, 0.0, 4.0);	// position of light source in world space

vec3 Ls = vec3(1.0, 1.0, 1.0);				// white specular light colour
vec3 Ld = vec3(0.7, 0.7, 0.7);				// dull white diffuse light color
vec3 La = vec3(0.25, 0.25, 0.25);				// grey ambient light colour

// Surface properties
vec3 Ks = vec3(1.0, 1.0, 1.0);				// fully reflect specular light
vec3 Ka = vec3(1.0, 1.0, 1.0);				// fully reflect ambient light
float specular_exponent = 400.0;			// specular power

// final output color 
out vec4 frag_colour;

void main() {

	// *** AMBIENT INTENSITY CALCULATIONS
	vec3 Ia = La * Ka;

	// *** DIFFUSE INTENSITY CALCULATIONS
	
	// light source's vertex positions in eye space
	vec3 light_pos_eye = vec3(view * vec4(light_pos_world, 1.0));

	// distance of light source to sruface
	vec3 dist_from_eye = light_pos_eye - eye_pos; 

	// direction of surface to light
	vec3 direction_to_eye = normalize(dist_from_eye);

	// dot product of direction light vec and normal (surface).
	// gives intensity of diffuse based on how the object being illuminated faces the light source
	float dot_prod = dot(direction_to_eye, eye_normal);
	
	// negative dot product possible so set max value to 0.0
	dot_prod = max(dot_prod, 0.0);

	// *** END DIFFUSE INTENSITY CALCULATIONS

	// diffuse intensity
	vec3 Id = Ld * Kd * dot_prod;
	
	// *** SPECULAR INTENSITY CALCULATIONS

	// direction of observer in respect to direction of surface 
	vec3 surface_to_viewer = normalize(-eye_pos);

	// direction half way between surface to camera and surface to light, used to approximate reflection
	vec3 half_way_eye = normalize(surface_to_viewer + direction_to_eye);

	// dot product of reflection approx. and surface normal. maximum set to zero, (possible negative dot product)
	float dot_prod_specular = max(0.0, dot(half_way_eye, eye_normal));

	// specular intensity raised to power of specular exponent
	float specular_factor = pow(dot_prod_specular, specular_exponent);

	// *** END OF SPECULAR INTENSITY CALCULATIONS

	// specular intensity
	vec3 Is = Ls * Ks * specular_factor;
	
	//frag_colour=vec4(Is + Id + Ia, 1.0);
	frag_colour=vec4(normal, 1.0);

}