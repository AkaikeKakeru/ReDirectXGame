#pragma once

/// <summary>
/// �Փ˔���I�u�W�F�N�g
/// </summary>
class Collider {
public:
	//�Փˎ��ɌĂ΂��֐�
	virtual void OnCollisuion();

	//���a���擾
	float GetRadius();
	//���a��ݒ�
	void SetRadius(float r) {this->radius_ = r; };
private:
	//�Փ˔��a
	float radius_ = 0.5f;
};