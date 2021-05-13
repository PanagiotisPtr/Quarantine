#ifndef SIMULATION_SIMULATION_H
#define SIMULATION_SIMULATION_H

#include <set>
#include <unordered_map>
#include <limits>
#include <cmath>

#include "../Screen/Screen.h"
#include "../Level/Level.h"
#include "../Object/ObjectFactory.h"
#include "../Object/Covid.h"

#include "../Animation/Keyframe.h"

#include "../Global/Global.h"

namespace Simulation {
	
	class Simulation {
	public:
		using EditableObjectPtr = std::unique_ptr<Object::Editable>;
		using Path = std::vector<std::pair<int, int> >;
		using WinCallback = std::function<void()>;
		using LoseCallback = std::function<void()>;

		constexpr static unsigned ID = 8888;

		Simulation(std::vector<EditableObjectPtr>& objs, Level::Level l, WinCallback wc, LoseCallback lc)
		: level(l), objects(objs), winCallback(wc), loseCallback(lc), simulationResult(false) {}

		~Simulation() {
			Global::EventBus.detachHandlersForObject(Simulation::ID);
		}

		void loadLevel(Level::Level l) { this->level = l; }

		void startPandemic() {
			this->addCovid();
			this->calculatePaths();
			this->attachEventHandlers();
		}

		auto& getCovidIds() { return this->covidIds; }

		EditableObjectPtr& getObjectWithId(unsigned id) {
			for (auto& o : this->objects) {
				if (o->getObjectId() == id) {
					return o;
				}
			}

			return this->objects.back();
		}

		glm::vec2 mapIndexToPos(int i, int j) {
			float y = (int)this->level.height - i - 1;
			float x = (int)this->level.width - j - 1;

			return { x * 0.125, y * 0.125 };
		}

		Path getPath(std::pair<int, int> start, std::pair<int, int> end) {
			using Point = std::pair<int, int>;
			using Map = std::vector<Point>;

			auto h = [end](const Point& p) -> int {
				return abs(p.first - end.first) + abs(p.second - end.second);
			};

			auto comp = [h](const Point& p1, const Point& p2) -> bool {
				return h(p1) < h(p2);
			};

			std::set<Point, decltype(comp)> openSet(comp);
			openSet.insert(start);

			std::unordered_map<
				int,
				std::unordered_map<int, std::pair<int, int> >
			> cameFrom;

			std::unordered_map<int, std::unordered_map<int, int> > gScore;
			std::unordered_map<int, std::unordered_map<int, int> > fScore;

			gScore[start.first] = {};
			gScore[start.first][start.second] = 0;

			fScore[start.first] = {};
			fScore[start.first][start.second] = h(start);

			while (!openSet.empty()) {
				auto current = *std::begin(openSet);
				if (current.first == end.first && current.second == end.second) {
					return this->reconstructPath(cameFrom, current);
				}

				openSet.erase(openSet.begin());
				std::vector<std::pair<int, int> > moves = {
					{-1, 0},
					{1, 0},
					{0, -1},
					{0, 1}
				};
				for (auto move : moves) {
					auto neighbor = current;
					neighbor.first += move.first;
					neighbor.second += move.second;
					if (!isValidMove(neighbor.first, neighbor.second)) continue;

					if (!gScore.count(current.first)) {
						gScore[current.first] = {};
					}
					if (!gScore[current.first].count(current.second)) {
						gScore[current.first][current.second] = std::numeric_limits<int>::max();
					}
					int tentativeGScore = gScore[current.first][current.second] + 1;

					if (!gScore.count(neighbor.first)) {
						gScore[neighbor.first] = {};
					}
					if (!gScore[neighbor.first].count(neighbor.second)) {
						gScore[neighbor.first][neighbor.second] = std::numeric_limits<int>::max();
					}
					int neighbourGScore = gScore[neighbor.first][neighbor.second];
					if (tentativeGScore < neighbourGScore) {
						if (cameFrom.count(neighbor.first) == 0) {
							cameFrom[neighbor.first] = {};
						}
						cameFrom[neighbor.first][neighbor.second] = current;
						gScore[neighbor.first][neighbor.second] = tentativeGScore;
						if (openSet.find(neighbor) == std::end(openSet)) {
							openSet.insert(neighbor);
						}
					}
				}
			}

			return {};
		}

		void callResult() {
			if (this->simulationResult)
				this->loseCallback();
			else
				this->winCallback();
		}
	private:
		std::vector<EditableObjectPtr>& objects;
		std::vector<
			std::pair<unsigned, 
				std::pair<int, int>
			>
		> covidIds;
		std::unordered_map<unsigned, Path> paths;
		Level::Level level;
		WinCallback winCallback;
		LoseCallback loseCallback;
		bool simulationResult;

