#include <DxLib.h>
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	// インスタンスの生成
	Application::CreateInstance();

	// インスタンスの取得
	Application* instance = Application::GetInstance();

	if (instance->IsError())
	{
		return -1;
	}

	// ゲームループ起動
	instance->Run();

	// 解放処理
	instance->Destroy();
	if (instance->IsError())
	{
		return -1;
	}

	return 0;

}
