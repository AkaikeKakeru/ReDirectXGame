#pragma once

#include <functional>

/// <summary>
/// ��������
/// </summary>
class TimedCall {
public:
	//�R���X�g���N�^
	TimedCall(std::function<void(void)> f,uint32_t time);

	//�X�V
	void Update();

	//�����Ȃ�true��Ԃ�
	bool IsFinished() const { return isFinished_; };

private:
	//�R�[���o�b�N
	std::function<void(void)> f_;

	//�c�莞��
	uint32_t time_;

	//�����t���O
	bool isFinished_ = false;
};