#pragma once
#include <map>
#include <DxLib.h>

class InputManager
{
public:
	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputManager* GetInstance(void);

	// インスタンスの削除
	static void DeleteInstance(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// リソースの破棄
	void Delete(void);

	// 判定を行うキーをクリア
	void Clear(void);

	// キーが押されているか
	bool IsNew(int key);

	// キーが今押されたか
	bool IsTrgDown(int key);

	// キーは離されたか
	bool IsTrgUp(int key);

private:

	// キーの入力状態の情報
	struct Info
	{
		int key;
		bool keyOld;
		bool keyNew;
		bool keyTrgDown;
		bool keyTrgUp;
	};

	// シングルトン用インスタンス
	static InputManager* instance_;

	// キー情報はstd::mapでキーと情報を紐付けておく
	std::map<int, InputManager::Info> keyInfos_;

	// コンストラクタ・デストラクタをprivateにして、
	// 外部から生成出来ない様にする
	InputManager(void);
	InputManager(const InputManager& manager);
	~InputManager(void);

	// 判定を行うキーを追加
	void Add(int key);
};

