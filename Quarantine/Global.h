#ifndef GLOBAL_H
#define GLOBAL_H

#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>
#include <memory>

#include "Object.h"
#include "EventBus.h"
#include "glm/mat4x4.hpp"

namespace Global {

extern Event::EventBus EventBus;
extern glm::dvec2 Cursor;
extern unsigned ObjectId;

} // namespace Global

#endif