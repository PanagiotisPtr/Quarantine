#ifndef SCREEN_START_H
#define SCREEN_START_H

#include "Screen.h"
#include "ScreenFactory.h"
#include "Game.h"
#include "Exit.h"

#include "../Object/Object.h"
#include "../Object/Camera.h"
#include "../Object/Plane.h"
#include "../Object/Image.h"
#include "../Object/Cube.h"
#include "../Object/Sphere.h"
#include "../Object/Cylinder.h"
#include "../Object/Button.h"
#include "../Object/Light.h"

namespace Screen {

	class Start : public Screen {
	public:
		Start(ScreenTransitionFunc t) : Screen(t) {}
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
			this->objects.emplace_back(Object::ObjectFactory<Object::Camera>::createPointer(
				glm::vec3{ 0, 0.25, -2 }, true)
			);

			// setup background
			this->objects.emplace_back(Object::ObjectFactory<Object::Image>::createPointer(
				glm::vec3{ 0,0,0 }, "assets/mainScreen.bmp")
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.5f,0.0 });
			this->objects.back()->scaleAndPlaceObject({ -0.4f,-0.4f,-0.4f });
			this->objects.back()->scaleAndPlaceObject({ 0.65f,0.1f,0.65f });

			// setup 'play' button
			this->objects.emplace_back(Object::ObjectFactory<Object::Button>::createPointer(
				glm::vec3{ 0,0,0 }, "assets/play.bmp", [this]() {
					this->transitionScreen(ScreenFactory<Game>::createPointer(this->screenTransitionFunction));
				})
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.8f,-0.01f });
			this->objects.back()->scaleAndPlaceObject({ -0.7f,-0.9f,-0.7f });

			// setup 'exit' button
			this->objects.emplace_back(Object::ObjectFactory<Object::Button>::createPointer(
				glm::vec3{ 0,0,0 }, "assets/exit.bmp", [this]() {
					this->transitionScreen(ScreenFactory<Exit>::createPointer(this->screenTransitionFunction));
				})
			);
			this->objects.back()->moveAndPlaceObject({ 0.0f,0.3f,-0.01f });
			this->objects.back()->scaleAndPlaceObject({ -0.7f,-0.9f,-0.7f });
		}
	};

} // namespace screen

#endif