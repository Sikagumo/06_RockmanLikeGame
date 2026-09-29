#pragma once
#include "../Vector2F.h"
#include <vector>

class Blast
{
public:

	/* --- <<< 定数定義 >>> --- */

	//画像の種類
	static constexpr int IMAGE_SIZE_MAX = 6;

	static constexpr int SIZE_X = 16; // 横サイズ
	static constexpr int SIZE_Y = 16; // 縦サイズ

	// 粒子の拡散
	static constexpr float SPLIT_ANGLE = 30.0f;

	// 粒子の生存生成
	static constexpr float MAX_LIFE = 0.2f;

	// 生存時間減少
	static constexpr float DEC_LIFE = 0.01f;

	// 最大移動速度
	static constexpr float MIN_SPEED = 2.0f;

	// 最小移動速度
	static constexpr float MAX_SPEED = 8.0f;

	// 透過最大値
	static constexpr float MAX_ALPHA = 255.0f;

	// 透過減少値
	static constexpr float DEC_ALPHA = 8.0f;

	/* --- <<< 構造体定義 >>> --- */
	// 粒子の１つずつの情報
	struct BlastEffectInfo
	{
		Vector2F pos;			// 座標
		Vector2F dirVec;		// sinとcosから算出
		int size = 0;	// サイズ
		float speed = 0.0f;	// 速さ
		float life = 0.0f;	// 生存時間	
		float blendRate = 0.0f; // 透過値
	};


	/* --- <<< 関数プロトタイプ宣言 >>> --- */

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Blast(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// 指定座標にエフェクトを生成
	/// </summary>
	/// <param name="pos">指定する座標</param>
	void CreateEffect(Vector2F pos);

	/// <summary>
	/// エフェクト終了判定
	/// </summary>
	/// <returns>演出が終了したか否か</returns>
	bool GetIsHit(void);

private:

	// 画像
	int images_[IMAGE_SIZE_MAX];

	// 粒子
	std::vector<BlastEffectInfo> particles_;

	bool isActive_; // 有効フラグ

	/// <summary>
	/// 粒子の初期化
	/// </summary>
	void InitParticle(void);

};