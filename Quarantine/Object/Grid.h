#ifndef OBJECT_GRID_H
#define OBJECT_GRID_H

#include <cmath>
#include <functional>

#include "Composite.h"
#include "../Global/Global.h"
#include "Plane.h"

#include "glm/vec3.hpp"

namespace Object {

	class Grid : public Composite {
	public:
		using CallbackFunc = std::function<void(glm::vec3)>;

		Grid(glm::vec3 p, size_t width, size_t height, CallbackFunc cb)
		: Composite(p), callback(cb) {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					this->objects.emplace_back(ObjectFactory<Plane>::createPointer(
						glm::vec3{ 0,0,0 }, glm::vec3{ 1.0f,0.0f,0.0f })
					);
					this->objects.back()->scaleAndPlaceObject({ -0.95f,-0.95f,-0.95f });
					this->objects.back()->moveAndPlaceObject({ (float)i * 2.5f,0.0f, (float)j * 2.5f });
				}
			}
			
			this->lock();
		}

		void select() override { }
	protected:
		std::string objectName() const { return "GRID"; }

		void attachEventHandlers() override {
			Global::EventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
				const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

				if (e.button != GLFW_MOUSE_BUTTON_LEFT || e.action != GLFW_PRESS) {
					return;
				}

				int windowHeight, windowWidth;
				glfwGetWindowSize(Global::Window, &windowWidth, &windowHeight);
				float pixel[4];
				int flippedY = windowHeight - (int)Global::Cursor.y;
				glfwGetCursorPos(Global::Window, &Global::Cursor.x, &Global::Cursor.y);

				for (auto& o : this->objects) {
					o->deselect();
				}
				glDisable(GL_LIGHTING);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, windowWidth, windowHeight);
				for (const auto& o : this->objects) {
					o->drawWithId();
				}
				glReadPixels((int)Global::Cursor.x, flippedY, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
				for (int i = 0; i < 4; i++) pixel[i] = this->roundToTwoDecimals(pixel[i]);

				for (auto& o : this->objects) {
					if (o->getObjectId() == ColourIdGenerator::decodeId({ pixel[0], pixel[1], pixel[2] })) {
						this->callback(o->getPos());
					}
				}

				glEnable(GL_LIGHTING);
			}, this->getObjectId());
		}
	private:
		CallbackFunc callback;

		float roundToTwoDecimals(float v) {
			v += 0.005f;

			return std::trunc(100 * v) / 100;
		}
	};

} // namespace Object

#endif