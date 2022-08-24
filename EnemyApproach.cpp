#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();
	int32_t fireTimer = pEnemy->GetFireTimer();

	//�ړ�(�x�N�g�������Z)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	pEnemy->UpdateApproachPhase();

	//�K��̈ʒu�ɓ��B�����痣�E
	if (position.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}
}