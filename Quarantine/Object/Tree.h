#ifndef OBJECT_TREE_H
#define OBJECT_TREE_H

#include "Composite.h"

namespace Object {

	class Tree : public Composite {
	public:
		Tree(glm::vec3 p) : Composite(p) {
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

	protected:
		std::string objectName() const { return "TREE"; }
	};

} // namespace Object

#endif