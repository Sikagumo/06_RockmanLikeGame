#pragma once
#include "Vector2.h"
#include "Vector2F.h"
#include "Utility.h"
class GameScene;
class Blast;

class Shot
{
public:

	// 画像ハンドル
	const char* IMAGE_HANDLE = "Image/Player/P1/Shot.png";

	static constexpr int SIZE_X = 16; // 縦サイズ
	static constexpr int SIZE_Y = 16; // 横サイズ

	static constexpr float START_SPEED = 5.0f; // 発射速度
	static constexpr float MOVE_ACC_POWER = 1.0f;  // 発射速度上昇率
	static constexpr float MOVE_ACC_MAX	  = 17.5f; // 発射速度上限

	Shot(void);
	~Shot(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(GameScene* scene);

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
	/// 発射位置割り当て(整数型)
	/// </summary>
	/// <param name="pos">整数ベクトル</param>
	void SetShotPos(Vector2 pos, Utility::DIR dir);

	/// <summary>
	/// 発射位置割り当て(実数型)
	/// </summary>
	/// <param name="pos">実数ベクトル</param>
	void SetShotPos(Vector2F pos, Utility::DIR dir);

	/// <summary>
	/// 衝突割り当て
	/// </summary>
	void SetIsActive(bool flag);

	/// <summary>
	/// 爆発クラス 取得
	/// </summary>
	/// <returns>爆破クラス</returns>
	Blast* GetEffect(void);

	bool GetIsActive(void);

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <returns>衝突したか否か</returns>
	bool GetIsHit(void);


private:
	
	GameScene* gameScene_; // ゲームシーン処理

	int image_;		   // 画像
	Vector2F pos_;	   // 発射位置
	float veloX_; // 発射速度
	Utility::DIR dir_; // 向き
	Blast* blast_; //爆発

	bool isBlast_; // 爆発するか否か
	bool isActive_;

	
};