		std::vector<std::pair<int, int> > reconstructPath(
			std::unordered_map<
				int,
				std::unordered_map<int, std::pair<int, int> >
			>& cameFrom,
			std::pair<int, int> current
		) {
			std::vector<std::pair<int, int> > path;
			path.push_back(current);
			while (cameFrom.count(current.first)
				&& cameFrom[current.first].count(current.second)) {
				current = cameFrom[current.first][current.second];
				path.push_back(current);
			}

			return path;
		}

		bool isValidMove(int x, int y) {
			bool outOfBounds = (int)this->level.height + 1 < y || y < -1
				|| (int)this->level.width + 1 < x || x < -1;
			if (outOfBounds) {
				return false;
			}

			if (0 <= y && y < this->level.height
				&& 0 <= x && x < this->level.width) {
				return level.map[y][x] == Level::Cell::GRASS
					|| level.map[y][x] == Level::Cell::TARGET;
			}

			return true;
		}

		void attachEventHandlers() {
			Global::EventBus.addEventHandler<Event::Tick>([this](const Event::Base& baseEvent) -> void {
				bool noChanges = true;
				for (auto& i : this->getCovidIds()) {
					unsigned& id = i.first;
					int& y = i.second.first;
					int& x = i.second.second;

					if (this->paths.count(id) == 0) continue;
					Path& path = this->paths[id];
					if (path.size() == 0) continue;

					noChanges = false;
					
					x = path.back().first;
					y = path.back().second;
					path.pop_back();

					auto& o = this->getObjectWithId(id);
					glm::vec2 nextPos = this->mapIndexToPos(y, x);
					o->runSequence(
						Animation::animate(
							{ o->getPos(), o->getRot(), o->getScale() },
							{ {nextPos.x, o->getPos().y, nextPos.y}, o->getRot(), o->getScale() },
							10
						)
					);
				}

				if (noChanges) {
					this->callResult();
				}
			}, Simulation::ID);
		}

		std::pair<int, int> targetPosition() {
			for (int i = 0; i < this->level.height; i++) {
				for (int j = 0; j < this->level.width; j++) {
					if (level.map[i][j] == Level::Cell::TARGET) {
						return { i, j };
					}
				}
			}

			return { -1, -1 };
		}

		void calculatePaths() {
			bool isReachable = false;
			for (auto& i : this->getCovidIds()) {
				unsigned& id = i.first;
				int& y = i.second.first;
				int& x = i.second.second;
				auto target = this->targetPosition();

				Path path = this->getPath({ x, y }, { target.second, target.first });
				
				isReachable |= path.size();

				this->paths[id] = path;
			}

			this->simulationResult = isReachable;
		}

		void addCovid() {
			int midX = this->level.width / 2;
			int midY = this->level.height / 2;

			this->objects.emplace_back(
				Object::ObjectFactory<Object::Covid>::createPointer(
					glm::vec3{ 0.0, 0.0, 0.0 }
				)
			);
			this->objects.back()->setScale({ 0.02,0.02,0.02 });
			glm::vec2 pos = this->mapIndexToPos(this->level.height, midX);
			this->objects.back()->setPos({ pos.x,0.0333,pos.y });
			this->covidIds.push_back({
				this->objects.back()->getObjectId(),
				{
					this->level.height,
					midX
				}
			});

			this->objects.emplace_back(
				Object::ObjectFactory<Object::Covid>::createPointer(
					glm::vec3{ 0.0, 0.0, 0.0 }
				)
			);
			this->objects.back()->setScale({ 0.02,0.02,0.02 });
			pos = this->mapIndexToPos(-1, midX);
			this->objects.back()->setPos({ pos.x,0.0333,pos.y });
			this->covidIds.push_back({
				this->objects.back()->getObjectId(),
				{
					-1,
					midX
				}
			});

			this->objects.emplace_back(
				Object::ObjectFactory<Object::Covid>::createPointer(
					glm::vec3{ 0.0, 0.0, 0.0 }
				)
			);
			this->objects.back()->setScale({ 0.02,0.02,0.02 });
			pos = this->mapIndexToPos(midY, this->level.width);
			this->objects.back()->setPos({ pos.x,0.0333,pos.y });
			this->covidIds.push_back({
				this->objects.back()->getObjectId(),
				{
					midY,
					this->level.width
				}
			});

			this->objects.emplace_back(
				Object::ObjectFactory<Object::Covid>::createPointer(
					glm::vec3{ 0.0, 0.0, 0.0 }
				)
			);
			this->objects.back()->setScale({ 0.02,0.02,0.02 });
			pos = this->mapIndexToPos(midY, -1);
			this->objects.back()->setPos({ pos.x,0.0333,pos.y });
			this->covidIds.push_back({
				this->objects.back()->getObjectId(),
				{
					midY,
					-1
				}
			});
		}
	};

} // namespace Utility

#endif