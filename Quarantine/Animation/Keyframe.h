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

	using Sequence = std::queue<Keyframe>;

	inline Keyframe operator+(const Keyframe& k1, const Keyframe& k2) {
		return Keyframe{
			k1.pos + k2.pos,
			k1.rot + k2.rot,
			k1.scale + k2.scale
		};
	}

	inline Keyframe operator-(const Keyframe& k1, const Keyframe& k2) {
		return Keyframe{
			k1.pos - k2.pos,
			k1.rot - k2.rot,
			k1.scale - k2.scale
		};
	}

	inline Keyframe operator/(const Keyframe& k, const float& v) {
		return Keyframe{
			k.pos / v,
			k.rot / v,
			k.scale / v
		};
	}

	inline Keyframe operator*(const Keyframe& k, const float& v) {
		return Keyframe{
			k.pos * v,
			k.rot * v,
			k.scale * v
		};
	}

	inline Sequence animate(Keyframe start, Keyframe end, size_t steps = 10) {
		Keyframe step = (end - start) / (float)steps;
		Sequence sequence;

		for (int i = 0; i <= steps; i++) {
			sequence.push(start + step * (float)i);
		}

		return sequence;
	}

} // namespace animation

#endif