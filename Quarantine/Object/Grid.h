#ifndef OBJECT_GRID_H
#define OBJECT_GRID_H

#include <cmath>
#include <functional>
#include <unordered_map>

#include "../Level/Level.h"
#include "../Global/Global.h"
#include "Composite.h"
#include "Plane.h"
#include "Tree.h"
#include "Sphere.h"
#include "Covid.h"
#include "ObjectFactory.h"

#include "glm/vec3.hpp"

namespace Object {

	template<typename TileObject>
	class Grid : public Composite {
	public:
		using PlaceBoxFunc = std::function<void(unsigned)>;
		using UnplaceBoxFunc = std::function<unsigned()>;
		using CallbackFunc = std::function<void(glm::vec3, PlaceBoxFunc, UnplaceBoxFunc, bool)>;
		using OnUpdateFunc = std::function<void(Grid& g)>;

		Grid(glm::vec3 p, size_t width, size_t height,
			CallbackFunc cb = [](glm::vec3, PlaceBoxFunc, UnplaceBoxFunc, bool) {}, OnUpdateFunc updateCb = [](Grid& g) {})
		: Composite(p), callback(cb), onUpdate(updateCb), level(new Level::Level(width, height, 0)), pandemicStarted(false){
			float pi = std::acos(-1.0f);
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					this->objects.emplace_back(ObjectFactory<TileObject>::createPointer(
						glm::vec3{ 0,0,0 })
					);
					this->objects.back()->scaleAndPlaceObject({ -0.95f,-0.95f,-0.95f });
					this->objects.back()->moveAndPlaceObject({ (float)i * 2.5f,0.0f, (float)j * 2.5f });
					this->objects.back()->rotateAndPlaceObject({ 0.0f,pi / 2,0.0f });
					this->tileMapIdx[this->objects.back()->getObjectId()]
						= { this->level->height - 1 - i, this->level->width - j - 1 };
				}
			}

			this->firstTileId = this->objects.front()->getObjectId();
			this->lastTileId = this->objects.back()->getObjectId();
			
