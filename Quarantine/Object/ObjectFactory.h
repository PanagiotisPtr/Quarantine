#ifndef OBJECT_OBJECTFACTORY_H
#define OBJECT_OBJECTFACTORY_H

#include <type_traits>

#include "Object.h"

namespace Object {

	template<class ObjectClass>
	class ObjectFactory {
	public:
		template<typename... Args>
		static std::enable_if_t<std::is_base_of<Object, ObjectClass>::value, ObjectClass>
			create(Args... args) {
			ObjectClass object = ObjectClass(args...);
			object.init();

			return object;
		}

		template<typename... Args>
		static std::enable_if_t<std::is_base_of<Object, ObjectClass>::value, ObjectClass>*
			createPointer(Args... args) {
			ObjectClass* object = new ObjectClass(args...);
			object->init();

			return object;
		}
	};

} // namespace object

#endif