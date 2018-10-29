#include "Enemy.h"
#include <random>
#include <ctime>
//#include <cstdlib>

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

box Enemy::hurtBox()
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
void Enemy::killTarget() {
	moveUp = moveDown = moveLeft = moveRight = atackTarguet = false;
	if (target != NULL) {
		if (canHit(target)) atackTarguet = true;
		else {
			srand(time(0));
			bool randomBool = rand() % 2;
			if (pos.y > target->pos.y) moveUp = true;
			else moveDown = true;
			if (pos.x > target->pos.x) moveLeft = true;
			else moveRight = true;
		}
	}
}
void setTarguet() {

}

bool Enemy::canHit(Enemy * target) {
	box b1, b2;
	b1 = hitBox();
	b2 = target->hurtBox();
	if (b1.mins.x > b2.mins.x) {
		if (b1.mins.x < b2.maxs.x) {
			if (b1.mins.y > b2.mins.y) {
				return true;
			}
			else if (b2.mins.y < b1.maxs.y) return true;

			if (b1.maxs.y < b2.maxs.y) {
				if (b1.maxs.y > b2.mins.y) return true;
			}
			else if (b2.maxs.y > b1.mins.y) return true;
		}
	}
	else if (b2.mins.x < b1.maxs.x) {
		if (b1.mins.y > b2.mins.y) {
			if (b1.mins.y < b2.maxs.y) return true;
		}
		else if (b2.mins.y < b1.maxs.y) return true;

		if (b1.maxs.y < b2.maxs.y) {
			if (b1.maxs.y > b2.mins.y) return true;
		}
		else if (b2.maxs.y > b1.mins.y) return true;
	}

	if (b1.maxs.x < b2.maxs.x) {
		if (b1.maxs.x > b2.mins.x) {
			if (b1.mins.y > b2.mins.y) {
				if (b1.mins.y < b2.maxs.y) return true;
			}
			else if (b2.mins.y < b1.maxs.y) return true;

			if (b1.maxs.y < b2.maxs.y) {
				if (b1.maxs.y > b2.mins.y) return true;
			}
			else if (b2.maxs.y > b1.mins.y) return true;
		}
	}
	else if (b2.maxs.x > b1.mins.x) {
		if (b1.mins.y > b2.mins.y) {
			if (b1.mins.y < b2.maxs.y) return true;
		}
		else if (b2.mins.y < b1.maxs.y) return true;

		if (b1.maxs.y < b2.maxs.y) {
			if (b1.maxs.y > b2.mins.y) return true;
		}
		else if (b2.maxs.y > b1.mins.y) return true;
	}
	return false;
}

box Enemy::hitBox() {
	box yOu_HAvE_tO_reTuRN_A_bOx;
	return yOu_HAvE_tO_reTuRN_A_bOx;
}