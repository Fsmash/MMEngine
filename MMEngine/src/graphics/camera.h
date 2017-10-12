#pragma once
#include "../math/mat4.h"

namespace mme {
	namespace graphics {
	
		class Camera{

		private:
			
			math::vec3 m_pos;
			math::mat4 m_translation;
			math::mat4 m_rotation_x;
			math::mat4 m_rotation_y;
			math::mat4 m_rotation_z;
			
			bool m_moved;
			bool m_rotated;
			bool m_init;
			float m_pitch;		// rotation x axis
			float m_yaw;		// y axis
			float m_roll;		// z axis

			float m_near;	
			float m_far;
			float m_fov;

			void updateTranslation();
			void updateRotation();

		public:

			float speed;
			float yaw_speed;
			float pitch_speed;
			float roll_speed;

			Camera();
			Camera(const float x, const float y, const float z);
			Camera(const math::vec3 cam_pos);

			void setPos(const float x, const float y, const float z);
			inline void setPos(const math::vec3 cam) { m_pos = cam; }
			inline void setYaw(const float angle)	 { m_yaw = angle; }
			inline void setPitch(const float angle)  { m_pitch = angle; }
			inline void setRoll(const float angle)	 { m_roll = angle; }
			inline void setNear(const float dist)	 { m_near = dist; }
			inline void setFar(const float dist)	 { m_far = dist; }
			inline void setFOV(const float angle)	 { m_fov = angle; }

			inline math::vec3 getPos() const { return m_pos; }
			inline float getYaw() const      { return m_yaw; }
			inline float getPitch() const	 { return m_pitch; }
			inline float getRoll() const	 { return m_roll; }
			inline float getNear() const	 { return m_near; }
			inline float getFar() const		 { return m_far; }
			inline float getFOV() const		 { return m_fov; }

			void right();
			void left();
			void up();
			void down();
			void forward();
			void back();

			void turnRight();
			void turnLeft();
			void lookUp();
			void lookDown();
			void tiltRight();
			void tiltLeft();

			void init();
			inline bool update() { return m_moved || m_rotated; }
			math::mat4 viewMatrix();
			math::mat4 projMatrix(const float width, const float height);
			//math::mat4 lookAt(math::vec3 cam_pos, math::vec3 target, math::vec3 up);

		};
	}
}