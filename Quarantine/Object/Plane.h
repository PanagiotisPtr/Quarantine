#ifndef OBJECT_PLANE_H
#define OBJECT_PLANE_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/common.hpp"

#include "Editable.h"

namespace Object {

	class Plane : public Editable {
	public:
		Plane(glm::vec3 p, glm::vec3 c) : Editable(p, c) {}

	protected:
		void drawShape() const override {
			glBegin(GL_TRIANGLES);

			glVertex3f(-1, 0, 1);
			glVertex3f(1, 0, 1);
			glVertex3f(1, 0, -1);

			glVertex3f(1, 0, -1);
			glVertex3f(-1, 0, -1);
			glVertex3f(-1, 0, 1);

			glEnd();
		}

		std::string objectName() const { return "PLANE"; }
	};

} // namespace Object

#endif