#include "EnemyLeave.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//�ړ�(�x�N�g�������Z)
	pEnemy->Move(position, pEnemy->GetLeaveSpeed());
}