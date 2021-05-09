#include "Global.h"

namespace Global {

	Event::EventBus EventBus = Event::EventBus();
	GLFWwindow* Window = NULL;
	glm::dvec2 Cursor = { 0.0f, 0.0f };
	unsigned ObjectId = 99999;

} // namespace Global