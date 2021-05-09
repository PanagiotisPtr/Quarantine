#ifndef LEVEL_LEVEL_H
#define LEVEL_LEVEL_H

#include <fstream>
#include <string>
#include <vector>

namespace Level {
	enum class Cell {
		GRASS,
		BOX,
		TREE,
		TARGET
	};

	struct Level {
		std::vector<std::vector<Cell> > map;
		size_t height;
		size_t width;
		size_t boxCount;

		Level(size_t w, size_t h, size_t bc)
		: height(h), width(w), boxCount(bc),
		map(std::vector<std::vector<Cell> >(h, std::vector<Cell>(w, Cell::GRASS))) {}

		Level(std::string filename) {
			std::ifstream fin(filename);
			size_t n, m, b; fin >> n >> m >> b;
			this->height = n;
			this->width = m;
			this->boxCount = b;
			std::string row;
			this->map = std::vector<std::vector<Cell> >(n, std::vector<Cell>(m, Cell::GRASS));

			for (int i = 0; i < n; i++) {
				fin >> row;
				for (int j = 0; j < row.size(); j++) {
					switch (row[j]) {
						case 'p':
							this->map[i][j] = Cell::TARGET;
							break;
						case 't':
							this->map[i][j] = Cell::TREE;
							break;
					}
				}
			}

			fin.close();
		}
	};

} // namespace Utility

#endif