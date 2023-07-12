#ifndef MYSTER_H
#define MYSTER_H

#include "levelobject.h"

class Coin;
class Item;

class MysterBlock : public LevelObject
{
public:
	enum class ItemType {
		COIN, MUSHROOM
	};

	MysterBlock(World* pWorld);

	void update(float dt) override;
	void render(SDL_Renderer* pRenderer) override;

	void set_item(ItemType item) {
		m_item_type = item;
	}

	void on_hit();

private:
	AnimComponent* m_anim;
	SDL_Rect m_rect;
	Item *m_item;

	Vector2f m_init_pos;

	bool m_hit;
	ItemType m_item_type;
	bool m_item_spawned;
	bool m_finished;
	float m_grav;
};

#endif