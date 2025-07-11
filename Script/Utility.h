#pragma once
#include "Vector2.h"
#include "Vector2F.h"

// 共通で使用する定義
class Utility
{

public:

	// 移動方向
	enum class DIR
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		MAX
	};

	// 四捨五入
	static int Round(float value);

	// 四捨五入
	static Vector2 Round(Vector2F value);

	// 四捨五入
	static Vector2 Round(float x, float y);

};

