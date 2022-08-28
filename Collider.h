#pragma once
#include "Vector3.h"
#include <cstdint>
#include "CollisionConfig.h"

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider {
public:
	//�Փˎ��ɌĂ΂��֐�
	virtual void OnCollision();

	//���[���h���W���擾
	virtual Vector3 GetWorldPosition() = 0;
	//���a���擾
	virtual float GetRadius() = 0;
	//�Փˑ���(����)���擾
	virtual uint32_t GetCollisionAttribute() = 0;
	//�Փ˃}�X�N(����)���擾
	virtual uint32_t GetCollisionMask() = 0;

	//���a��ݒ�
	void SetRadius(float r) {this->radius_ = r; };
	//�Փˑ���(����)��ݒ�
	void SetCollisionAttribute(uint32_t collisionAttribute) { this->collisionAttribute_ = collisionAttribute; };
	//�Փ˃}�X�N(����)��ݒ�
	void SetCollisionMask(uint32_t collisionMask) { this->collisionMask_ = collisionMask; };

private:
	//�Փ˔��a
	float radius_ = 0.5f;

	//�Փˑ���(����)
	uint32_t collisionAttribute_ = 0xffffffff;
	//�Փ˃}�X�N(����)
	uint32_t collisionMask_ = 0xffffffff;
};