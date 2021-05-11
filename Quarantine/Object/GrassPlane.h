#ifndef OBJECT_GRASSPLANE_H
#define OBJECT_GRASSPLANE_H

#include "Image.h"

namespace Object {

	class GrassPlane : public Image {
	public:
		GrassPlane(glm::vec3 p) : Image(p, "assets/grass.bmp") {}
	};

} // namespace Object

#endif