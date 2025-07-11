#include "SceneManager.h"
#include "Blast.h"
#include "Vector2.h"
#include <string>
#include <DxLib.h>

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Blast::Blast(void)
{
	for (int i = 0; i < IMAGE_SIZE_MAX; i++) 
	{
		images_[i] = -1;
	}
}

/// <summary>
///  初期化処理
/// </summary>
void Blast::Init(void)
{
	int image = LoadDivGraph("image/Effect/EffectRed.png",
							 IMAGE_SIZE_MAX, IMAGE_SIZE_MAX, 1,
							 SIZE_X, SIZE_Y,
							 &(images_[0]), true);
	if (image == -1)
	{
		OutputDebugString("\n爆破画像が割り当ていません\n");
	}
	InitParticle();

	isActive_ = false;

}

/// <summary>
/// 更新処理
/// </summary>
void Blast::Update(void)
{
	if (isActive_)
	{
		isActive_ = false;

		size_t size = particles_.size();

		for (int i = 0; i < size; i++)
		{
			// 生存していなければ、処理をスキップ
			if (particles_[i].life <= 0.0f) continue;

			// 生存時間を減らす
			particles_[i].life -= DEC_LIFE;

			// 透明度を減らす
			particles_[i].blendRate -= DEC_ALPHA;

			// 座標更新処理
			particles_[i].pos.x += particles_[i].dirVec.x * particles_[i].speed;
			particles_[i].pos.y += particles_[i].dirVec.y * particles_[i].speed;

			// エフェクト有効
			isActive_ = true;
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Blast::Draw(void)
{
	if (isActive_)
	{
		size_t size = particles_.size();
		for (int i = 0; i < size; i++)
		{
			if (particles_[i].life <= 0.0f) continue;

			// 半透明モードに変更する
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(particles_[i].blendRate));

			// 描画
			DrawRotaGraphF(particles_[i].pos.x, particles_[i].pos.y,
				1.0, 0.0,
				images_[particles_[i].size],
				true);

			// 描画モード無効化
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
}

/// <summary>
/// 解放処理
/// </summary>
void Blast::Release(void)
{
	for (int i = 0; i < IMAGE_SIZE_MAX; i++)
	{
		// 画像解放
		DeleteGraph(images_[i]);
	}

	// 配列解放
	particles_.clear();
}



void Blast::CreateEffect(Vector2F pos)
{
	isActive_ = true;

	size_t size = particles_.size();
	for (int i = 0; i < size; i++)
	{
		// 位置
		particles_[i].pos = pos;

		particles_[i].size = GetRand(IMAGE_SIZE_MAX - 1);

		// 移動ランダム算出
		particles_[i].speed = MIN_SPEED + GetRand(static_cast<int>(MAX_SPEED - MIN_SPEED));

		// 生存時間
		particles_[i].life = MAX_LIFE;

		// 透過値
		particles_[i].blendRate = 255.0f;
	}
}

void Blast::InitParticle(void)
{
	// 発射方向の弧度
	float deg = 0.0f;

	while (deg < 360.0f)
	{
		// パーティクル作製
		BlastEffectInfo particle;

		float red = deg * DX_PI_F / 180.0f;

		particle.dirVec.x = cosf(red);
		particle.dirVec.y = sinf(red);

		// 配列追加
		particles_.push_back(particle);

		// 指定角度分、進行する
		deg += SPLIT_ANGLE;
	}
}

/// <summary>
/// エフェクト終了判定
/// </summary>
/// <returns>エフェクトが終了したか否か</returns>
bool Blast::GetIsHit(void)
{
	return isActive_;
}