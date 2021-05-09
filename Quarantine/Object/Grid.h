#ifndef OBJECT_GRID_H
#define OBJECT_GRID_H

#include <cmath>

#include "Composite.h"
#include "Plane.h"

namespace Object {

	class Grid : public Composite {
	public:
		Grid(glm::vec3 p, size_t width, size_t height) : Composite(p) {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					this->objects.emplace_back(new Plane({ 0,0,0 }, { 1.0f,0.0f,0.0f }));
					this->objects.back()->scaleAndPlaceObject({ -0.95f,-0.95f,-0.95f });
					this->objects.back()->moveAndPlaceObject({ (float)i * 2.5f,0.0f, (float)j * 2.5f });
				}
			}
		}

	protected:
		std::string objectName() const { return "GRID"; }
	};

} // namespace Object

#endif