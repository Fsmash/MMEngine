#pragma once
#include "../math/mat4.h"

namespace mme {
	namespace graphics {
	
		class Camera{

		private:
			
			math::vec4 m_right;			// x axis, defines what is right
			math::vec4 m_up;				// y axis
			math::vec4 m_forward;			// z axis
			math::vec3 m_pos;			// camera position
			math::vec3 m_vel;
			math::mat4 m_translation;	// translation matrix for view matrix calculation
			math::mat4 m_rotation;		// rotation matrix for view matrix calculation
			
			bool m_moved;
			bool m_init;
			
			float m_pitch;		// x rotation angle
			float m_yaw;		// y rotation angle
			float m_roll;		// z rotation angle

			float m_quat[4];	// main quat versor, better as a float doesn't follow same behaivior as a vector
			
			float m_near;	
			float m_far;
			float m_fov;

			void createVersor(float *const q, const float angle, const float x, const float y, const float z);
			void normalizeVersor();
			void multVersor(float *const q);
			void quatToMatrix();

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

			inline bool update() { return m_moved; }
			void init(const float angle, const float x, const float y, const float z);
			math::mat4 viewMatrix();
			math::mat4 viewMatrixUpdate();
			math::mat4 projMatrix(const int width, const int height);
			//math::mat4 lookAt(math::vec3 cam_pos, math::vec3 target, math::vec3 up);

		};
	}
}