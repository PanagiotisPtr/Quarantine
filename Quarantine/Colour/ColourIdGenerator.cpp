#include "ColourIdGenerator.h"

#include <iostream>
#include <exception>

unsigned ColourIdGenerator::prevId = 0;
unsigned ColourIdGenerator::fixedId = 0;
bool ColourIdGenerator::fixed = false;


ColourIdGenerator::ColourId ColourIdGenerator::getColourId() {
	ColourIdGenerator::ColourId rv;
	rv.r = 0.0f;
	rv.g = 0.0f;
	rv.b = 0.0f;

	float sum = 0.0f;
	if (ColourIdGenerator::fixed) {
		sum = (float)ColourIdGenerator::fixedId * ColourIdGenerator::stepSize;
	}
	else {
		ColourIdGenerator::prevId++;
		sum = (float)ColourIdGenerator::prevId * ColourIdGenerator::stepSize;
	}
	rv.r = sum;
	rv.r = rv.r > 1.0f ? 1.0f : rv.r;

	rv.g = sum - 1.0f > 0.0f ? sum - 1.0f : 0.0f;
	rv.g = rv.g > 1.0f ? 1.0f : rv.g;

	rv.b = sum - 2.0f > 0.0f ? sum - 2.0f : 0.0f;
	rv.b = rv.b > 1.0f ? 1.0f : rv.b;

	if (sum > 3.0f) {
		throw std::exception("Ran out of IDs");
	}

	return rv;
}

unsigned ColourIdGenerator::decodeId(const ColourId& ci) {
	float sum = ci.r + ci.g + ci.b;
	sum *= (1 / stepSize) + 0.5;

	return (unsigned)sum;
}

void ColourIdGenerator::useDynamicId() {
	ColourIdGenerator::fixed = false;
}

void ColourIdGenerator::useStaticId(unsigned id) {
	ColourIdGenerator::fixed = true;
	ColourIdGenerator::fixedId = id;
}

void ColourIdGenerator::resetIds() {
	ColourIdGenerator::prevId = 0;
}

void ColourIdGenerator::setCounter(unsigned id) {
	ColourIdGenerator::prevId = id;
}