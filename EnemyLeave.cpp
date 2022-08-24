#include "EnemyLeave.h"

void EnemyStateLeave::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//ˆÚ“®(ƒxƒNƒgƒ‹‚ð‰ÁŽZ)
	pEnemy->Move(position, pEnemy->GetLeaveSpeed());
}