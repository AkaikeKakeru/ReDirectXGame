#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();
	int32_t fireTimer = pEnemy->GetFireTimer();

	//ˆÚ“®(ƒxƒNƒgƒ‹‚ð‰ÁŽZ)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	pEnemy->UpdateApproachPhase();

	//‹K’è‚ÌˆÊ’u‚É“ž’B‚µ‚½‚ç—£’E
	if (position.z < 0.0f) {
		pEnemy->ChangeState(new EnemyStateLeave);
	}
}