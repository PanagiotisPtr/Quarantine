#ifndef OBJECT_IMAGE_H
#define OBJECT_IMAGE_H

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
#include "ImageLoader.h"

namespace Object {

	class Image : public Editable {
	public:
		Image(glm::vec3 p, const char* textureLocation)
		: Editable(p, { 0.0f, 0.0f, 0.0f }), texture(Utility::ImageLoader::loadImage(textureLocation)) {
			this->lock();
		}

	protected:
		void drawOutline() const override {}

		void drawShape() const override {
			glBegin(GL_TRIANGLES);
			
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			// images are renderd on both sides

			glVertex3f(1.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			
			glEnd();
		}

		void drawObject() const override {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(this->getTransform()));

			this->drawShapeWithTexture();

			glPopMatrix();
		}

		virtual void drawWithId() const override {
			glColor3f(this->colourId.r, this->colourId.g, this->colourId.b);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(this->getTransform()));

			this->drawShape();

			glPopMatrix();
		}

		std::string objectName() const { return "IMAGE"; }
	private:
		GLuint texture;

		void drawShapeWithTexture() const {
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, this->texture);

			glBegin(GL_TRIANGLES);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			// images are renderd on both sides

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glEnd();

			glDisable(GL_TEXTURE_2D);
		}
	};

} // namespace Object

#endif