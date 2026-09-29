/* --------------
　プレイヤー処理
 ---------------- */
#include <DxLib.h>
#include "Player.h"
#include "Shot.h"
#include "Blast.h"
#include "../Scene/GameScene.h"
#include "../Application.h"
#include "../stage.h"
#include "../InputManager.h"
#include "../Utility.h"
#include <string>


/*　次回 08アニメーション再生　*/


Player::Player(void)
{
	for (int i = 0; i < static_cast<int>(ANIM_STATE::MAX); ++i)
	{
		for (int y = 0; y < static_cast<int>(ATK_STATE::MAX); ++y)
		{
			for (int x = 0; x < MAX_NUM_ANIM; ++x)
			{
				// 画像ハンドル
				images_[i][y][x] = -1;
			}
		}
	}

	isHit_ = new IsHitSide;
	left_ = right_ = new WidthPos;	// X軸
	top_ = bottom_ = new HeightPos; // Y軸

	isHit_->bottom = false;
	isHit_->top	  = false;
	isHit_->left	  = false;
	isHit_->right  = false;
}

Player::~Player(void)
{
}

void Player::Init(GameScene* gameScene)
{
	gameScene_ = gameScene;

	// マップチップ当たり判定用のプレイヤーを読み込む
	LoadImages();

	// 初期位置設定
	pos_.x = PLAYER_INIT_POS_X;
	pos_.y = PLAYER_INIT_POS_Y;

	movePosX_ = movePosY_ = 0;
	isJump_ = false;
	jumpCount_ = 0;

	dir_ = Utility::DIR::RIGHT; // 向きを変える

	// アニメーション初期化
	ChangeAnimState(ANIM_STATE::IDLE, true);
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update(void)
{
	Move(); // 移動処理

	// アニメーション更新処理
	UpdateAnim();

	// アニメーション遷移テスト
	UpdateChangeAnimTest();

	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_RETURN))
	{
		Shot* shot = new Shot;
		shot->Init(gameScene_);
		shot->SetShotPos(pos_, dir_);

		shot_.push_back(shot);
	}
	for (int i = 0; i < shot_.size(); i++)
	{
		shot_[i]->Update();
	}
}

void Player::Draw(void)
{
	int animState = static_cast<int>(animState_);
	int atkState  = static_cast<int>(atkState_);
	
	// プレイヤー描画
	if (images_[animState][atkState][animImgIdx_] != -1)
	{
		DrawRotaGraphF(pos_.x, pos_.y,
					   1.0, 0.0,
					   images_[animState][atkState][animImgIdx_],
					   true,
					   dir_ == Utility::DIR::LEFT);
	}
	for (int i = 0; i < shot_.size(); i++) 
	{
		shot_[i]->Draw();
	}


#if _DEBUG
	DrawFormatString(16, 0, 0xFF, "Player：Pos[%.3f, %.3f]", pos_.x, pos_.y);
	if (isHit_->bottom) DrawString(16, 16, "下側Hit", 0xFF);
	if (isHit_->top)	   DrawString(16, 32, "上側Hit", 0xFF);
	if (isHit_->left)   DrawString(16, 48, "左側Hit", 0xFF);
	if (isHit_->right)  DrawString(16, 64, "右側Hit", 0xFF);

	DrawFormatString(16, 80, 0xFF0000, "移動量: [%f, %f]", movePosX_, movePosY_);
	DrawFormatString(16, 96, 0xFF, "ジャンプフラグ：%d", isJump_);
#endif
}

/// <summary>
/// 解放処理
/// </summary>
void Player::Release(void)
{
	for (int i = 0; i < shot_.size(); i++)
	{
		shot_[i]->Release();
	}
	DeleteImages(); // 画像解放
}


