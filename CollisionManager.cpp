#include "CollisionManager.h"

// 衝突判定と応答
void CollisionManager::CheckAllCollision() {
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA){
		Collider* colA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB){
			Collider* colB = *itrB;

			//ペアの衝突判定
			CheckCollisionPair(colA,colB);
		}
	}
};

// コライダー2つの衝突判定と応答
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//衝突フィルタリング
	if (
		(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0
		){
		return;
	}

	//コライダーのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (myVector3_.CollisionAlgorithm(posA, rA, posB, rB) == true) {

		//自弾の衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
};

/// コライダーリストを全削除
void CollisionManager::ClearColliderList() {
	colliders_.clear();
};