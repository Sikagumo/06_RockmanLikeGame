#pragma once
#include "SceneBase.h"
#include "Vector2.h"
class Stage;
class Player;
class Blast;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// ワールド座標をマップ座標に変換する
	Vector2 World2MapPos(Vector2 worldPos);

	/// <summary>
	/// マップチップを使用した衝突判定
	/// </summary>
	/// <param name="worldPos">衝突判定の対象座標</param>
	/// <returns>マップチップに衝突したか否か</returns>
	bool IsCollisionStage(Vector2 worldPos);

	// カメラ座標の取得
	Vector2 GetCameraPos(void);

private:

	// ステージ
	Stage* stage_;

	// プレイヤー
	Player* player_;

	// カメラ位置
	Vector2 cameraPos_;
};

