#ifndef UTILITY_STRINGFORMATTER_H
#define UTILITY_STRINGFORMATTER_H

#include "GL/glew.h"
#include <stdio.h>
#include <cstdarg>
#include <exception>
#include <string>

namespace Utility {
	const size_t MAX_MESSAGE_SIZE = 512;

	class StringFormatter {
	public:
		static std::string formatString(const char* format, ...) {
			std::string rv;
			char carry[MAX_MESSAGE_SIZE];

			std::va_list args;
			va_start(args, format);
			vsnprintf(carry, sizeof(carry), format, args);
			va_end(args);

			rv = std::string(carry);

			return rv;
		}
	};

} // namespace Utility

#endif