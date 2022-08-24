#pragma once

class Enemy; //EnemyƒNƒ‰ƒX‘O•ûéŒ¾

/// <summary>
/// Šî’ê
/// </summary>
class BaseEnemyState {
protected:
	Enemy* enemy_ = nullptr;
public:
	virtual void Update(Enemy* pEnemy) = 0;
};