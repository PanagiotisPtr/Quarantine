#ifndef SCREEN_GAME_H
#define SCREEN_GAME_H

#include <iostream>
#include <algorithm>

#include "Screen.h"

#include "../Level/Level.h"

#include "../Animation/Keyframe.h"

#include "../Object/Object.h"
#include "../Object/ObjectFactory.h"
#include "../Object/Camera.h"
#include "../Object/GrassPlane.h"
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

#include "../Simulation/Simulation.h"

#include "glm/vec3.hpp"

namespace Screen {

	class Game : public Screen {
	public:
		Game(ScreenTransitionFunc t)
		: Screen(t), boxCount(0), maxBoxes(0), level(0,0,0), boxDisplayId(0),
		gameEnded(false) {
			this->objects.emplace_back(Object::ObjectFactory<Object::Camera>::createPointer(
				glm::vec3{ 0.5, 2.75, -0.5 }, false)
			);
			this->objects.back()->setRot(glm::vec3{ -3.72529e-09, 1.39626, 0.0 });
		}
		
		void endGame() { this->gameEnded = true; }

		bool gameOver() { return this->gameEnded; }

		void resetGame() {
			glm::vec3 spos{ 0.5, 2.75, -0.5 };
			glm::vec3 sort{ -3.72529e-09, 1.39626,0 };

			auto& camera = this->getObjects().front();
			camera->runSequence(
				Animation::animate(
					{ camera->getPos(), camera->getRot(), camera->getScale() },
					{ spos, sort, camera->getScale() },
					100
				)
			);
			std::cout << "Reset!" << std::endl;
			this->gameEnded = false;
			this->clearItems();
			this->setupScene();
		}

		void endSimulation() { this->simulation.reset(); }
	protected:
		void update() override {}

		void attachEventHandlers() override {
			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);
				
				glm::vec3 tpos{ 0.540255, 1.14891, -2.21037 };
				glm::vec3 tort{ -0.03, 0.601261, 0 };

				if (e.key == GLFW_KEY_SPACE && e.action == GLFW_PRESS) {
					if (this->gameOver()) {
						this->endSimulation();
						this->resetGame();
						return;
					}
					auto& camera = this->getObjects().front();
					camera->runSequence(
						Animation::animate(
							{ camera->getPos(), camera->getRot(), camera->getScale() },
							{ tpos, tort, camera->getScale() },
							100
						)
					);

					this->simulation->loadLevel(this->level);
					this->simulation->startPandemic();
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
			this->level = Level::Level("assets/levels/level1.lvl");
			this->maxBoxes = this->level.boxCount;

			ColourIdGenerator::setCounter(this->objects.back()->getObjectId() + 10);
			
			// setup background
			this->objects.emplace_back(Object::ObjectFactory<Object::Grid<Object::GrassPlane> >::createPointer(
				glm::vec3{ 0,0,0 }, &this->level, [this](glm::vec3 pos, Object::Grid<Object::GrassPlane>::PlaceBoxFunc place,
				Object::Grid<Object::GrassPlane>::UnplaceBoxFunc unplace, bool occupied) {
					int tmpBoxCount = boxCount;
					if (occupied) {
						if (tmpBoxCount > 0) {
							tmpBoxCount--;
						}
						unsigned boxId = unplace();
						if (boxId == 0) {
							return;
						}
						this->deleteObject(boxId);
						this->updateBoxCount(tmpBoxCount);
						return;
					}
					if (tmpBoxCount < maxBoxes) {
						tmpBoxCount++;
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
					this->updateBoxCount(tmpBoxCount);
				}
			));

			this->boxCount = 0;
			this->objects.emplace_back(
				Object::ObjectFactory<Object::Grid<Object::Box> >::createPointer(
					glm::vec3{ 1.5,0.8,0 }, this->maxBoxes, 1
				)
			);
			this->boxDisplayId = this->objects.back()->getObjectId();
			this->simulation = std::make_unique<Simulation::Simulation>(
				this->getObjects(),
				level,
				[this](){
					this->clearItems();
					this->getObjects().emplace_back(
						Object::ObjectFactory<Object::Image>::createPointer(glm::vec3{0,0,0}, "assets/win.bmp")
					);
					this->getObjects().back()->setScale({ 0.5, 0.5, 0.5 });
					this->getObjects().back()->moveAndPlaceObject({ 1.0, 0.0, 0.0 });
					this->getObjects().back()->scaleAndPlaceObject({ 0.5, 0.0, 0.0 });
					this->getObjects().back()->rotateAndPlaceObject({ 0.0, 0.4, 0.0 });
					this->endGame();
				},
				[this]() {
					this->clearItems();
					this->getObjects().emplace_back(
						Object::ObjectFactory<Object::Image>::createPointer(glm::vec3{ 0,0,0 }, "assets/lose.bmp")
					);
					this->getObjects().back()->setScale({ 0.5, 0.5, 0.5 });
					this->getObjects().back()->moveAndPlaceObject({ 1.0, 0.0, 0.0 });
					this->getObjects().back()->scaleAndPlaceObject({ 0.5, 0.0, 0.0 });
					this->getObjects().back()->rotateAndPlaceObject({ 0.0, 0.4, 0.0 });
					this->endGame();
				}
			);
		}

	private:
		unsigned boxDisplayId;
		std::unique_ptr<Simulation::Simulation> simulation;
		Level::Level level;
		bool gameEnded;
		int maxBoxes;
		int boxCount;

		void updateBoxCount(int newBoxCount) {
			if (newBoxCount == boxCount) return;
			this->boxCount = newBoxCount;
			this->deleteObject(this->boxDisplayId);

			if (newBoxCount == maxBoxes) return;

			this->objects.emplace_back(
				Object::ObjectFactory<Object::Grid<Object::Box> >::createPointer(
					glm::vec3{ 1.5,0.8,0 }, this->maxBoxes - this->boxCount, 1
				)
			);

			this->boxDisplayId = this->objects.back()->getObjectId();
		}
	};

} // namespace screen

#endif