#ifndef SCREEN_SCREEN_H
#define SCREEN_SCREEN_H

#include <vector>
#include <queue>
#include <memory>

#include "Object.h"
#include "Camera.h"
#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Light.h"

#include "Global.h"

namespace Screen {

	class Screen {
	public:
		using ObjectPtr = std::unique_ptr<Object::Object>;

		Screen() {
			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_1 && e.action == GLFW_PRESS) {
					this->objectsQueue.push(ObjectClasses::PLANE);
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

		std::vector<ObjectPtr>& getObjects() { return this->objects; }

		void deleteObject(unsigned objectId) { deletionQueue.push(objectId); }
	protected:
		std::vector<ObjectPtr> objects;
		std::queue<ObjectClasses> objectsQueue;
		std::queue<unsigned> deletionQueue;

		virtual void update() = 0;

		void createQueuedObjects() {
			while (!this->objectsQueue.empty()) {
				switch (this->objectsQueue.front()) {
				case ObjectClasses::SPHERE:
					this->objects.emplace_back(new Object::Sphere({ 0, 0, 0 }, { 0.8f, 0.2f, 0.2f }));
					break;
				case ObjectClasses::PLANE:
					this->objects.emplace_back(new Object::Plane({ 0, 0, 0 }, { 0.8f, 0.2f, 0.2f }));
					break;
				case ObjectClasses::CUBE:
					this->objects.emplace_back(new Object::Cube({ 0, 0, 0 }, { 0.8f, 0.2f, 0.2f }));
					break;
				case ObjectClasses::CYLINDER:
					this->objects.emplace_back(new Object::Cylinder({ 0, 0, 0 }, { 0.8f, 0.2f, 0.2f }));
					break;
				case ObjectClasses::LIGHT:
					this->objects.emplace_back(new Object::Light({ 0, 0, 0 }));
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
	};

} // namespace screen

#endif