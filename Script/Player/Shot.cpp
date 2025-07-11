/* --------
　発射処理
 ---------- */
#include "GameScene.h"
#include "Blast.h"
#include "Shot.h"
#include <DxLib.h>

Shot::Shot(void)
{
	blast_ = new Blast;
	image_ = -1;

	isActive_ = true;
	isBlast_  = false;

	veloX_ = START_SPEED;
}

Shot::~Shot(void)
{
}

void Shot::Init(GameScene* scene)
{
	gameScene_ = scene;

	blast_->Init();

	image_ = LoadGraph(IMAGE_HANDLE);
	if (image_ == -1)
	{
		OutputDebugString("\n弾丸画像がありません\n");
	}

	isActive_ = true;
	isBlast_ = false;

	veloX_ = START_SPEED;
}

void Shot::Update(void)
{
	//発射処理
	if (isActive_)
	{
		if (veloX_ < MOVE_ACC_MAX)
		{
			veloX_ += MOVE_ACC_POWER;
		}

		if (dir_ == Utility::DIR::LEFT)
		{
			pos_.x -= veloX_;
		}
		if (dir_ == Utility::DIR::RIGHT)
		{
			pos_.x += veloX_;
		}
		if (GetIsHit())
		{
			isActive_ = false;
			isBlast_ = true;
			blast_->CreateEffect(pos_);
		}
	}
	if (isBlast_)
	{
		blast_->Update();
	}
}

void Shot::Draw(void)
{
	int posX = static_cast<int>(pos_.x) + (SIZE_X / 2);
	int posY = static_cast<int>(pos_.y) + (SIZE_Y / 2);
	if (isActive_)
	{
		DrawRotaGraph(posX, posY,
			1.0, 0.0,
			image_, true,
			dir_ == Utility::DIR::LEFT);
	}
	if (isBlast_)
	{
		blast_->Draw();
	}
}

/// <summary>
/// 解放処理
/// </summary>
void Shot::Release(void)
{
	// 画像解放
	DeleteGraph(image_);

	delete blast_; // 爆発解放
}


/* --- <<< セッター処理 >>> --- */

/// <summary>
/// 発射位置割り当て(整数型)
/// </summary>
/// <param name="pos">整数ベクトル</param>
/// <param name="dir">対象の向き</param>
void Shot::SetShotPos(Vector2 pos, Utility::DIR dir)
{
	Vector2F posF;
	posF.x = static_cast<float>(pos.x);
	posF.y = static_cast<float>(pos.y);

	pos_ = posF; // 位置割り当て
	dir_ = dir;	 // 向き割り当て
}

/// <summary>
/// 発射位置割り当て(実数型)
/// </summary>
/// <param name="pos">実数ベクトル</param>
/// <param name="dir">対象の向き</param>
void Shot::SetShotPos(Vector2F pos, Utility::DIR dir)
{
	pos_ = pos; // 位置割り当て
	dir_ = dir; // 向き割り当て
}

/// <summary>
/// 衝突割り当て
/// </summary>
void Shot::SetIsActive(bool flag) 
{
	isActive_ = flag;
}

/// <summary>
/// 爆発クラス 取得
/// </summary>
/// <returns>爆破クラス</returns>
Blast* Shot::GetEffect(void)
{
	return blast_;
}

bool Shot::GetIsActive(void)
{
	return isActive_;
}

/// <summary>
/// 衝突判定
/// </summary>
/// <returns>衝突したか否か</returns>
bool Shot::GetIsHit(void)
{
	Vector2 pos;
	pos.x = static_cast<int>(pos_.x);
	pos.y = static_cast<int>(pos_.y);

	return gameScene_->IsCollisionStage(pos);
}