/// <summary>
/// 移動処理
/// </summary>
void Player::Move(void)
{
	Vector2F prePos = pos_;

	Jump(); // ジャンプ処理

	if (movePosY_ > MAX_GRAVITY)
	{
		// 上下移動量 制限
		movePosY_ = MAX_GRAVITY;
	}

	/*　上当たり判定　*/
	top_ = ClacHeightPos(true);
	isHit_->top = IsHitPos(top_->center, top_->left, top_->right);
	if (prePos.y > pos_.y) CollisionTop();

	/*　下当たり判定　*/
	bottom_ = ClacHeightPos(false);
	isHit_->bottom = IsHitPos(bottom_->center, bottom_->left, bottom_->right);
	if (prePos.y < pos_.y) CollisionBottom();

	/* << 左移動 >> */
	if (CheckHitKey(KEY_INPUT_A))
	{
		dir_ = Utility::DIR::LEFT;

		movePosX_ -= MOVE_ACC_POWER;

		if (movePosX_ < -MAX_MOVE_SPEED)
		{
			movePosX_ = -MAX_MOVE_SPEED;
		}
		pos_.x += movePosX_;
		if (!isJump_ && movePosX_ != 0)
		{
			// アニメーション再生
			ChangeAnimState(ANIM_STATE::RUN, false);
		}
	}

	if (InputManager::GetInstance()->IsTrgUp(KEY_INPUT_A) && !isJump_)
	{
		// キーを離したときに待機アニメーション化
		ChangeAnimState(ANIM_STATE::IDLE, true);
	}
	
	if (movePosX_ < 0.0f)
	{
		movePosX_ += MOVE_DEC_POWER;
		// 移動量制御
		if (movePosX_ > 0.0f)
		{
			movePosX_ = 0.0f;
		}
		pos_.x += movePosX_; // 座標更新
	}
	left_ = CalcWidthPos(true);

	isHit_->left = IsHitPos(left_->center, left_->up, left_->down);
	if (prePos.x > pos_.x) CollisionLeft();

	// 左移動制限
	if (pos_.x < HALF_COL_SIZE_X) pos_.x = HALF_COL_SIZE_X;


	/* << 右移動 >> */
	if (CheckHitKey(KEY_INPUT_D))
	{
		dir_ = Utility::DIR::RIGHT;

		movePosX_ += MOVE_ACC_POWER;

		// 移動量制御
		if (movePosX_ > MAX_MOVE_SPEED)
		{
			movePosX_ = MAX_MOVE_SPEED;
		}
		pos_.x += movePosX_; // 座標更新

		if (!isJump_ && movePosX_ != 0)
		{
			// アニメーション再生
			ChangeAnimState(ANIM_STATE::RUN, false);
		}
	}
	if (InputManager::GetInstance()->IsTrgUp(KEY_INPUT_D) && !isJump_)
	{
		// キーを離したときに待機アニメーション化
		ChangeAnimState(ANIM_STATE::IDLE, true);
	}

	if (movePosX_ > 0.0f)
	{
		movePosX_ -= MOVE_DEC_POWER;
		// 移動量制御
		if (movePosX_ < 0.0f)
		{
			movePosX_ = 0.0f;
		}
		pos_.x += movePosX_; // 座標更新
	}
	right_ = CalcWidthPos(false);

	isHit_->right = IsHitPos(right_->center, right_->up, right_->up);
	if (prePos.x < pos_.x) CollisionRight();
	if (pos_.x > (Application::SCREEN_SIZE_X - (HALF_COL_SIZE_X * 2)))
	{
		pos_.x = (Application::SCREEN_SIZE_X - (HALF_COL_SIZE_X * 2));
	}
}

/// <summary>
/// ジャンプ処理
/// </summary>
void Player::Jump(void)
{
	// 移動処理[キー入力 + カウントが１以上]
	if (InputManager::GetInstance()->IsTrgDown(KEY_INPUT_SPACE) &&
		jumpCount_ > 0)
	{
		isJump_	  = true;
		movePosY_ = JUMP_POWER;
		jumpCount_--;

		ChangeAnimState(ANIM_STATE::JUMP, true);
	}

	bool isWallHit = (isHit_->left && CheckHitKey(KEY_INPUT_A) ||
					  isHit_->right && CheckHitKey(KEY_INPUT_D));

	/*　壁に衝突時 重力減速　*/
	if (movePosY_ > 0 && isWallHit)
	{
		pos_.y += movePosY_ / DEC_GRAVITY; // 上下移動量 加算
		movePosY_ += GRAVITY / DEC_GRAVITY;	  // 重力加算
	}
	else
	{
		pos_.y += movePosY_; // 上下移動量 加算
		movePosY_ += GRAVITY;	// 重力加算
	}
}


