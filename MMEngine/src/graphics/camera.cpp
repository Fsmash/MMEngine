#include "camera.h"

namespace mme {
	namespace graphics {

		void Camera::createVersor(float *const q, const float angle, const float x, const float y, const float z) {
			float rad = angle * RADIANS;
			q[0] = cos(rad * 0.5f);		// w, angle
			q[1] = sin(rad * 0.5f) * x;	// x comp
			q[2] = sin(rad * 0.5f) * y;	// y comp
			q[3] = sin(rad * 0.5f) * z;	// z comp
		}

		void Camera::normalizeVersor() {
			float sum = m_quat[0] * m_quat[0] + m_quat[1] * m_quat[1] + m_quat[2] * m_quat[2] + m_quat[3] * m_quat[3];
			// only compute sqrt if sum meets threshold
			if (fabs(1.0f - sum) < 0.0001f) return;
			float mag = sqrt(sum);
			for (int i = 0; i < 4, i++;) {
				m_quat[i] /= mag;
			}
		}

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

		void Camera::quatToMatrix() {
			float w = m_quat[0];
			float x = m_quat[1];
			float y = m_quat[2];
			float z = m_quat[3];
			
			m_rotation.columns[0].x = 1.0f - 2.0f * y * y - 2.0f * z * z;
			m_rotation.columns[0].y = 2.0f * x * y + 2.0f * w * z;
			m_rotation.columns[0].z = 2.0f * x * z - 2.0f * w * y;
			m_rotation.columns[0].w = 0.0f;

			m_rotation.columns[1].x = 2.0f * x * y - 2.0f * w * z;
			m_rotation.columns[1].y = 1.0f - 2.0f * x * x - 2.0f * z * z;
			m_rotation.columns[1].z = 2.0f * y * z + 2.0f * w * x;
			m_rotation.columns[1].w = 0.0f;

			m_rotation.columns[2].x = 2.0f * x * z + 2.0f * w * y;
			m_rotation.columns[2].y = 2.0f * y * z - 2.0f * w * x;
			m_rotation.columns[2].z = 1.0f - 2.0f * x * x - 2.0f * y * y;
			m_rotation.columns[2].w = 0.0f;

			m_rotation.columns[3].x = 0.0f;
			m_rotation.columns[3].y = 0.0f;
			m_rotation.columns[3].z = 0.0f;
			m_rotation.columns[3].w = 1.0f;
		}

		void Camera::updateTranslation() {
			using namespace math;
			m_translation = mat4::translationMatrix(-m_pos.x, -m_pos.y, -m_pos.z);

			std::cout << "updated translation: " << m_pos << std::endl;
		}

		void Camera::updateRotation() {
			quatToMatrix();
			// maintain current orientation
			m_right	  = m_rotation * math::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up	  = m_rotation * math::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = m_rotation * math::vec4(0.0f, 0.0f, -1.0f, 0.0f);

			std::cout << "updated rotation x: " << m_right << std::endl;
			std::cout << "updated rotation y: " << m_up << std::endl;
			std::cout << "updated rotation z: " << m_forward << std::endl;
		}

