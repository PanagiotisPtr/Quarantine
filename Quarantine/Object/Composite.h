#ifndef OBJECT_COMPOSITE_H
#define OBJECT_COMPOSITE_H

#include "Editable.h"
#include "Pyramid.h"
#include "Cylinder.h"

namespace Object {

	class Composite : public Editable {
	public:
		using EditableObjectPtr = std::unique_ptr<Editable>;

		Composite(glm::vec3 p) : Editable(p, { 0.0f,0.0f,0.0f }) {}

		virtual void drawShape() const override {
			for (auto& o : this->objects) {
				o->drawWithoutOutline();
			}
		}

		virtual void drawWithId() {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(this->getTransform()));

			glColor3f(this->colourId.r, this->colourId.g, this->colourId.b);
			for (auto& o : this->objects) {
				o->drawWithColour({ this->colourId.r, this->colourId.g, this->colourId.b });
			}

			glPopMatrix();
		}

		std::string objectName() const { return "COMPOSITE"; }
	protected:
		std::vector<EditableObjectPtr> objects;
	};

} // namespace Object

#endif