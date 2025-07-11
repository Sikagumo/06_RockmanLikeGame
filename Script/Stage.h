#pragma once
class Vector2;
class GameScene;

class Stage
{
public:

	// ステージの分割画像
	static constexpr int NUM_MAP_CHIPS_X = 13;	// マップチップ横画像
	static constexpr int NUM_MAP_CHIPS_Y = 1;	// マップチップ縦枚数
	static constexpr int NUM_MAP_CHIPS = NUM_MAP_CHIPS_X * NUM_MAP_CHIPS_Y;	// 合計

	// マップチップのサイズ
	static constexpr int CHIP_SIZE_X = 32;	// 横
	static constexpr int CHIP_SIZE_Y = 32;	// 縦
	static constexpr int CHIP_HALF_SIZE_X = CHIP_SIZE_X / 2;	// 横半分
	static constexpr int CHIP_HALF_SIZE_Y = CHIP_SIZE_Y / 2;	// 縦半分

	// 地上マップのサイズ(縦枚数×横枚数)
	static constexpr int MAP_GROUND_SIZE_X = 48;
	static constexpr int MAP_GROUND_SIZE_Y = 15;

	Stage();
	~Stage();

	void Init(GameScene* scene);
	void Update();
	void Draw();
	void Release();

	// 外部ファイルから地上のステージデータを読み込む
	void LoadGroundCsvData(void);

	// マップチップ番号を取得する
	int GetChipNo(Vector2 mapPos);

private:

	// マップ画像
	int imgMapChips_[NUM_MAP_CHIPS];

	// 地上マップ
	int groundMap_[MAP_GROUND_SIZE_Y][MAP_GROUND_SIZE_X];

	// ゲームシーンのポインタ変数
	GameScene* gameScene_;

};