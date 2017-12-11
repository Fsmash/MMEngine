#include "camera.h"

namespace mme {
	namespace graphics {

		// PRIVATE MEMBER FUNCTIONS

		// Initializes a quaternion for rotation (versor). Takes in angle and x, y, and z componentes representing axis to be rotated.
		void Camera::createVersor(float *const q, const float angle, const float x, const float y, const float z) {
			float rad = angle * RADIANS;
			q[0] = cos(rad * 0.5f);		// w, angle
			q[1] = sin(rad * 0.5f) * x;	// x comp
			q[2] = sin(rad * 0.5f) * y;	// y comp
			q[3] = sin(rad * 0.5f) * z;	// z comp
		}

		// Normalize the quaternion (versor), i.e. give it a magitude of 1 making it a direction.
		void Camera::normalizeVersor() {
			float sum = m_quat[0] * m_quat[0] + m_quat[1] * m_quat[1] + m_quat[2] * m_quat[2] + m_quat[3] * m_quat[3];
			// only compute sqrt if sum meets threshold
			if (fabs(1.0f - sum) < 0.0001f) return;
			float mag = sqrt(sum);
			for (int i = 0; i < 4, i++;) {
				m_quat[i] /= mag;
			}
		}

		// Multiplies a rotation quaternion (versor) with private member m_quat[4] to keep track of all rotations.
		void Camera::multVersor(float *const r) {
			float s0 = m_quat[0];
			float s1 = m_quat[1];
			float s2 = m_quat[2];
			float s3 = m_quat[3];

			m_quat[0] = s0 * r[0] - s1 * r[1] - s2 * r[2] - s3 * r[3];
			m_quat[1] = s0 * r[1] + s1 * r[0] - s2 * r[3] + s3 * r[2];
			m_quat[2] = s0 * r[2] + s1 * r[3] + s2 * r[0] - s3 * r[1];
			m_quat[3] = s0 * r[3] - s1 * r[2] + s2 * r[1] + s3 * r[0];
			
			normalizeVersor();
		}

