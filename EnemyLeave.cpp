#include "EnemyLeave.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//移動(ベクトルを加算)
	pEnemy->Move(position, pEnemy->GetLeaveSpeed());
}