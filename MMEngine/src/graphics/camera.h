/*	Code by Bryant Dinh for Senior Sem 2017 Project : Misunderstood Magician Engine
	
	This class, Camera, holds all the relevant data to construct a view and projection matrix for the Transformation Pipeline. 
	It handles all changes to camera position (translation) and orientation (rotation) with member functions. 
	Additionally, this class also has a member function that return ray cast of mouse position as calculations that require the projection and view matrix. 
*/

#pragma once
#include "../math/mat4.h"

namespace mme {
	namespace graphics {
	
		class Camera{

		private:
			
			math::vec4 m_right;			// x axis, defines what is right
			math::vec4 m_up;			// y axis
			math::vec4 m_forward;		// z axis
			math::vec3 m_pos;			// camera position
			math::vec3 m_vel;			// velocity component to translate camera
			math::mat4 m_translation;	// translation matrix, for view matrix calculation
			math::mat4 m_rotation;		// rotation matrix, for view matrix calculation
			math::mat4 m_view;			// view matrix
			math::mat4 m_projection;	// projection matrix
			
			bool m_moved;		// update view if camera moved
			bool m_init;		// check whether camera has been initialized
			
			float m_pitch;		// x rotation angle
			float m_yaw;		// y rotation angle
			float m_roll;		// z rotation angle

			float m_quat[4];	// main quat versor, better as a float. doesn't follow same behaivior as a vector
			
			float m_near;		// set cut off near plane for projection frustrum
			float m_far;		// set cut off far plane for projection frustrum
			float m_fov;		// angle in degrees of field of vision

			void createVersor(float *const q, const float angle, const float x, const float y, const float z);
			void normalizeVersor();
			void multVersor(float *const q);
			void quatToMatrix();

		public:

			float speed;		// speed of translation
			float yaw_speed;	// speed of y axis rotation
			float pitch_speed;	// speed of x axis rotation
			float roll_speed;	// speed of z axis rotation

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
			void setOrientation(const float angle, const float x, const float y, const float z);
			math::mat4 viewMatrix();
			math::mat4 viewMatrixUpdate();
			math::mat4 projMatrix(const int width, const int height);
			math::vec3 worldRayVec(const float xpos, const float ypos, const float width, const float height);
			// TO DO: work on look at function eventually.
			//math::mat4 lookAt(math::vec3 cam_pos, math::vec3 target, math::vec3 up);

		};
	}
}