			this->lock();
		}

		Grid(glm::vec3 p, Level::Level* l, CallbackFunc cb, OnUpdateFunc updateCb = [](Grid& g) {})
		: Composite(p), callback(cb), onUpdate(updateCb), level(l), pandemicStarted(false) {
			float pi = std::acos(-1.0f);
			for (size_t i = 0; i < this->level->height; i++) {
				for (size_t j = 0; j < this->level->width; j++) {
					this->objects.emplace_back(ObjectFactory<TileObject>::createPointer(
						glm::vec3{ 0,0,0 })
					);
					this->objects.back()->scaleAndPlaceObject({ -0.95f,-0.95f,-0.95f });
					this->objects.back()->moveAndPlaceObject({ (float)j * 2.5f,0.0f, (float)i * 2.5f });
					this->objects.back()->rotateAndPlaceObject({ 0.0f,pi / 2,0.0f });
					this->tileMapIdx[this->objects.back()->getObjectId()]
						= { this->level->height - 1 - i, this->level->width - j - 1 };
				}
			}

			this->firstTileId = this->objects.front()->getObjectId();
			this->lastTileId = this->objects.back()->getObjectId();

			for (size_t i = this->firstTileId; i < this->level->height * this->level->width; i++) {
				auto& tile = this->objects[i];
				auto p = this->tileMapIdx[tile->getObjectId()];
				unsigned mapI = p.first;
				unsigned mapJ = p.second;
				glm::vec3 pos = tile->getPos();
				pos.y += 0.04f;
				if (this->level->map[mapI][mapJ] == Level::Cell::TREE) {
					this->objects.emplace_back(
						ObjectFactory<Tree>::createPointer(pos)
					);
					this->objects.back()->scaleAndPlaceObject({ -0.96f, -0.96f, -0.96f });
				}
				else if (this->level->map[mapI][mapJ] == Level::Cell::TARGET) {
					this->objects.emplace_back(
						ObjectFactory<Sphere>::createPointer(pos, glm::vec3{ 0.0f, 0.0f, 1.0f })
					);
					this->objects.back()->scaleAndPlaceObject({ -0.96f, -0.96f, -0.96f });
				}
			}

			this->lock();
		}

		void select() override { }

		void placeBox(int i, int j, unsigned boxId) {
			level->map[i][j] = Level::Cell::BOX;
			if (boxes.count(i) == 0) {
				boxes[i] = {};
			}
			boxes[i][j] = boxId;
		}

		unsigned unplaceBox(int i, int j) {
			level->map[i][j] = Level::Cell::GRASS;
			if (boxes.count(i) == 0 || boxes[i].count(j) == 0) {
				return 0;
			}
			unsigned boxId = boxes[i][j];
			boxes[i].erase(j);

			return boxId;
		}

		bool hasBox(int i, int j) {
			return boxes.count(i) && boxes[i].count(j);
		}

		void removeLast() {
			if (this->objects.size()) {
				this->objects.pop_back();
			}
		}

		size_t getObjectCount() {
			return this->getObjects().size();
		}

		Level::Level* getLevel() { return this->level; }

	protected:
		Level::Level* level;
		std::unordered_map<unsigned, std::pair<int, int> > tileMapIdx;
		std::unordered_map<int, std::unordered_map<int, unsigned> > boxes;
		OnUpdateFunc onUpdate;
		unsigned firstTileId;
		unsigned lastTileId;

		void attachEventHandlers() override {
			Editable::attachEventHandlers();
			Global::EventBus.addEventHandler<Event::MouseButton>([this](const Event::Base& baseEvent) -> void {
				const Event::MouseButton& e = static_cast<const Event::MouseButton&>(baseEvent);

				if (e.button != GLFW_MOUSE_BUTTON_LEFT || e.action != GLFW_PRESS) {
					return;
				}

				int windowHeight, windowWidth;
				glfwGetWindowSize(Global::Window, &windowWidth, &windowHeight);
				float pixel[4];
				int flippedY = windowHeight - (int)Global::Cursor.y;
				glfwGetCursorPos(Global::Window, &Global::Cursor.x, &Global::Cursor.y);

				for (auto& o : this->objects) {
					o->deselect();
				}
				glDisable(GL_LIGHTING);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, windowWidth, windowHeight);
				for (const auto& o : this->objects) {
					o->drawWithId();
				}
				glReadPixels((int)Global::Cursor.x, flippedY, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
				for (int i = 0; i < 4; i++) pixel[i] = this->roundToTwoDecimals(pixel[i]);

				for (auto& o : this->objects) {
					if (o->getObjectId() == ColourIdGenerator::decodeId({ pixel[0], pixel[1], pixel[2] })) {
						if (this->tileMapIdx.find(o->getObjectId()) != std::end(this->tileMapIdx)) {
							std::pair<int,int> p = this->tileMapIdx[o->getObjectId()];
							if (this->level->map[p.first][p.second] == Level::Cell::GRASS) {
								this->callback(o->getPos(), [this, p](unsigned boxId) -> void {
									this->placeBox(p.first, p.second, boxId);
								}, [this, p]() -> unsigned {
									return this->unplaceBox(p.first, p.second);
								}, false);
							} else if (this->level->map[p.first][p.second] == Level::Cell::BOX) {
								this->callback(o->getPos(), [this, p](unsigned boxId) -> void {
									this->placeBox(p.first, p.second, boxId);
								}, [this, p]() -> unsigned {
									return this->unplaceBox(p.first, p.second);
								}, true);
							}
						}
					}
				}

				glEnable(GL_LIGHTING);
			}, this->getObjectId());
		}
	private:
		CallbackFunc callback;
		bool pandemicStarted;

		float roundToTwoDecimals(float v) {
			v += 0.005f;

			return std::trunc(100 * v) / 100;
		}
	};

} // namespace Object

#endif