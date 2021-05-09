#ifndef OBJECT_PYRAMID_H
#define OBJECT_PYRAMID_H

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include "GL/glew.h"

#include "Editable.h"

namespace Object {

	class Pyramid : public Editable {
	public:
		Pyramid(glm::vec3 p, glm::vec3 c) : Editable(p, c) {}

	protected:
		void drawShape() const override {
			glBegin(GL_TRIANGLES);

			glVertex3f(-1.0f, 0, -1.0f);
			glVertex3f(-1.0f, 0, 1.0f);
			glVertex3f(1.0f, 0, 1.0f);

			glVertex3f(1.0f, 0, 1.0f);
			glVertex3f(1.0f, 0, -1.0f);
			glVertex3f(-1.0f, 0, -1.0f);


			glVertex3f(-1.0f, 0, -1.0f);
			glVertex3f(1.0f, 0, -1.0f);
			glVertex3f(0, 1.0f, 0);

			glVertex3f(1.0f, 0, -1.0f);
			glVertex3f(1.0f, 0, 1.0f);
			glVertex3f(0, 1.0f, 0);

			glVertex3f(1.0f, 0, 1.0f);
			glVertex3f(-1.0f, 0, 1.0f);
			glVertex3f(0, 1.0f, 0);

			glVertex3f(-1.0f, 0, 1.0f);
			glVertex3f(-1.0f, 0, -1.0f);
			glVertex3f(0, 1.0f, 0);

			glEnd();
		}

		std::string objectName() const { return "PYRAMID"; }
	};

} // namespace Object

#endif