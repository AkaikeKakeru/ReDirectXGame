#include "GridLine.h"

void GridLine::Intialize() 
{
	//gridCoodinateを初期化
	SetCoordinate();

	//gridListを初期化
	SetList();

	//lineColorを初期化
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

//gridCoodinateを初期化
void GridLine::SetCoordinate()
{
	for (int i = 0; i < 44; i++)
	{
		//5～-5を代入する
		static int j = 5;

		//iは22未満か
		if (i < 22)
		{
			//11未満か
			if (i < 11)
			{
				//z座標を-5に
				gridCoordinate[i].z = -5.0f;
			}
			else
			{
				//z座標を5に
				gridCoordinate[i].z = 5.0f;
			}
			//x座標は5～-5に
			gridCoordinate[i].x = static_cast<float>(j);
		}
		//上と処理はほぼ同じ
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
		//y座標は一律0に
		gridCoordinate[i].y = 0.0f;

		//jは-5を下回ったか
		if (j <= -5)
		{
			//5に戻す
			j = 5;
		}
		else
		{
			//jを1減らす
			j--;
		}
	}
}

//gridListを初期化
void GridLine::SetList()
{

	for (int i = 0; i < 22; i++)
	{
		//iは11未満か
		if (i < 11)
		{
			//始点はgridのi番目の頂点
			gridList[i][0] = i;
		}
		else
		{
			//始点はgridの(i + 11)番目の頂点
			gridList[i][0] = i + 11;
		}
		//終点はgridの(始点 + 11)番目の頂点
		gridList[i][1] = gridList[i][0] + 11;
	}
}

//lineColorを初期化
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