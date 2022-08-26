#include "EnemyApproach.h"

void EnemyStateApproach::Update(Enemy* pEnemy) {
	Vector3 position = pEnemy->GetPosition();

	//ˆÚ“®(ƒxƒNƒgƒ‹‚ð‰ÁŽZ)
	pEnemy->Move(position, pEnemy->GetApproachSpeed());

	//‹K’è‚ÌˆÊ’u‚É“ž’B‚µ‚½‚ç—£’E
	if (position.z < 0.0f) {
		pEnemy->TimerClear();

		pEnemy->ChangeState(new EnemyStateLeave);
	}
}