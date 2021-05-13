#ifndef COLOUR_ID_GENERATOR_H
#define COLOUR_ID_GENERATOR_H

class ColourIdGenerator {
public:
	constexpr static float stepSize = 0.01f;
	static unsigned prevId;

	struct ColourId {
		float r, g, b;
	};

	static ColourId getColourId();

	static unsigned decodeId(const ColourId& ci);

	static void useStaticId(unsigned id);

	static void useDynamicId();

	static void resetIds();

	static void setCounter(unsigned id);
private:
	static bool fixed;
	static unsigned fixedId;
};

#endif