		// Converts 4D quaternion with w, x, y, z components (w, being angle in 4D space) to 3D rotation matrix, m_rotation.
		void Camera::quatToMatrix() {
			float w = m_quat[0];
			float x = m_quat[1];
			float y = m_quat[2];
			float z = m_quat[3];
			
			m_rotation.matrix[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
			m_rotation.matrix[1] = 2.0f * x * y + 2.0f * w * z;
			m_rotation.matrix[2] = 2.0f * x * z - 2.0f * w * y;
			m_rotation.matrix[3] = 0.0f;
					   
			m_rotation.matrix[4] = 2.0f * x * y - 2.0f * w * z;
			m_rotation.matrix[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
			m_rotation.matrix[6] = 2.0f * y * z + 2.0f * w * x;
			m_rotation.matrix[7] = 0.0f;
					   
			m_rotation.matrix[8] = 2.0f * x * z + 2.0f * w * y;
			m_rotation.matrix[9] = 2.0f * y * z - 2.0f * w * x;
			m_rotation.matrix[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
			m_rotation.matrix[11] = 0.0f;
					   
			m_rotation.matrix[12] = 0.0f;
			m_rotation.matrix[13] = 0.0f;
			m_rotation.matrix[14] = 0.0f;
			m_rotation.matrix[15] = 1.0f;
		}

		// END OF PRIVATE MEMBER FUNCTIONS

		// Constructor to set default values for camera position, rotation angles, and projection matrix components
		Camera::Camera() {
			m_pos.x = m_pos.y = m_pos.z = 0.0f;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		// Constructor to set default values for rotation angles, and projection matrix components
		Camera::Camera(const float x, const float y, const float z) {
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		// Constructor to set default values for rotation angles, and projection matrix components
		Camera::Camera(const math::vec3 cam_pos) {
			m_pos.x = cam_pos.x;
			m_pos.y = cam_pos.y;
			m_pos.z = cam_pos.z;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		// Mutator to set camera position
		void Camera::setPos(const float x, const float y, const float z) {
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
		}
		
		// Increase m_vel in the positive x direction
		void Camera::right() { 
			m_vel.x += speed; 
			m_moved = true;
		}
		
		// Increase translation values (m_vel) of camera. Moves camera around.
		void Camera::left() {
			m_vel.x -= speed; 
			m_moved = true;
		}
		
		void Camera::up() { 
			m_vel.y += speed; 
			m_moved = true;
		}
		
		void Camera::down() { 
			m_vel.y -= speed; 
			m_moved = true;
		}
		
		void Camera::forward() { 
			m_vel.z -= speed; 
			m_moved = true;
		}
		
		void Camera::back() {
			m_vel.z += speed; 
			m_moved = true;
		}

		// Construct quaternions based off of angle and axis of rotation. Rotates the camera.
		void Camera::turnRight() { 
			m_yaw -= yaw_speed;
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			float q_yaw[4];
			createVersor(q_yaw, m_yaw, m_up.x, m_up.y, m_up.z);
			multVersor(q_yaw);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}
		
		void Camera::turnLeft() { 
			m_yaw += yaw_speed; 
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			float q_yaw[4];
			createVersor(q_yaw, m_yaw, m_up.x, m_up.y, m_up.z);
			multVersor(q_yaw);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}
		
		void Camera::lookUp() { 
			m_pitch += pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			float q_pitch[4];
			createVersor(q_pitch, m_pitch, m_right.x, m_right.y, m_right.z);
			multVersor(q_pitch);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}
		
		void Camera::lookDown() {
			m_pitch -= pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			float q_pitch[4];
			createVersor(q_pitch, m_pitch, m_right.x, m_right.y, m_right.z);
			multVersor(q_pitch);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}
		
		void Camera::tiltRight() {
			m_roll -= roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			float q_roll[4];
			createVersor(q_roll, m_roll, m_forward.x, m_forward.y, m_forward.z);
			multVersor(q_roll);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}

		void Camera::tiltLeft() {
			m_roll += roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			float q_roll[4];
			createVersor(q_roll, m_roll, m_forward.x, m_forward.y, m_forward.z);
			multVersor(q_roll);
			quatToMatrix();
			m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_moved = true;
		}

		// Initialize camera orientation given angle and axis of rotation.
		void Camera::init(const float angle, const float x, const float y, const float z) {
			if (!m_init) {
				createVersor(m_quat, angle, x, y, z);
				normalizeVersor();
				quatToMatrix();
				m_right = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
				m_up = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
				m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);
				m_translation = math::mat4::translationMatrix(-m_pos.x, -m_pos.y, -m_pos.z);
			}
			m_init = true;
		}

		// Sets camera back to specified orientation.
		void Camera::setOrientation(const float angle, const float x, const float y, const float z) {
			m_init = false;
			init(angle, x, y, z);
		}

		// Construct initial view matrix
		math::mat4 Camera::viewMatrix() {
			if (!m_init) {
				std::cout << "Need to initialize eye view first." << std::endl;
				return math::mat4::identity();
			}

			m_view = math::mat4::inverseMatrix(m_rotation) * m_translation;

			return m_view;
		}

		// Construct view matrix after translation or rotation occurs i.e. m_move set to true
		math::mat4 Camera::viewMatrixUpdate() {
			using namespace math;

			if (!m_init) {
				std::cout << "Need to initialize eye view first." << std::endl;
				return math::mat4::identity();
			}

			quatToMatrix();

			// maintain current orientation
			m_pos = m_pos + vec3(m_right.x, m_right.y, m_right.z).scale(m_vel.x);
			m_pos = m_pos + vec3(m_up.x, m_up.y, m_up.z).scale(m_vel.y);
			m_pos = m_pos + vec3(m_forward.x, m_forward.y, m_forward.z).scale(-m_vel.z);

			m_translation = mat4::translationMatrix(m_pos.x, m_pos.y, m_pos.z);

			m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;

			m_view = mat4::inverseMatrix(m_rotation) * mat4::inverseMatrix(m_translation);

			return m_view;
		}

		// Construct projection matrix based off of window width and height
		math::mat4 Camera::projMatrix(const int width, const int height) {
			using namespace math;

			float aspect = (float) width / (float) height;
			float range = tan(m_fov * RADIANS * 0.5f) * m_near;
			float Sx = (2.0f * m_near) / (range * aspect + range * aspect);
			float Sy = m_near / range;
			float Sz = -(m_far + m_near) / (m_far - m_near);
			float Pz = -(2.0f * m_far * m_near) / (m_far - m_near);

			m_projection.columns[0] = vec4(Sx, 0.0f, 0.0f, 0.0f);
			m_projection.columns[1] = vec4(0.0f, Sy, 0.0f, 0.0f);
			m_projection.columns[2] = vec4(0.0f, 0.0f, Sz, -1.0f);
			m_projection.columns[3] = vec4(0.0f, 0.0f, Pz, 0.0f);
			
			return m_projection;
		}

		// Constructs a vec3 "ray" cast of mouse given mouse position and window width and height
		math::vec3 Camera::worldRayVec(const float xpos, const float ypos, const float width, const float height) {
			using namespace math;

			// viewport to NDC space
			float x = (2.0f * xpos) / width - 1.0f;
			float y = 1.0f - (2.0f * ypos) / height;
			// NDC to Homogeneous Clip space
			vec4 ray(x, y, 1.0, 1.0);
			// Homo. Clip to Eye space
			ray = mat4::inverseMatrix(m_projection) * ray;
			// Eye to World space
			ray = mat4::inverseMatrix(m_view) * ray;
			// Normalize vec3
			float mag = sqrt(ray.x * ray.x + ray.y * ray.y + ray.z * ray.z);	// magnitude
			if (mag == 0.0f)
				return vec3(0.0f, 0.0f, 0.0f);

			return vec3(ray.x / mag, ray.y / mag, ray.z / mag);
		}
		
		void Camera::lookAt(math::vec3 center) {
			setOrientation(180.0f, 0.0, 1.0f, 0.0f);
			setPos(center);
		}
	}
}