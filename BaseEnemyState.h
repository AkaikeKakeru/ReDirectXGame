#pragma once

class Enemy; //Enemy�N���X�O���錾

/// <summary>
/// ���
/// </summary>
class BaseEnemyState {
protected:
	Enemy* enemy_ = nullptr;
public:
	virtual void Update(Enemy* pEnemy) = 0;
};