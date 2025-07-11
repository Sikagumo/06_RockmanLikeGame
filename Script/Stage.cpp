#include "DxLib.h"
#include "Stage.h"
#include "string"
#include "fstream"
#include "sstream"
#include "Vector2.h"
#include "GameScene.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(GameScene* scene)
{
	// ゲームシーンの機能を使えるようにする
	gameScene_ = scene;

	// 分割された画像を読み込み
	LoadDivGraph(
		"Image/Stage/Map.png",
		NUM_MAP_CHIPS,
		NUM_MAP_CHIPS_X,
		NUM_MAP_CHIPS_Y,
		CHIP_SIZE_X,
		CHIP_SIZE_Y,
		imgMapChips_
	);

	// 外部ファイルからマップデータを読み込む
	LoadGroundCsvData();
}

void Stage::Update()
{
}

void Stage::Draw()
{
	// マップの表示
	for (int y = 0; y < MAP_GROUND_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_GROUND_SIZE_X; x++)
		{
			// マップ座標のXとYからマップチップ番号を取得する
			int mapChipNo = groundMap_[y][x];

			// マップチップ番号から画像のハンドルIDを取得する
			int imgHandle = imgMapChips_[mapChipNo];

			// マップチップのワールド座標
			int mapChipWorldPosX = x * CHIP_SIZE_X;
			int mapChipWorldPosY = y * CHIP_SIZE_Y;

			// カメラ座標取得
			Vector2 cameraPos = gameScene_->GetCameraPos();

			// マップチップのスクリーン座標
			// 2Dでは「スクリーン座標 = ワールド座標 - カメラ座標」
			int mapChipScreenPosX = mapChipWorldPosX - cameraPos.x;
			int mapChipScreenPosY = mapChipWorldPosY - cameraPos.y;

			// スクリーン座標描画
			DrawGraph(mapChipScreenPosX, mapChipScreenPosY, imgHandle, true);
		}
	}
}

void Stage::Release()
{
	// 読み込んだ画像の解放
	for (int i = 0; i < NUM_MAP_CHIPS; i++)
	{
		DeleteGraph(imgMapChips_[i]);
	}
}

void Stage::LoadGroundCsvData(void)
{
	// 地上データの初期化
	for (int y = 0; y < MAP_GROUND_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_GROUND_SIZE_X; x++)
		{
			groundMap_[y][x] = -1;
		}
	}

	// ファイルの読み込み
	std::ifstream ifs = std::ifstream("Data/Map.csv");
	if (!ifs)
	{
		// エラーが発生
		return;
	}

	// ファイルを１行ずつ読み込む
	std::string line;
	std::string c;
	int chipNo = 0;
	int x = 0;
	int y = 0;
	while (getline(ifs, line))
	{
		// 1行情報 string を ifstream の仲間に変換
		std::istringstream stream(line);

		// 1文字ずつ読み込み(カンマ区切り)
		x = 0;
		while (getline(stream, c, ','))
		{
			// stringからintに変換
			chipNo = stoi(c);

			// 2次元配列にマップチップ番号を格納
			groundMap_[y][x] = chipNo;

			++x;
		}
		++y;
	}
}

int Stage::GetChipNo(Vector2 mapPos)
{
	// マップ範囲外であれば判定しない
	if (mapPos.x < 0 || MAP_GROUND_SIZE_X <= mapPos.x 
		|| mapPos.y < 0 || MAP_GROUND_SIZE_Y <= mapPos.y)	{
		return -1;
	}

	return groundMap_[mapPos.y][mapPos.x];
}