void Player::UpdateAnim(void)
{
	switch (animState_)
	{
	case ANIM_STATE::IDLE:
	case ANIM_STATE::JUMP:
		break;

	case ANIM_STATE::RUN:
		animCounter_ += ANIM_SPEED;
		animImgIdx_ = Utility::Round(animCounter_) % MAX_NUM_ANIM;
		break;

	case ANIM_STATE::DAMAGED:
		break;
	}
}

void Player::UpdateChangeAnimTest(void)
{
	InputManager* input = InputManager::GetInstance();
	if (input->IsNew(KEY_INPUT_1)) ChangeAnimState(ANIM_STATE::IDLE, true);
	if (input->IsNew(KEY_INPUT_2)) ChangeAnimState(ANIM_STATE::JUMP, true);
	if (input->IsNew(KEY_INPUT_3)) ChangeAnimState(ANIM_STATE::RUN, true);
	if (input->IsNew(KEY_INPUT_4)) ChangeAnimState(ANIM_STATE::DAMAGED, true);

	if (input->IsNew(KEY_INPUT_5)) atkState_ = ATK_STATE::NONE;
	if (input->IsNew(KEY_INPUT_6)) atkState_ = ATK_STATE::SHOT;
}

/// <summary>
/// アニメーション変更処理
/// </summary>
/// <param name="state">変更後の状態</param>
/// <param name="isResetAnimCount">アニメーションを初期化するか否か</param>
void Player::ChangeAnimState(ANIM_STATE state, bool isResetAnimCount)
{
	animState_	 = state; // アニメーション更新

	// リセット処理
	if (isResetAnimCount)
	{
		animCounter_ = 0.0f;	  // カウンタ初期化
		animImgIdx_  = 0;	  // 番号初期化
	}
}
#ifdef _DEBUG
void Player::DrawAnimState(void)
{
	constexpr unsigned int TEXT_COLOR = 0x000000;
	constexpr int DRAW_POS_X = 45;
	constexpr int DRAW_POS_Y = 80;

	switch (animState_)
	{
	case ANIM_STATE::IDLE:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "AnimParam：待機", TEXT_COLOR);
		break;

	case ANIM_STATE::JUMP:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "AnimParam：ジャンプ", TEXT_COLOR);
		break;

	case ANIM_STATE::RUN:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "AnimParam：走る", TEXT_COLOR);
		break;

	case ANIM_STATE::DAMAGED:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "AnimParam：ダメージ", TEXT_COLOR);
		break;

	}
}

void Player::DrawAtkState(void)
{
	constexpr unsigned int TEXT_COLOR = 0x000000;
	constexpr int DRAW_POS_X = 45;
	constexpr int DRAW_POS_Y = 100;
	switch (atkState_)
	{
	case ATK_STATE::NONE:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "atkParam：なし", TEXT_COLOR);
		break;
	case ATK_STATE::SHOT:
		DrawString(DRAW_POS_X, DRAW_POS_Y, "atkParam：攻撃", TEXT_COLOR);
		break;
	}
}
#endif


