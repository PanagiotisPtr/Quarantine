#ifndef OBJECT_BUTTON_H
#define OBJECT_BUTTON_H

#include <functional>

#include "Image.h"

namespace Object {

	class Button : public Image {
	public:
		using CallbackFunc = std::function<void()>;
		Button(glm::vec3 p, const char* textureLocation, CallbackFunc cb) : Image(p, textureLocation), callback(cb) {}

		void select() override {
			Object::select();
			this->callback();
		}
	private:
		CallbackFunc callback;
	};

} // namespace Object

#endif