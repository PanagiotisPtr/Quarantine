#ifndef SCREEN_SCREENFACTORY_H
#define SCREEN_SCREENFACTORY_H

#include <type_traits>

#include "Screen.h"

namespace Screen {

	template<class ScreenClass>
	class ScreenFactory {
	public:
		template<typename... Args>
		static std::enable_if_t<std::is_base_of<Screen, ScreenClass>::value, ScreenClass>
		create(Args... args) {
			ScreenClass screen = ScreenClass(args...);
			screen.init();

			return screen;
		}

		template<typename... Args>
		static std::enable_if_t<std::is_base_of<Screen, ScreenClass>::value, ScreenClass>*
		createPointer(Args... args) {
			ScreenClass* screen = new ScreenClass(args...);
			screen->init();

			return screen;
		}
	};

} // namespace screen

#endif