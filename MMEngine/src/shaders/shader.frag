// Fragment Shader
#version 410
uniform mat4 view;
in vec3 Kd, eye_pos, eye_normal; // should probably rename eye_pos and eye_normal

// Light properties
vec3 light_pos_world = vec3(0.0, 0.0, 4.0);	// position of light source in world space

vec3 Ls = vec3(1.0, 1.0, 1.0);				// white specular light colour
vec3 Ld = vec3(0.7, 0.7, 0.7);				// dull white diffuse light color
vec3 La = vec3(0.2, 0.2, 0.2);				// grey ambient light colour

// Surface properties
vec3 Ks = vec3(1.0, 1.0, 1.0);				// fully reflect specular light
vec3 Ka = vec3(1.0, 1.0, 1.0);				// fully reflect ambient light
float specular_exponent = 100.0;			// specular power

out vec4 frag_colour;

void main() {

	// *** AMBIENT INTENSITY CALCULATIONS
	vec3 Ia = La * Ka;

	// *** DIFFUSE INTENSITY CALCULATIONS
	
	// light source's vertex positions in eye space
	vec3 light_pos_eye = vec3(view * vec4(light_pos_world, 1.0));

	// distance from surface to light source
	vec3 dist_from_eye = light_pos_eye - eye_pos; 

	// direction to light from surface
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

	// refleciton of light around surface normal
	vec3 reflection_eye = reflect(-direction_to_eye, eye_normal);

	// direction of observer in respect to direction of reflection 
	vec3 surface_to_viewer = normalize(-eye_pos);

	// dot product of specular light, surface and observer direction
	float dot_prod_specular = dot(reflection_eye, surface_to_viewer);

	// negative dot product possible so set max value to 0.0
	dot_prod_specular = max(dot_prod_specular, 0.0);

	// specular intensity raised to power of specular exponent
	float specular_factor = pow(dot_prod_specular, specular_exponent);

	// *** END OF SPECULAR INTENSITY CALCULATIONS

	// specular intensity
	//vec3 Is = vec3(0.0, 0.0, 0.0); 
	vec3 Is = Ls * Ks * specular_factor;
	
	frag_colour=vec4(Is + Id + Ia, 1.0);

}