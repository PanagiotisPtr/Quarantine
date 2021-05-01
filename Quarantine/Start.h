#ifndef SCREEN_START_H
#define SCREEN_START_H

#include "Screen.h"

namespace Screen {

	class Start : public Screen {
	public:
		Start() : Screen() {
			this->objects.emplace_back(new Object::Camera({ 0, 0.25, -2 }));
		}
	protected:
		void update() override {}
	};

} // namespace screen

#endif