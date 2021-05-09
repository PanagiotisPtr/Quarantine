#ifndef SCREEN_SCREENIDGENERATOR_H
#define SCREEN_SCREENIDGENERATOR_H

namespace Screen {

	class ScreenIdGenerator {
	public:
		static unsigned prevId;

		static unsigned getScreenId() { return 8000 + (ScreenIdGenerator::prevId++); }
	private:
	};

} // namespace screen

#endif