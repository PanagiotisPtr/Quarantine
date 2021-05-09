#ifndef OBJECT_COMPOSITE_H
#define OBJECT_COMPOSITE_H

#include "Editable.h"
#include "Pyramid.h"
#include "Cylinder.h"

namespace Object {

	class Composite : public Editable {
	public:
		using EditableObjectPtr = std::unique_ptr<Editable>;

		Composite(glm::vec3 p) : Editable(p, { 0.0f,0.0f,0.0f }) {
			this->objects.emplace_back(new Pyramid({ 0,0,0 }, { 0.0f,1.0f,0.0f }));
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0 });

			this->objects.emplace_back(new Pyramid({ 0,0,0 }, { 0.0f,1.0f,0.0f }));
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.5f,0.0 });
			this->objects.back()->scaleAndPlaceObject({ -0.25f,-0.25f,-0.25f });

			this->objects.emplace_back(new Pyramid({ 0,0,0 }, { 0.0f,1.0f,0.0f }));
			this->objects.back()->moveAndPlaceObject({ 0.0f, 1.0f,0.0 });
			this->objects.back()->scaleAndPlaceObject({ -0.5f,-0.5f,-0.5f });

			this->objects.emplace_back(new Cylinder({ 0,0,0 }, { 1.0f,0.3f,0.0f }));
			this->objects.back()->scaleAndPlaceObject({ -0.85f,-0.85f,-0.85f });
			this->objects.back()->scaleAndPlaceObject({ 0.0f,0.65f,0.0f });
			this->objects.back()->moveAndPlaceObject({ 0.0f, -0.51f,0.0 });
		}

		virtual void drawShape() const override {
			for (auto& o : this->objects) {
				o->drawWithoutOutline();
			}
		}

		virtual void drawWithId() const {
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
	private:
		std::vector<EditableObjectPtr> objects;
	};

} // namespace Object

#endif