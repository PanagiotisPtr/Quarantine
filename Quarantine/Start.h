#ifndef SCREEN_START_H
#define SCREEN_START_H

#include "Screen.h"

namespace Screen {

	class Start : public Screen {
	public:
		Start() : Screen() {
			this->objects.emplace_back(new Object::Camera({ 0, 0.25, -2 }, true));
		}
	protected:
		void update() override {}

		void attachEventHandlers() override {
			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_1 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::IMAGE);
				}
				if (e.key == GLFW_KEY_2 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::CUBE);
				}
				if (e.key == GLFW_KEY_3 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::SPHERE);
				}
				if (e.key == GLFW_KEY_4 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::CYLINDER);
				}
				if (e.key == GLFW_KEY_L && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::LIGHT);
				}
			}, Global::ObjectId);

			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_X && e.action == GLFW_PRESS) {
					for (auto& o : this->objects) {
						if (o->isSeleted() && !o->isChanging()) this->deleteObject(o->getObjectId());
					}
				}
			}, Global::ObjectId);
		}
	};

} // namespace screen

#endif