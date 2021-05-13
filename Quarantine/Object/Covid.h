#ifndef OBJECT_COVID_H
#define OBJECT_COVID_H

#include <cmath>

#include "Composite.h"
#include "ObjectFactory.h"
#include "Sphere.h"
#include "Crown.h"

namespace Object {

	class Covid : public Composite {
	public:
		Covid(glm::vec3 p) : Composite(p) {
			this->lock();
			float pi = std::acos(-1.0f);

			ColourIdGenerator::useStaticId(this->getObjectId());

			this->objects.emplace_back(ObjectFactory<Sphere>::createPointer(
				glm::vec3{ 0,0,0 }, glm::vec3{ 1.0, 0.0, 0.0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });

			this->objects.emplace_back(ObjectFactory<Crown>::createPointer(
				glm::vec3{ 0,0,0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });

			this->objects.emplace_back(ObjectFactory<Crown>::createPointer(
				glm::vec3{ 0,0,0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });
			this->objects.back()->rotateAndPlaceObject({ pi / 2,0.0f,0.0f });

			this->objects.emplace_back(ObjectFactory<Crown>::createPointer(
				glm::vec3{ 0,0,0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });
			this->objects.back()->rotateAndPlaceObject({ pi / 4,0.0f,0.0f });

			this->objects.emplace_back(ObjectFactory<Crown>::createPointer(
				glm::vec3{ 0,0,0 })
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });
			this->objects.back()->rotateAndPlaceObject({ -pi / 4,0.0f,0.0f });

			ColourIdGenerator::useDynamicId();

			this->scaleAndPlaceObject({ -0.8f,-0.8f,-0.8f });
			this->animates = true;
		}

		virtual void select() override {}
	protected:
		std::string objectName() const { return "COVID"; }
	};

} // namespace Object

#endif