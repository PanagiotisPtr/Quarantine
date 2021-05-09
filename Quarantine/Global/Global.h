#ifndef GLOBAL_H
#define GLOBAL_H

#include <unordered_map>
#include <functional>
#include <vector>
#include <queue>
#include <memory>

#include "../Object/Object.h"
#include "../Event/EventBus.h"
#include "glm/mat4x4.hpp"

namespace Global {

extern Event::EventBus EventBus;
extern glm::dvec2 Cursor;
extern GLFWwindow* Window;
extern unsigned ObjectId;

} // namespace Global

#endif