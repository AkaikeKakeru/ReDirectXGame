#include "CollisionManager.h"

// �Փ˔���Ɖ���
void CollisionManager::CheckAllCollision() {
	//���X�g���̃y�A�𑍓�����
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA){
		Collider* colA = *itrA;

		//�C�e���[�^B�̓C�e���[�^A�̎��̗v�f�����(�d����������)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB){
			Collider* colB = *itrB;

			//�y�A�̏Փ˔���
			CheckCollisionPair(colA,colB);
		}
	}
};

// �R���C�_�[2�̏Փ˔���Ɖ���
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//�Փ˃t�B���^�����O
	if (
		(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0
		){
		return;
	}

	//�R���C�_�[�̃��[���h���W���擾
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();

	float rA = colliderA->GetRadius();
	float rB = colliderB->GetRadius();

	if (myVector3_.CollisionAlgorithm(posA, rA, posB, rB) == true) {

		//���e�̏Փˎ��R�[���o�b�N���Ăяo��
		colliderA->OnCollision();
		//�G�e�̏Փˎ��R�[���o�b�N���Ăяo��
		colliderB->OnCollision();
	}
};

/// �R���C�_�[���X�g��S�폜
void CollisionManager::ClearColliderList() {
	colliders_.clear();
};