#include "../Stage.h"
#include "GameScene.h"
#include "../Player/Player.h"
#include "../Player/Blast.h"
#include "../InputManager.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_ = new Stage();
	stage_->Init(this);

	player_ = new Player();
	player_->Init(this);
}

void GameScene::Update(void)
{
	stage_->Update();
	player_->Update();
}

void GameScene::Draw(void)
{
	stage_->Draw();
	player_->Draw();
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;

	player_->Release();
	delete player_;
}

Vector2 GameScene::World2MapPos(Vector2 worldPos)
{
	Vector2 ret;

	int mapX = worldPos.x / Stage::CHIP_SIZE_X;
	int mapY = worldPos.y / Stage::CHIP_SIZE_Y;

	ret.x = mapX;
	ret.y = mapY;

	return ret;
}

bool GameScene::IsCollisionStage(Vector2 worldPos)
{
	// ワールド座標からマップ座標へ変換する
	Vector2 mapPos = World2MapPos(worldPos);

	// プレイヤーがいる位置のマップチップ番号を取得する
	int chipNo = stage_->GetChipNo(mapPos);

	// 障害物のチップ番号と当たっていたら真を返す
	if (chipNo == 8
		|| chipNo == 9
		|| chipNo == 10
		|| chipNo == 11
		|| chipNo == 12)
	{
		return true;
	}

	return false;
}

Vector2 GameScene::GetCameraPos(void)
{
	return cameraPos_;
}