/// <summary>
/// 画像読み込み
/// </summary>
void Player::LoadImages(void)
{
	const std::string IMAGE_PATH = "Image/Player/P1/";

	/*　プレイヤー画像初期化　*/
	for (int animIdx = 0; animIdx < static_cast<int>(ANIM_STATE::MAX); ++animIdx)
	{
		for (int atkIdx = 0; atkIdx < static_cast<int>(ATK_STATE::MAX); ++atkIdx)
		{
			for (int animNumIdx = 0; animNumIdx < MAX_NUM_ANIM; ++animNumIdx)
			{
				images_[animIdx][atkIdx][animNumIdx] = -1;
			}
		}
	}
	int atkNoneIdx = static_cast<int>(ATK_STATE::NONE);
	int atkShotIdx = static_cast<int>(ATK_STATE::SHOT);

	int animIdx = static_cast<int>(ANIM_STATE::IDLE);

	images_[animIdx][atkNoneIdx][0] = LoadGraph((IMAGE_PATH + "Idle.png").c_str());
	images_[animIdx][atkShotIdx][0] = LoadGraph((IMAGE_PATH + "IdleShot.png").c_str());

	animIdx = static_cast<int>(ANIM_STATE::JUMP);
	images_[animIdx][atkNoneIdx][0] = LoadGraph((IMAGE_PATH + "Jump.png").c_str());
	images_[animIdx][atkShotIdx][0] = LoadGraph((IMAGE_PATH + "JumpShot.png").c_str());

	// 被ダメージ画像
	animIdx = static_cast<int>(ANIM_STATE::DAMAGED);
	images_[animIdx][atkNoneIdx][0] = LoadGraph((IMAGE_PATH + "Damaged.png").c_str());

	// 走る画像
	animIdx = static_cast<int>(ANIM_STATE::RUN);
	LoadDivGraph((IMAGE_PATH + "Run.png").c_str(),
				 MAX_NUM_ANIM, MAX_NUM_ANIM, 1,
				 SIZE_X, SIZE_Y, 
				 images_[animIdx][atkNoneIdx], false);

	// 走りなら撃つ画像
	LoadDivGraph((IMAGE_PATH + "RunShot.png").c_str(),
				 MAX_NUM_ANIM, MAX_NUM_ANIM, 1,
				 SIZE_X, SIZE_Y,
				 images_[animIdx][atkShotIdx], false);
}

/// <summary>
/// 画像削除
/// </summary>
void Player::DeleteImages(void)
{
	/*　プレイヤー画像初期化　*/
	for (int animIdx = 0; animIdx < static_cast<int>(ANIM_STATE::MAX); ++animIdx)
	{
		for (int atkIdx = 0; atkIdx < static_cast<int>(ATK_STATE::MAX); ++atkIdx)
		{
			for (int animNumIdx = 0; animNumIdx < MAX_NUM_ANIM; ++animNumIdx)
			{
				if (images_[animIdx][atkIdx][animNumIdx] != -1)
				{
					DeleteGraph(images_[animIdx][atkIdx][animNumIdx]);
				}
			}
		}
	}
}


/// <summary>
/// Y軸の衝突位置計算
/// </summary>
/// <param name="isTop">上位置を判定するか否か</param>
/// <returns> 縦衝突位置(構造体) </returns>
Player::HeightPos* Player::ClacHeightPos(bool isTop)
{
	HeightPos* height = new HeightPos;
	int offsetY = (HALF_COL_SIZE_Y + COL_OFFSET);
	if (isTop)
	{
		offsetY *= (-1); // 判定位置反転
	}

	height->center.x = static_cast<int>(pos_.x);
	height->center.y = static_cast<int>(pos_.y);
	height->center.y += offsetY;

	height->left = height->center;
	height->left.x -= (HALF_COL_SIZE_X - COL_OFFSET);

	height->right	= height->center;
	height->right.x += (HALF_COL_SIZE_X - COL_OFFSET);

	return height;
}

/// <summary>
/// X軸の衝突位置計算
/// </summary>
/// <param name="isLeft">左位置を判定するか否か</param>
/// <returns> 横衝突位置(構造体) </returns>
Player::WidthPos* Player::CalcWidthPos(bool isLeft)
{
	WidthPos* width = new WidthPos;
	int offsetX = (HALF_COL_SIZE_X + COL_OFFSET);

	if (isLeft)
	{
		offsetX *= (-1); // 判定位置反転
	}

	width->center.x = static_cast<int>(pos_.x);
	width->center.y = static_cast<int>(pos_.y);
	width->center.x += offsetX;

	width->up = width->center;
	width->up.y -= (HALF_COL_SIZE_Y - COL_OFFSET);

	width->down = width->center;
	width->down.y += (HALF_COL_SIZE_Y - COL_OFFSET);

	return width;
}


/// <summary>
/// 衝突判定
/// </summary>
/// <param name="middle"> 中央 当たり判定</param>
/// <param name="side1"> 横 当たり判定</param>
/// <param name="side2"> 横 当たり判定</param>
/// <returns>物に衝突したか否か</returns>
bool Player::IsHitPos(Vector2 middle, Vector2 side1, Vector2 side2)
{
	return gameScene_->IsCollisionStage(middle)
		|| gameScene_->IsCollisionStage(side1)
		|| gameScene_->IsCollisionStage(side2);
}

