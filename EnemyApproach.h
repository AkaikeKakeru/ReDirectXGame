#pragma once
#include "BaseEnemyState.h"
#include "Enemy.h"

/// <summary>
/// �ڋ�
/// </summary>
class EnemyStateApproach : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};