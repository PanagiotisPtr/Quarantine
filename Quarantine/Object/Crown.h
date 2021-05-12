#ifndef OBJECT_CROWN_H
#define OBJECT_CROWN_H

#include <cmath>

#include "Composite.h"
#include "ObjectFactory.h"
#include "Sphere.h"
#include "cube.h"

namespace Object {

	class Crown : public Composite {
	public:
		Crown(glm::vec3 p) : Composite(p) {
			this->lock();
			float pi = std::acos(-1.0f);

			ColourIdGenerator::useStaticId(this->getObjectId());

			this->objects.emplace_back(ObjectFactory<Cube>::createPointer(
				glm::vec3{ 0,0,0 }, glm::vec3{ 1.0, 0.0, 0.0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });
			this->objects.back()->scaleAndPlaceObject({ -0.8f,-0.8f,1.5f });

			this->objects.emplace_back(ObjectFactory<Sphere>::createPointer(
				glm::vec3{ 0,0,0 }, glm::vec3{ 1.0, 0.0, 0.0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,2.2f });
			this->objects.back()->scaleAndPlaceObject({ -0.55f,-0.55f,-0.55f });

			this->objects.emplace_back(ObjectFactory<Sphere>::createPointer(
				glm::vec3{ 0,0,0 }, glm::vec3{ 1.0, 0.0, 0.0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,-2.2f });
			this->objects.back()->scaleAndPlaceObject({ -0.55f,-0.55f,-0.55f });

			ColourIdGenerator::useDynamicId();
		}

		virtual void select() override {}
	protected:
		std::string objectName() const { return "COVID"; }
	};

} // namespace Object

#endif