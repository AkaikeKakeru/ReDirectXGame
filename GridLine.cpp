#include "GridLine.h"

void GridLine::Intialize() 
{
	//gridCoodinate��������
	SetCoordinate();

	//gridList��������
	SetList();

	//lineColor��������
	SetColor();
}

void GridLine::UpDate()
{

}

void GridLine::Draw()
{
	for (int i = 0; i < 22; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			gridCoordinate[gridList[i][0]],
			gridCoordinate[gridList[i][1]],
		lineColor[i]
		);
	}
}

//gridCoodinate��������
void GridLine::SetCoordinate()
{
	for (int i = 0; i < 44; i++)
	{
		//5�`-5��������
		static int j = 5;

		//i��22������
		if (i < 22)
		{
			//11������
			if (i < 11)
			{
				//z���W��-5��
				gridCoordinate[i].z = -5.0f;
			}
			else
			{
				//z���W��5��
				gridCoordinate[i].z = 5.0f;
			}
			//x���W��5�`-5��
			gridCoordinate[i].x = static_cast<float>(j);
		}
		//��Ə����͂قړ���
		else
		{
			if (i < 33)
			{
				gridCoordinate[i].x = -5.0f;
			}
			else
			{
				gridCoordinate[i].x = 5.0f;
			}
			gridCoordinate[i].z = static_cast<float>(j);
		}
		//y���W�͈ꗥ0��
		gridCoordinate[i].y = 0.0f;

		//j��-5�����������
		if (j <= -5)
		{
			//5�ɖ߂�
			j = 5;
		}
		else
		{
			//j��1���炷
			j--;
		}
	}
}

//gridList��������
void GridLine::SetList()
{

	for (int i = 0; i < 22; i++)
	{
		//i��11������
		if (i < 11)
		{
			//�n�_��grid��i�Ԗڂ̒��_
			gridList[i][0] = i;
		}
		else
		{
			//�n�_��grid��(i + 11)�Ԗڂ̒��_
			gridList[i][0] = i + 11;
		}
		//�I�_��grid��(�n�_ + 11)�Ԗڂ̒��_
		gridList[i][1] = gridList[i][0] + 11;
	}
}

//lineColor��������
void GridLine::SetColor()
{
	xColor = 
	{
		0xFF, 0x00, 0x00, 0xFF
	};

	zColor = 
	{
		0x00, 0x00, 0xFF, 0xFF
	};

	for (int i = 0; i < 22; i++)
	{
		if (i < 11)
		{
			lineColor[i] = zColor;

		}
		else
		{
			lineColor[i] = xColor;

		}
	};

}