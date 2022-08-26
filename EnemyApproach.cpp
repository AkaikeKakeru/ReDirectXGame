#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//�ړ�(�x�N�g�������Z)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	//�K��̈ʒu�ɓ��B�����痣�E
	if (position.z < 0.0f) {
		pEnemy->TimerClear();

		pEnemy->ChangeState(new EnemyStateLeave);
	}
}