/// <summary>
/// 上側当たり判定処理
/// </summary>
void Player::CollisionTop(void)
{
	top_ = ClacHeightPos(true); // 上側３点の座標計算

	// 上側３点との当たり判定
	isHit_->top = IsHitPos(top_->center, top_->left, top_->right);

	// 座標補正処理	
	if (isHit_->top)
	{
		// 上側のマップ座標
		Vector2 mapPos = gameScene_->World2MapPos(top_->center);

		// 衝突したマップチップの上側を計算
		int mapChipUpY = mapPos.y * Stage::CHIP_SIZE_Y;

		// 衝突したマップチップの下側を計算
		int mapChipDownY = mapChipUpY + Stage::CHIP_SIZE_Y;

		// プレイヤーの上側を 衝突した位置の下側の位置に補正
		pos_.y = (static_cast<float>(mapChipDownY) + HALF_COL_SIZE_Y + COL_OFFSET);

		// 天井に衝突時上下移動量を0にする
		movePosY_ = 0.0f;
	}
}

/// <summary>
/// 下側当たり判定処理
/// </summary>
void Player::CollisionBottom(void)
{
	bottom_ = ClacHeightPos(false); // 下側３点の座標計算

	// 下側３点との当たり判定
	isHit_->bottom = IsHitPos(bottom_->center, bottom_->left, bottom_->right);

	// 座標補正処理	
	if (isHit_->bottom)
	{
		// 下側のマップ座標
		Vector2 mapPos = gameScene_->World2MapPos(bottom_->center);

		// 衝突したマップチップの上側を計算
		int mapChipUpY = mapPos.y * Stage::CHIP_SIZE_Y;

		// プレイヤーの下側を 衝突した位置の上側の位置に補正
		pos_.y = (static_cast<float>(mapChipUpY) - HALF_COL_SIZE_Y - COL_OFFSET);

		if (isJump_)
		{
			ChangeAnimState(ANIM_STATE::IDLE, true);
			isJump_ = false;
			jumpCount_ = MAX_JUMP;
		}
	}

	else if (!isJump_)
	{
		movePosY_ = 0.0f; // ジャンプ可能時
		isJump_ = true;
		ChangeAnimState(ANIM_STATE::JUMP, true);
	}
}

/// <summary>
/// 左側当たり判定
/// </summary>
/// <param name=""></param>
void Player::CollisionLeft(void)
{
	// 左側３点の座標計算
	left_ = CalcWidthPos(true);

	// 左側３点との当たり判定
	isHit_->left = IsHitPos(left_->center, left_->up, left_->down);

	// 座標補正処理	
	if (isHit_->left)
	{
		// 左側のマップ座標
		Vector2 mapPos = gameScene_->World2MapPos(left_->center);

		// 衝突したマップチップの左側を計算
		int mapChipLeftX = mapPos.x * Stage::CHIP_SIZE_X;

		// 衝突したマップチップの右側を計算
		int mapChipRightX = mapChipLeftX + Stage::CHIP_SIZE_X;

		// プレイヤーの左側を 衝突した位置の右側の位置に補正
		pos_.x = (static_cast<float>(mapChipRightX) + HALF_COL_SIZE_X + COL_OFFSET);

		// 右に移動量があるときに移動量を失くす
		if (movePosX_ > 0.0f) movePosX_ = 0.0f;
	}
}

/// <summary>
/// 右側当たり判定
/// </summary>
void Player::CollisionRight(void)
{
	// 右側３点の座標計算
	right_ = CalcWidthPos(false);

	// 右側３点との当たり判定
	isHit_->right = IsHitPos(right_->center, right_->up, right_->down);

	// 座標補正処理	
	if (isHit_->right)
	{
		// 右側のマップ座標
		Vector2 mapPos = gameScene_->World2MapPos(right_->center);

		// 衝突したマップチップの左側を計算
		int mapChipLeftX = mapPos.x * Stage::CHIP_SIZE_X;

		// プレイヤーの右側を 衝突した位置の左側の位置に補正
		pos_.x = (static_cast<float>(mapChipLeftX) - HALF_COL_SIZE_X - COL_OFFSET);

		// 右に移動量があるときに移動量を失くす
		if (movePosX_ > 0.0f) movePosX_ = 0.0f;
	}
}