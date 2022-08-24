#pragma once
#include "BaseEnemyState.h"
#include "Enemy.h"

/// <summary>
/// —£’E
/// </summary>
class EnemyStateLeave : public BaseEnemyState {
public:
	void Update(Enemy* pEnemy);
};
