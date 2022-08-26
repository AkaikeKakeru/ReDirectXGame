#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//移動(ベクトルを加算)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	//規定の位置に到達したら離脱
	if (position.z < 0.0f) {
		pEnemy->TimerClear();

		pEnemy->ChangeState(new EnemyStateLeave);
	}
}