#include "level.h"
#include "world.h"
#include <fstream>
#include <vector>
#include "tile.h"
#include "mysterblock.h"
#include "game.h"
#include "world.h"
#include "koopa.h"
#include "keyboard.h"
#include "background.h"

Level::Level(World* pWorld) : GameObject(pWorld)
{
	m_level_size = { 16,16 };
	load_level();
}

void Level::load_level()
{
	get_world()->reset();

	std::vector<LevelProperties> levels = read_file("assets\\levels\\1\\level1.json");

	for (const auto& level : levels) {

		for (int i = m_level_size.y - 1; i >= 0; i--) {

			for (int j = 0; j < m_level_size.x; j++) {

				int number = level.data[(i * m_level_size.y) + j];
				Vector2i pos{ j,i };
				Vector2i tileset_pos{ level.x,level.y };

				if (number == 283)
					add_basic_tile({ 0,102 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::ON_TOP);
				else if (number == 303)
					add_basic_tile({ 0,119 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 287)
					add_basic_tile({ 68,102 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::ON_TOP);
				else if (number == 307)
					add_basic_tile({ 68,119 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 284)
					add_basic_tile({ 17,102 }, j, i, tileset_pos);
				else if (number == 285)
					add_basic_tile({ 34,102 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::ON_TOP);
				else if (number == 286)
					add_basic_tile({ 51,102 }, j, i, tileset_pos);
				else if (number == 304)
					add_basic_tile({ 17,119 }, j, i, tileset_pos);
				else if (number == 305)
					add_basic_tile({ 34,119 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 306)
					add_basic_tile({ 51,119 }, j, i, tileset_pos);
				else if (number == 324)
					add_basic_tile({ 17,136 }, j, i, tileset_pos);
				else if (number == 326)
					add_basic_tile({ 51,136 }, j, i, tileset_pos);
				else if (number == 254)
					add_basic_tile({ 187,68 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 255)
					add_basic_tile({ 204,68 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 234)
					add_basic_tile({ 187,51 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 235)
					add_basic_tile({ 204,51 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 189)
					add_basic_tile({ 102,17 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 190)
					add_basic_tile({ 119,17 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 191)
					add_basic_tile({ 136,17 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 209)
					add_basic_tile({ 102,34 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 210)
					add_basic_tile({ 119,34 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 211)
					add_basic_tile({ 136,34 }, j, i, tileset_pos, LevelObject::COLLIDE_TYPE::NONE);
				else if (number == 162)
					add_koopa(j, i, tileset_pos);
				else if (number == 161)
					add_mario(j, i, tileset_pos);
				else if (number == 1)
					add_coin(j, i, tileset_pos);
				else if (number == 28)
					add_myster_block(j, i, tileset_pos);
			}
		}
	}

	m_backgrounds[0] = get_world()->add_gameobject_to_back<Background>();
	m_backgrounds[0]->set_rect({ 1024,0 });
	m_backgrounds[0]->set_pos(-m_backgrounds[0]->get_size().x, 0.f);

	m_backgrounds[1] = get_world()->add_gameobject_to_back<Background>();
	m_backgrounds[1]->set_rect({ 1024,0 });
	m_backgrounds[1]->set_pos(0.f, 0.f);

	m_backgrounds[2] = get_world()->add_gameobject_to_back<Background>();
	m_backgrounds[2]->set_rect({ 1024,0 });
	m_backgrounds[2]->set_pos(m_backgrounds[0]->get_size().x, 0.f);

	m_backgrounds[3] = get_world()->add_gameobject_to_back<Background>();
	m_backgrounds[3]->set_rect({ 1024,0 });
	m_backgrounds[3]->set_pos(m_backgrounds[0]->get_size().x * 2.f, 0.f);

	
}

void Level::destroy_current_level()
{
	for (auto go = get_world()->begin(); go != get_world()->end(); ++go) {
		if (go->get() != this)get_world()->delete_gameobject(go->get());
	}
}

void Level::update(float dt)
{
	handle_backgrounds(dt);
}

void Level::handle_backgrounds(float dt)
{
	for (auto& background : m_backgrounds) {
		if (background->get_pos().x + background->get_size().x < -background->get_size().x) {
			background->set_x(background->get_size().x * 2.f);
		}
	}
}

void Level::add_basic_tile(Vector2i rect, int j, int i, Vector2i tileset_pos, BasicTile::COLLIDE_TYPE collide)
{
	BasicTile* tile = get_world()->add_gameobject_to_back<BasicTile>();

	tile->set_collide_type(collide);
	tile->set_rect(rect);
	tile->set_pos(TILE_SIZE * j + tileset_pos.x * TILE_SIZE,
		Game::instance().get_window_size().y - 16 * tile->get_size().y + i * tile->get_size().y);
}

void Level::add_mario(int j, int i, Vector2i tileset_pos)
{
	Mario* mario = get_world()->add_gameobject<Mario>();

	mario->set_pos(mario->get_size().x * j + tileset_pos.x * mario->get_size().x,
		Game::instance().get_window_size().y - 16 * TILE_SIZE + i * TILE_SIZE);
}

void Level::add_coin(int j, int i, Vector2i tileset_pos)
{
	Coin* coin = get_world()->add_gameobject<Coin>();

	coin->set_pos(coin->get_size().x * j + tileset_pos.x * coin->get_size().x,
		Game::instance().get_window_size().y - 16 * coin->get_size().y + i * coin->get_size().y);
}

void Level::add_myster_block(int j, int i, Vector2i tileset_pos)
{
	MysterBlock* myster_block = get_world()->add_gameobject<MysterBlock>();

	myster_block->set_pos(myster_block->get_size().x * j + tileset_pos.x * myster_block->get_size().x,
		Game::instance().get_window_size().y - 16 * myster_block->get_size().y + i * myster_block->get_size().y);
}

void Level::add_koopa(int j, int i, Vector2i tileset_pos)
{
	Koopa* koopa = get_world()->add_gameobject<Koopa>();

	koopa->set_pos(koopa->get_size().x * j + tileset_pos.x * koopa->get_size().x,
		Game::instance().get_window_size().y - 16 * koopa->get_size().y + i * koopa->get_size().y -
		koopa->get_size().y / 2.f);
}

std::vector<LevelProperties> Level::read_file(const std::string& path)
{
	std::string base_path = SDL_GetBasePath();
	std::ifstream file(base_path + path);

	if (!file.is_open()) {
		BREAK();
	}

	nlohmann::json json;
	file >> json;

	file.close();

	std::vector<LevelProperties> levels;

	nlohmann::json layers = json["layers"];
	nlohmann::json firstLayer = layers[0];
	nlohmann::json chunks = firstLayer["chunks"];

	for (int i = 0; i < chunks.size(); i++) {

		nlohmann::json level_data = chunks[i];

		LevelProperties level{
			level_data["data"],
			level_data["x"],
			level_data["y"]
		};

		levels.push_back(level);
	}

	return levels;
}
