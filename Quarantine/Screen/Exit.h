#ifndef SCREEN_EXIT_H
#define SCREEN_EXIT_H

#include "Screen.h"

#include "../Global/Global.h"

#include "GLFW/glfw3.h"

namespace Screen {

	class Exit : public Screen {
	public:
		Exit(ScreenTransitionFunc t) : Screen(t) {}
	protected:
		void update() override { glfwDestroyWindow(Global::Window); }

		void attachEventHandlers() override {}

		void setupScene() override {}
	};

} // namespace screen

#endif