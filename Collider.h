#pragma once
#include "Vector3.h"
#include <cstdint>
#include "CollisionConfig.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	//衝突時に呼ばれる関数
	virtual void OnCollision();

	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	//半径を取得
	virtual float GetRadius() = 0;
	//衝突属性(自分)を取得
	virtual uint32_t GetCollisionAttribute() = 0;
	//衝突マスク(相手)を取得
	virtual uint32_t GetCollisionMask() = 0;

	//半径を設定
	void SetRadius(float r) {this->radius_ = r; };
	//衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//衝突マスク(相手)を設定
	void SetCollisionMask(uint32_t collisionMask) { this->collisionMask_ = collisionMask; };

private:
	//衝突半径
	float radius_ = 0.5f;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
};