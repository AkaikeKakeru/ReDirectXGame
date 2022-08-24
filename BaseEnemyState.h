#pragma once

class Enemy; //Enemyクラス前方宣言

/// <summary>
/// 基底
/// </summary>
class BaseEnemyState {
protected:
	Enemy* enemy_ = nullptr;
public:
	virtual void Update(Enemy* pEnemy) = 0;
};