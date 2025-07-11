/* ---------------
　プレイヤーヘッダ
 ----------------- */

#pragma once
#include "Utility.h"
#include "Vector2F.h"
#include "Vector2.h"
class Shot;
class GameScene;
#include <vector>

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{

public:

	enum class ANIM_STATE
	{
		IDLE,	// 待機状態
		JUMP,	// ジャンプ状態
		RUN,	// 走る
		DAMAGED, // 被ダメージ
		MAX,	// 状態数
	};

	enum class ATK_STATE
	{
		NONE, // 通常状態
		SHOT, // 攻撃状態
		MAX,  // 状態数
	};

	/* --- <<< 定数定義 >>> --- */

	const char* IMAGE_HANDLE = "Image/Player/TestPlayer.png";
	static constexpr int SIZE_X = 96;
	static constexpr int SIZE_Y = 96;


	// 最大アニメーション数
	static constexpr int MAX_NUM_ANIM = 4;

	// アニメーション速度
	static constexpr float ANIM_SPEED = 0.1f;

	/* << 当たり判定サイズ >> */
	static constexpr int COL_SIZE_X		 = 32;				 // 横
	static constexpr int COL_SIZE_Y		 = 32;				 // 縦
	static constexpr int HALF_COL_SIZE_X = (COL_SIZE_X / 2); // 横(半分)
	static constexpr int HALF_COL_SIZE_Y = (COL_SIZE_Y / 2); // 縦(半分)
	static constexpr int COL_OFFSET		 = 7;				 // 補正値

	/* << 初期位置 >> */
	static constexpr int PLAYER_INIT_POS_X = 350; // 横
	static constexpr int PLAYER_INIT_POS_Y = 350; // 縦

	// 移動速度
	static constexpr float MOVE_SPEED = 3.5f;

	// 重力

	static constexpr float GRAVITY	   = 0.25f; // 毎フレーム加算値
	static constexpr float MAX_GRAVITY = 8.5f; // 重力最大値
	static constexpr float DEC_GRAVITY = 2.5f; // 重力減少率

	// ジャンプ
	static constexpr float JUMP_POWER = -8.5f; // ジャンプ力
	static constexpr int MAX_JUMP = 2; // 最大ジャンプ数

	static constexpr float MOVE_ACC_POWER = 0.25f; // 加速度
	static constexpr float MOVE_DEC_POWER = 0.075f; // 減速量
	static constexpr float MAX_MOVE_SPEED = 3.5f;  // 最大移動速度


	/* --- <<< 関数プロトタイプ宣言 >>> --- */

	Player(void);  // コンストラクタ
	~Player(void); // デストラクタ

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="gameScene">ゲームシーンのデータ</param>
	void Init(GameScene* gameScene);

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


private:

	/* --- <<< 構造体定義 >>> --- */

	struct HeightPos
	{
		Vector2 left;	 // 左
		Vector2 center; // 中央
		Vector2 right;	 // 右
	};

	struct WidthPos
	{
		Vector2 up;	 // 上
		Vector2 center; // 中央
		Vector2 down;   // 下
	};

	struct IsHitSide
	{
		bool bottom;
		bool top;
		bool left;
		bool right;
	};


	/* --- <<< メンバ変数定義 >>> --- */

	// ゲームシーンのポインタ変数
	GameScene* gameScene_;

	Vector2F pos_; // 座標

	std::vector<Shot*> shot_; // 弾丸

	// 状態ごとの画像(モーション・攻撃・アニメーション数)
	int images_[static_cast<int>(ANIM_STATE::MAX)][static_cast<int>(ATK_STATE::MAX)][MAX_NUM_ANIM];

	ANIM_STATE animState_; // アニメーション状態
	ATK_STATE atkState_;   // 攻撃状態
	Utility::DIR dir_;	   // 向き状態

	int animImgIdx_;	   // アニメーション番号
	float animCounter_;	   // アニメーションカウンタ

	float movePosX_; // 左右移動量
	float movePosY_; //上下移動量
	int jumpCount_;	 // ジャンプ数

	bool isHitWall_; // 壁に当たったか否か
	bool isJump_;	 // ジャンプしているか否か

	HeightPos* bottom_; // 下位置
	HeightPos *top_;	   // 上位置
	WidthPos* left_;	 // 左位置
	WidthPos* right_; // 右位置

	IsHitSide* isHit_;


	/* --- <<< 関数プロトタイプ宣言 >>> --- */

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(void);

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	void Jump(void);

	/// <summary>
	/// 画像読み込み
	/// </summary>
	void LoadImages(void);

	/// <summary>
	/// 画像削除
	/// </summary>
	void DeleteImages(void);

	/// <summary>
	/// Y軸の衝突位置計算
	/// </summary>
	/// <param name="isTop">上位置を判定するか否か</param>
	/// <returns> 縦衝突位置(構造体) </returns>
	HeightPos* ClacHeightPos(bool isTop);

	/// <summary>
	/// X軸の衝突位置計算
	/// </summary>
	/// <param name="isLeft">左位置を判定するか否か</param>
	/// <returns> 横衝突位置(構造体) </returns>
	WidthPos* CalcWidthPos(bool isLeft);

	/// <summary>
	/// アニメーション更新処理
	/// </summary>
	void UpdateAnim(void);

	/// <summary>
	/// アニメーションテスト処理
	/// </summary>
	void UpdateChangeAnimTest(void);

	/// <summary>
	/// アニメーション変更処理
	/// </summary>
	/// <param name="state">変更後の状態</param>
	/// <param name="isResetAnimCount">アニメーションを初期化するか否か</param>
	void ChangeAnimState(ANIM_STATE state, bool isResetAnimCount);

#if _DEBUG
	/// <summary>
	/// アニメーションデバッグ表示
	/// </summary>
	void DrawAnimState(void);

	/// <summary>
	/// 攻撃状態のデバッグ表示
	/// </summary>
	/// <param name=""></param>
	void DrawAtkState(void);
#endif


	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="middle"> 中央 当たり判定</param>
	/// <param name="side1"> 横 当たり判定</param>
	/// <param name="side2"> 横 当たり判定</param>
	/// <returns>物に衝突したか否か</returns>
	bool IsHitPos(Vector2 middle, Vector2 side1, Vector2 side2);

	/// <summary>
	/// 上側当たり判定
	/// </summary>
	void CollisionTop(void);

	/// <summary>
	/// 下側当たり判定処理
	/// </summary>
	void CollisionBottom(void);

	/// <summary>
	/// 左側当たり判定処理
	/// </summary>
	void CollisionLeft(void);

	/// <summary>
	/// 右側当たり判定処理
	/// </summary>
	void CollisionRight(void);
};