		Camera::Camera() {
			using namespace math;
			m_right	  = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up	  = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_pos.x = m_pos.y = m_pos.z = 0.0f;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_translation = m_rotation = mat4::identity();
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		Camera::Camera(const float x, const float y, const float z) {
			using namespace math;
			m_right	  = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up	  = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_translation = m_rotation =  mat4::identity();
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		Camera::Camera(const math::vec3 cam_pos) {
			using namespace math;
			m_right	  = vec4(1.0f, 0.0f, 0.0f, 0.0f);
			m_up	  = vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_forward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
			m_pos.x = cam_pos.x;
			m_pos.y = cam_pos.y;
			m_pos.z = cam_pos.z;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;
			m_translation = m_rotation = mat4::identity();
			m_init = m_moved = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		void Camera::setPos(const float x, const float y, const float z) {
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
		}
		/*
		void Camera::rotateX(const float angle) {
			m_rotation *= math::mat4::rotationMatrixX(angle);
			m_moved = true;
		}
		
		void Camera::rotateY(const float angle) {
			m_rotation *= math::mat4::rotationMatrixX(angle);
			m_moved = true;
		}
		
		void Camera::rotateZ(const float angle) {
			m_rotation *= math::mat4::rotationMatrixX(angle);
			m_moved = true;
		}
		*/

		void Camera::right() { 
			m_vel.x += speed; 
			m_moved = true;
		}
		
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
			 
		void Camera::turnRight() { 
			m_yaw += yaw_speed;
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			float q_yaw[4];
			createVersor(q_yaw, m_yaw, m_up.x, m_up.y, m_up.z);
			multVersor(q_yaw);
			updateRotation();
			m_moved = true;
		}
		
		void Camera::turnLeft() { 
			m_yaw -= yaw_speed; 
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			float q_yaw[4];
			createVersor(q_yaw, m_yaw, m_up.x, m_up.y, m_up.z);
			multVersor(q_yaw);
			updateRotation();
			m_moved = true;
		}
		
		void Camera::lookUp() { 
			m_pitch += pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			float q_pitch[4];
			createVersor(q_pitch, m_pitch, m_right.x, m_right.y, m_right.z);
			multVersor(q_pitch);
			updateRotation();
			m_moved = true;
		}
		
		void Camera::lookDown() {
			m_pitch -= pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			float q_pitch[4];
			createVersor(q_pitch, m_pitch, m_right.x, m_right.y, m_right.z);
			multVersor(q_pitch);
			updateRotation();
			m_moved = true;
		}
		
		void Camera::tiltRight() {
			m_roll += roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			float q_roll[4];
			createVersor(q_roll, m_roll, m_forward.x, m_forward.y, m_forward.z);
			multVersor(q_roll);
			updateRotation();
			m_moved = true;
		}

		void Camera::tiltLeft() {
			m_roll -= roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			float q_roll[4];
			createVersor(q_roll, m_roll, m_forward.x, m_forward.y, m_forward.z);
			multVersor(q_roll);
			updateRotation();
			m_moved = true;
		}

		void Camera::init(const float angle, const float x, const float y, const float z) {
			if (!m_init) {
				createVersor(m_quat, angle, x, y, z);
				quatToMatrix();
				updateTranslation();
			}
			m_init = true;
		}

		math::mat4 Camera::viewMatrix() {
			if (!m_init) {
				std::cout << "Need to initialize eye view first." << std::endl;
				return math::mat4::identity();
			}

			// pretty sure I don't need to update this again.
			//updateRotation();

			m_pos += math::vec3(m_right.x, m_right.y, m_right.z).scale(m_vel.x);
			m_pos += math::vec3(m_up.x, m_up.y, m_up.z).scale(m_vel.y);
			m_pos += math::vec3(m_forward.x, m_forward.y, m_forward.z).scale(-m_vel.z);

			std::cout << "cam position: " << m_pos << std::endl;

			updateTranslation();

			m_moved = false;

			m_yaw = m_pitch = m_roll = 0.0f;
			m_vel.x = m_vel.y = m_vel.z = 0.0f;

			return m_rotation * m_translation;
		}

		math::mat4 Camera::projMatrix(const int width, const int height) {
			float aspect = (float) width / (float) height;
			float range = tan(m_fov * RADIANS * 0.5f) * m_near;
			float Sx = (2.0f * m_near) / (range * aspect + range * aspect);
			float Sy = m_near / range;
			float Sz = -(m_far + m_near) / (m_far - m_near);
			float Pz = -(2.0f * m_far * m_near) / (m_far - m_near);

			math::mat4 projection;
			projection.columns[0] = math::vec4(Sx, 0.0f, 0.0f, 0.0f);
			projection.columns[1] = math::vec4(0.0f, Sy, 0.0f, 0.0f);
			projection.columns[2] = math::vec4(0.0f, 0.0f, Sz, -1.0f);
			projection.columns[3] = math::vec4(0.0f, 0.0f, Pz, 0.0f);
			
			return projection;
		}

	}
}