#ifndef SCREEN_SCREEN_H
#define SCREEN_SCREEN_H

#include <vector>
#include <queue>
#include <memory>

#include "ScreenIdGenerator.h"

#include "../Object/Object.h"
#include "../Object/ObjectFactory.h"
#include "../Object/Camera.h"
#include "../Object/Plane.h"
#include "../Object/Pyramid.h"
#include "../Object/Image.h"
#include "../Object/Cube.h"
#include "../Object/Sphere.h"
#include "../Object/Cylinder.h"
#include "../Object/Button.h"
#include "../Object/Light.h"

#include "../Global/Global.h"

namespace Screen {

	class Screen {
	public:
		using EditableObjectPtr = std::unique_ptr<Object::Editable>;
		using ScreenTransitionFunc = std::function<void(Screen*)>;

		Screen(ScreenTransitionFunc t)
		: screenTransitionFunction(t), sceneSetupComplete(false), id(ScreenIdGenerator::getScreenId()) {}

		// has to be called after construction is finished
		// that's why a factory is used to create screens
		// to abstract away the need to call init() every time
		void init() {
			this->attachEventHandlers();
		}

		void draw(int windowWidth, int windowHeight) {
			this->update();
			this->createQueuedObjects();
			this->deleteQueuedObjets();

			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_NORMALIZE);
			glShadeModel(GL_SMOOTH);
			glDepthFunc(GL_LESS);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0, 0, windowWidth, windowHeight);

			if (!sceneSetupComplete) {
				this->setupScene();
				sceneSetupComplete = true;
			}

			// make sure that the first object is always the camera
			// draw non-selected objects first
			glStencilMask(0x00);
			for (const auto& o : this->objects) {
				if (o->isSeleted()) {
					continue;
				}
				o->draw();
			}

			// draw selected objects
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			for (const auto& o : this->objects) {
				if (!o->isSeleted()) {
					continue;
				}
				o->draw();
			}
		}

		std::vector<EditableObjectPtr>& getObjects() { return this->objects; }

		void deleteObject(unsigned objectId) { deletionQueue.push(objectId); }

		void clearScreen() {
			while (this->objects.size()) {
				auto& o = this->objects.back();
				Global::EventBus.detachHandlersForObject(o->getObjectId());
				this->objects.pop_back();
			}

			Global::EventBus.detachHandlersForObject(this->getId());
		}

		void transitionScreen(Screen* screen) {
			this->screenTransitionFunction(screen);
		}
	protected:
		std::vector<EditableObjectPtr> objects;
		std::queue<ObjectClasses> objectsQueue;
		std::queue<unsigned> deletionQueue;
		ScreenTransitionFunc screenTransitionFunction;

		virtual void update() = 0;

		virtual void attachEventHandlers() = 0;

		virtual void setupScene() = 0;

		unsigned getId() { return this->id; }

		void createQueuedObjects() {
			while (!this->objectsQueue.empty()) {
				switch (this->objectsQueue.front()) {
				case ObjectClasses::SPHERE:
					this->objects.emplace_back(Object::ObjectFactory<Object::Sphere>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 0.8f, 0.2f, 0.2f })
					);
					break;
				case ObjectClasses::PLANE:
					this->objects.emplace_back(Object::ObjectFactory<Object::Plane>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 0.8f, 0.2f, 0.2f })
					);
					break;
				case ObjectClasses::CUBE:
					this->objects.emplace_back(Object::ObjectFactory<Object::Cube>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 0.8f, 0.2f, 0.2f })
					);
					break;
				case ObjectClasses::CYLINDER:
					this->objects.emplace_back(Object::ObjectFactory<Object::Cylinder>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 0.8f, 0.2f, 0.2f })
					);
					break;
				case ObjectClasses::LIGHT:
					this->objects.emplace_back(Object::ObjectFactory<Object::Light>::createPointer(
						glm::vec3{ 0,0,0 })
					);
					break;
				case ObjectClasses::IMAGE:
					this->objects.emplace_back(Object::ObjectFactory<Object::Image>::createPointer(
						glm::vec3{ 0,0,0 }, "assets/mainScreen.bmp")
					);
					break;
				case ObjectClasses::PYRAMID:
					this->objects.emplace_back(Object::ObjectFactory<Object::Pyramid>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 0.8f, 0.2f, 0.2f })
					);
					break;
				}
				this->objectsQueue.pop();
			}
		}

		void deleteQueuedObjets() {
			while (!this->deletionQueue.empty()) {
				unsigned objectId = this->deletionQueue.front();
				this->deletionQueue.pop();

				Global::EventBus.detachHandlersForObject(objectId);

				this->objects.erase(std::remove_if(std::begin(this->objects), std::end(this->objects), [objectId](const auto& o) {
					return o->getObjectId() == objectId;
				}), std::end(this->objects));
			}
		}
	private:
		bool sceneSetupComplete;
		unsigned id;
	};

} // namespace screen

#endif