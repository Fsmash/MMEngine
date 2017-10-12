#include "camera.h"

namespace mme {
	namespace graphics {

		void Camera::updateTranslation() {
			using namespace math;
			m_translation = mat4::translationMatrix(-m_pos.x, -m_pos.y, -m_pos.z);
			m_moved = false;
		}

		void Camera::updateRotation() {
			using namespace math;
			m_rotation_x = mat4::rotationMatrixX(-m_pitch);
			m_rotation_y = mat4::rotationMatrixY(-m_yaw);
			m_rotation_z = mat4::rotationMatrixZ(-m_roll);
			m_rotated = false;
		}

		Camera::Camera() {
			using namespace math;
			m_pos.x = m_pos.y = m_pos.z = 0.0f;
			m_translation = m_rotation_x = 
			m_rotation_y = m_rotation_z = mat4::identity();
			m_init = m_moved = m_rotated = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		Camera::Camera(const float x, const float y, const float z) {
			using namespace math;
			m_pos.x = x;
			m_pos.y = y;
			m_pos.z = z;
			m_translation = m_rotation_x =
			m_rotation_y = m_rotation_z = mat4::identity();
			m_init = m_moved = m_rotated = false;
			m_yaw = m_pitch = m_roll = 0.0f;
			m_near = 0.1f;
			m_far = 100.0f;
			m_fov = 67.0f;
			speed = yaw_speed = pitch_speed = roll_speed = 0.1f;
		}

		Camera::Camera(const math::vec3 cam_pos) {
			using namespace math;
			m_pos.x = cam_pos.x;
			m_pos.y = cam_pos.y;
			m_pos.z = cam_pos.z;
			m_translation = m_rotation_x =
			m_rotation_y = m_rotation_z = mat4::identity();
			m_init = m_moved = m_rotated = false;
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

		void Camera::right() { 
			m_pos.x -= speed; 
			m_moved = true;
		}
		
		void Camera::left() {
			m_pos.x += speed; 
			m_moved = true;
		}
		
		void Camera::up() { 
			m_pos.y -= speed; 
			m_moved = true;
		}
		
		void Camera::down() { 
			m_pos.y += speed; 
			m_moved = true;
		}
		
		void Camera::forward() { 
			m_pos.z -= speed; 
			m_moved = true;
		}
		
		void Camera::back() {
			m_pos.z += speed; 
			m_moved = true;
		}
			 
		void Camera::turnRight() { 
			m_yaw += yaw_speed;
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			m_rotated = true;
		}
		
		void Camera::turnLeft() { 
			m_yaw -= yaw_speed; 
			if (abs(m_yaw) > 360.0f) m_yaw = 0.0f;
			m_rotated = true;
		}
		
		void Camera::lookUp() { 
			m_pitch += pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			m_rotated = true;
		}
		
		void Camera::lookDown() {
			m_pitch -= pitch_speed; 
			if (abs(m_pitch) > 360.0f) m_pitch = 0.0f;
			m_rotated = true;
		}
		
		void Camera::tiltRight() {
			m_roll += roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			m_rotated = true;
		}

		void Camera::tiltLeft() {
			m_roll -= roll_speed; 
			if (abs(m_roll) > 360.0f) m_roll = 0.0f;
			m_rotated = true;
		}

		void Camera::init() {
			using namespace math;
			if (!m_init) {
				updateTranslation();
				updateRotation();
			}
			m_init = true;
		}

		math::mat4 Camera::viewMatrix() {
			using namespace math;
			if (!m_init) {
				std::cout << "Need to initialize eye view first." << std::endl;
				return mat4::identity();
			}
			if (m_moved) updateTranslation();
			if (m_rotated) updateRotation();
			return m_rotation_x * m_rotation_y * m_rotation_z * m_translation;
		}

		math::mat4 Camera::projMatrix(const float width, const float height) {
			float aspect = width / height;
			float range = tan(m_fov * 0.5f) * m_near;
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