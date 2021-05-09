#ifndef APPLICATION_H
#define APPLICATION_H

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif

#include <vector>
#include <functional>
#include <sstream>
#include <memory>
#include <queue>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#include "../Object/Object.h"
#include "../Object/Sphere.h"
#include "../Object/Plane.h"
#include "../Object/Cube.h"
#include "../Object/Cylinder.h"
#include "../Object/Light.h"
#include "../Object/Camera.h"

#include "../Event/EventBus.h"
#include "../Event/Event.h" 

#include "../Global/Global.h"

#include "../Colour/ColourIdGenerator.h"

#include "../Screen/Screen.h"
#include "../Screen/ScreenFactory.h"
#include "../Screen/Start.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

namespace Application {

	class Application {
	public:
		using ScreenPtr = std::unique_ptr<Screen::Screen>;
		using ScreenTransitionFunc = std::function<void(Screen::Screen*)>;

		Application() {
			if (!glfwInit()) {
				throw std::exception("GLFW initialisation failed.");
			}
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();

			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			this->window = glfwCreateWindow(mode->width, mode->height, "Scene Builder", NULL, NULL);
			if (!this->window) {
				glfwTerminate();
				throw std::exception("GLFW failed to create window.");
			}

			this->windowWidth = mode->width;
			this->windowHeight = mode->height;
			this->multiSelectOn = false;

			this->enqueueScreen(Screen::ScreenFactory<Screen::Start>::createPointer(this->getScreenTransitionFunc()));

			glfwMakeContextCurrent(this->window);
			glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

			this->linkEvents();

			Global::EventBus.addEventHandler<Event::WindowResize>([this](const Event::Base& baseEvent) -> void {
				const Event::WindowResize& e = static_cast<const Event::WindowResize&>(baseEvent);

				this->windowHeight = e.height;
				this->windowWidth = e.width;
			}, Global::ObjectId);

			Global::EventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
				const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

				if (e.button != GLFW_MOUSE_BUTTON_LEFT || e.action != GLFW_PRESS) {
					return;
				}

				float pixel[4];
				int flippedY = this->windowHeight - (int)Global::Cursor.y;
				glfwGetCursorPos(this->window, &Global::Cursor.x, &Global::Cursor.y);

				if (!this->multiSelectOn) {
					for (auto& o : this->getCurrentScreen()->getObjects()) {
						o->deselect();
					}
				}
				glDisable(GL_LIGHTING);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, this->windowWidth, this->windowHeight);
				// make sure that the first object is always the camera
				for (const auto& o : this->getCurrentScreen()->getObjects()) {
					o->drawWithId();
				}
				glReadPixels((int)Global::Cursor.x, flippedY, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
				for (int i = 0; i < 4; i++) pixel[i] = this->roundToTwoDecimals(pixel[i]);

				for (auto& o : this->getCurrentScreen()->getObjects()) {
					if (o->getObjectId() == ColourIdGenerator::decodeId({ pixel[0], pixel[1], pixel[2] })) {
						o->select();
					}
					else if (!this->multiSelectOn) {
						o->deselect();
					}
				}

				glEnable(GL_LIGHTING);
			}, Global::ObjectId);

			Global::EventBus.addEventHandler<Event::CursorPos>([this](const Event::Base& baseEvent) -> void {
				glfwGetCursorPos(this->window, &Global::Cursor.x, &Global::Cursor.y);
			}, Global::ObjectId);

			Global::EventBus.addEventHandler<Event::KeyPress>([this](const Event::Base& baseEvent) -> void {
				const Event::KeyPress& e = static_cast<const Event::KeyPress&>(baseEvent);

				if (e.key == GLFW_KEY_LEFT_CONTROL && e.action == GLFW_PRESS) {
					this->multiSelectOn = true;
				}
				else if (e.key == GLFW_KEY_LEFT_CONTROL && e.action == GLFW_RELEASE) {
					this->multiSelectOn = false;
				}
			}, Global::ObjectId);

			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (err != GLEW_OK)
			{
				std::stringstream ss;
				ss << "GLEW initialisation failed: "
					<< glewGetErrorString(err) << std::endl;

				throw std::exception(ss.str().c_str());
			}

			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, 0.4f);
			glPopMatrix();
			glfwGetCursorPos(this->window, &Global::Cursor.x, &Global::Cursor.y);
		}

		void start() {
			GLfloat global_ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

			while (!glfwWindowShouldClose(this->window))
			{
				if (this->screenQueue.size() > 1) {
					this->clearScreenQueue();
				}

				this->getCurrentScreen()->draw(this->windowWidth, this->windowHeight);

				glfwSwapBuffers(this->window);
				glfwPollEvents();
			}
			glfwTerminate();
		}

		ScreenTransitionFunc getScreenTransitionFunc() {
			return [this](Screen::Screen* screen) -> void {
				this->enqueueScreen(screen);
			};
		}

		void enqueueScreen(Screen::Screen* screen) {
			this->screenQueue.emplace(screen);
		}
	private:
		GLFWwindow* window;
		std::queue<ScreenPtr> screenQueue;
		int windowWidth;
		int windowHeight;
		bool multiSelectOn;

		float roundToTwoDecimals(float v) {
			v += 0.005f;

			return std::trunc(100 * v) / 100;
		}

		ScreenPtr& getCurrentScreen() {
			return this->screenQueue.front();
		}

		void clearScreenQueue() {
			while (this->screenQueue.size() > 1) {
				this->screenQueue.front()->clearScreen();
				this->screenQueue.pop();
			}
		}

		void linkEvents() {
			glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height) {
				Global::EventBus.pushEvent(Event::WindowResize(
					window,
					width,
					height
				));
			});

			glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
				Global::EventBus.pushEvent(Event::KeyPress(
					window,
					key,
					scancode,
					action,
					mods
				));
			});

			glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos) {
				Global::EventBus.pushEvent(Event::CursorPos(
					window,
					xpos,
					ypos
				));
			});

			glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
				Global::EventBus.pushEvent(Event::MouseButton(
					window,
					button,
					action,
					mods
				));
			});

			glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xoffset, double yoffset) {
				Global::EventBus.pushEvent(Event::MouseScroll(
					window,
					xoffset,
					yoffset
				));
			});
		}
	};

}

#endif