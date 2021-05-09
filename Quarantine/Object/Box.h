#ifndef OBJECT_BOX_H
#define OBJECT_BOX_H

#include <cmath>

#include "Composite.h"
#include "Image.h"

namespace Object {

	class Box : public Composite {
	public:
		Box(glm::vec3 p) : Composite(p) {
			float pi = std::acos(-1.0f);

			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,0.0f });

			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ 1.0f,0.0f,1.0f });
			this->objects.back()->rotateAndPlaceObject({ pi / 2,0.0f,0.0f });

			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ -1.0f,0.0f,1.0f });
			this->objects.back()->rotateAndPlaceObject({ -pi / 2,0.0f,0.0f });
			
			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.0f,2.0f });
			
			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ 0.0f,1.0f,1.0f });
			this->objects.back()->rotateAndPlaceObject({ 0.0f,pi / 2,0.0f });

			this->objects.emplace_back(new Image({ 0,0,0 }, "assets/box.bmp"));
			this->objects.back()->moveAndPlaceObject({ 0.0f,-1.0f,1.0f });
			this->objects.back()->rotateAndPlaceObject({ 0.0f,pi / 2,0.0f });
		}

	protected:
		std::string objectName() const { return "TREE"; }
	};

} // namespace Object

#endif