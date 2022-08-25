#pragma once

#include <functional>

/// <summary>
/// ��������
/// </summary>
class TimedCall {
public:
	//�R���X�g���N�^
	TimedCall(std::function<int(void)> f,uint32_t time);

	//�X�V
	void Update();

	//�����Ȃ�true��Ԃ�
	bool IsFinished() { return true; };

private:
	//�R�[���o�b�N
	std::function<int(void)> f_;

	//�c�莞��
	uint32_t time_;

	//�����t���O
	bool isFinished_ = false;
};