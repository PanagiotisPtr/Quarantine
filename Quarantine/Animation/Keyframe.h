#ifndef ANIMATION_KEYFRAME_H
#define ANIMATION_KEYFRAME_H

#include <queue>

#include "glm/vec3.hpp"

namespace Animation {

	struct Keyframe {
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 scale;
	};

	Keyframe operator+(const Keyframe& k1, const Keyframe& k2) {
		return Keyframe{
			k1.pos + k2.pos,
			k1.rot + k2.rot,
			k1.scale + k2.scale
		};
	}

	Keyframe operator-(const Keyframe& k1, const Keyframe& k2) {
		return Keyframe{
			k1.pos - k2.pos,
			k1.rot - k2.rot,
			k1.scale - k2.scale
		};
	}

	template<typename T>
	Keyframe operator/(const Keyframe& k, const T& v) {
		return Keyframe{
			k.pos / v,
			k.rot / v,
			k.scale / v
		};
	}

	using Sequence = std::queue<Keyframe>;

	Sequence animate(Keyframe start, Keyframe end, size_t steps = 10) {
		Keyframe step = (end - start) / (float)steps;
		Sequence sequence;

		sequence.push(start);
		for (int i = 0; i < 10; i++) {
			sequence.push(start + step);
		}

		return sequence;
	}

} // namespace animation

#endif