#pragma once
#include "MyVector3.h"
#include <memory>
#include "Collider.h"
#include <list>

class CollisionManager {
public:
	/// <summary>
	/// �Փ˔���Ɖ���
	/// </summary>
	void CheckAllCollision();

	/// �R���C�_�[���X�g��S�폜
	void ClearColliderList();

	/// <summary>
	/// �Z�b�^�[
	/// </summary>
	//�R���C�_�[�����X�g�ɓo�^
	void SetCollider (Collider* collider) { 
		colliders_.push_back(collider); };

private:
	/// <summary>
	/// �R���C�_�[2�̏Փ˔���Ɖ���
	/// </summary>
	/// <param name="colliderA">�R���C�_�[A</param>
	/// <param name="colliderB">�R���C�_�[B</param>
	void CheckCollisionPair(Collider* colliderA,Collider* colliderB);


	//�R���C�_�[���X�g
	std::list<Collider*> colliders_;

	//Vector3�ɂ�����Ȍv�Z�̊֐��܂Ƃ�
	MyVector3 myVector3_;
};