#ifndef LEVEL_H
#define LEVEL_H

#include <nlohmann/json.hpp>
#include <vector>
#include "gameobject.h"
#include "texturemanager.h"
#include "tile.h"
#include "world.h"

class World;
class Background;

struct LevelProperties {
	nlohmann::json data;
	int x, y;
};

class Level : public GameObject
{
public:
	Level(World* pWorld);

	void load_level();
	void destroy_current_level();

	void update(float dt);

private:

	void handle_backgrounds(float dt);

	void add_basic_tile(Vector2i rect, int j, int i, Vector2i tileset_pos, BasicTile::COLLIDE_TYPE collide = BasicTile::COLLIDE_TYPE::TOTAL);
	void add_mario(int j, int i, Vector2i tileset_pos);
	void add_coin(int j, int i, Vector2i tileset_pos);
	void add_myster_block(int j, int i, Vector2i tileset_pos);
	void add_koopa(int j, int i, Vector2i tileset_pos);

	std::vector<LevelProperties> read_file(const std::string& path);
	std::unordered_map<int, LevelObject*> m_tiles_code;

	Background* m_backgrounds[4];
	Vector2i m_level_size;
};

#endif