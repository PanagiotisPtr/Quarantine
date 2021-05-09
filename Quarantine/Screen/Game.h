#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

#include "Screen.h"

#include "../Object/Object.h"
#include "../Object/Camera.h"
#include "../Object/Plane.h"
#include "../Object/Composite.h"
#include "../Object/Image.h"
#include "../Object/Cube.h"
#include "../Object/Sphere.h"
#include "../Object/Cylinder.h"
#include "../Object/Button.h"
#include "../Object/Light.h"

namespace Screen {

	class Game : public Screen {
	public:
		Game(ScreenTransitionFunc t) : Screen(t) {}
	protected:
		void update() override {}

		void attachEventHandlers() override {
			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_1 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::PYRAMID);
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
			}, this->getId());

			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_X && e.action == GLFW_PRESS) {
					for (auto& o : this->objects) {
						if (o->isSeleted() && !o->isChanging() && !o->isLocked()) this->deleteObject(o->getObjectId());
					}
				}
			}, this->getId());
		}

		void setupScene() override {
			this->objects.emplace_back(new Object::Camera({ 0, 0.25, -2 }, false));

			// setup background
			this->objects.emplace_back(new Object::Composite({ 0,0,0 }));

		}
	};

} // namespace screen

#endif