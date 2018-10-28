#include "Enemy.h"
//
//void Enemy::render()
//{
//	sprite->render();
//}
//
//void Enemy::setPosition()
//{
//	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
//}

void Enemy::render() { sprite->render(); }

box Enemy::calcHurtBox()
{
	box hurtBox;//depen de l'sprite potser haura de ser diferent per dreta o esquerra

	hurtBox.mins = glm::ivec2(pos.x + colisionOffset.x, pos.y + (size.y / 3.f));
	hurtBox.maxs = glm::ivec2(pos.x + size.x - colisionOffset.x, pos.y + size.y);

	return hurtBox;
}

void Enemy::setPosition()
{
	sprite->setPosition(glm::vec2(float(pos.x), float(pos.y)));
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}