#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();
	int32_t fireTimer = pEnemy->GetFireTimer();

	//移動(ベクトルを加算)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	pEnemy->UpdateApproachPhase();

	//規定の位置に到達したら離脱
	if (position.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}
}