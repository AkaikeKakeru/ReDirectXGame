#pragma once

class EnemyBullet{
public:
	EnemyBullet();
	~EnemyBullet();

	///<summary>
	///������
	///<summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Intialize(Model* mode, const Vector3& position);

	///<summary>
	///�X�V
	///<summary>
	void Update();

	///<summary>
	///�`��
	///<summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(const ViewProjection& viewProjection);

private:
};