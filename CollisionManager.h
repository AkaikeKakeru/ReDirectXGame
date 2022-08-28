#pragma once
#include "MyVector3.h"
#include <memory>
#include "Collider.h"
#include <list>

class CollisionManager {
public:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	/// コライダーリストを全削除
	void ClearColliderList();

	/// <summary>
	/// セッター
	/// </summary>
	//コライダーをリストに登録
	void SetCollider (Collider* collider) { 
		colliders_.push_back(collider); };

private:
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA,Collider* colliderB);


	//コライダーリスト
	std::list<Collider*> colliders_;

	//Vector3による特殊な計算の関数まとめ
	MyVector3 myVector3_;
};