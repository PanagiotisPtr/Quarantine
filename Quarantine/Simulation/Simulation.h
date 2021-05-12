#ifndef SIMULATION_SIMULATION_H
#define SIMULATION_SIMULATION_H

#include "../Screen/Screen.h"
#include "../Level/Level.h"
#include "../Object/ObjectFactory.h"
#include "../Object/Covid.h"

namespace Simulation {
	
	class Simulation {
	public:
		using EditableObjectPtr = std::unique_ptr<Object::Editable>;
		
		Simulation(std::vector<EditableObjectPtr>& objs, Level::Level l)
		: level(l), objects(objs) {}

		void startPandemic() {
			size_t midX = this->level.width / 2;
			size_t midY = this->level.height / 2;

			printf("Adding\n");
			this->objects.emplace_back(
				Object::ObjectFactory<Object::Covid>::createPointer(
					glm::vec3{ 0.0, 0.0, 0.0 }
				)
			);
			this->objects.back()->setScale({ 0.02,0.02,0.02 });
			glm::vec2 pos = this->mapIndexToPos(this->level.height + 1, midX);
			this->objects.back()->setPos({ pos.x,0.0333,pos.y });
		}
	private:
		std::vector<EditableObjectPtr>& objects;
		Level::Level level;

		glm::vec2 mapIndexToPos(int i, int j) {
			float y = (int)this->level.height - i;
			float x = (int)this->level.width - j;

			printf("%.4f\t%.4f\n", x, y);

			return { x * 0.125, y * 0.125 };
		}
	};

} // namespace Utility

#endif