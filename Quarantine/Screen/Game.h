#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

#include <iostream>
#include <algorithm>

#include "Screen.h"

#include "../Animation/Keyframe.h"

#include "../Level/Level.h"

#include "../Object/Object.h"
#include "../Object/ObjectFactory.h"
#include "../Object/Camera.h"
#include "../Object/Plane.h"
#include "../Object/Tree.h"
#include "../Object/Box.h"
#include "../Object/Grid.h"
#include "../Object/Image.h"
#include "../Object/Cube.h"
#include "../Object/Sphere.h"
#include "../Object/Cylinder.h"
#include "../Object/Button.h"
#include "../Object/Light.h"

#include "glm/vec3.hpp"

namespace Screen {

	class Game : public Screen {
	public:
		Game(ScreenTransitionFunc t) : Screen(t), boxCount(0), maxBoxes(0){}
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
			Level::Level level("assets/levels/level_1.map");
			this->maxBoxes = level.boxCount;
			this->objects.emplace_back(Object::ObjectFactory<Object::Camera>::createPointer(
				glm::vec3{ 0.5, 2.75, -0.5 }, false)
			);
			this->objects.back()->setRot(glm::vec3 { -3.72529e-09, 1.39626, 0.0 });

			// setup background
			this->objects.emplace_back(Object::ObjectFactory<Object::Grid>::createPointer(
				glm::vec3{ 0,0,0 }, level, [this](glm::vec3 pos, Object::Grid::PlaceBoxFunc place,
				Object::Grid::UnplaceBoxFunc unplace, bool occupied) {
					std::cout << std::boolalpha << occupied << std::endl;
					if (occupied) {
						if (boxCount > 0) {
							boxCount--;
						}
						unsigned boxId = unplace();
						if (boxId == 0) {
							return;
						}
						this->deleteObject(boxId);
						std::cout << "deleting: " << boxId << std::endl;
						return;
					}
					if (boxCount < maxBoxes) {
						boxCount++;
					}
					else {
						return;
					}
					pos.y += 0.04f;
					pos.z -= 0.04f;
					this->getObjects().emplace_back(
						Object::ObjectFactory<Object::Box>::createPointer(pos)
					);
					this->getObjects().back()->scaleAndPlaceObject({ -0.96f, -0.96f, -0.96f });
					place(this->getObjects().back()->getObjectId());
				}
			));
		}

	private:
		int maxBoxes;
		int boxCount;
	};

} // namespace screen

#endif