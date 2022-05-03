#pragma once
#include "SDL.h"
const int thickness = 15,window_length = 800,window_width = 600;

struct vector2
{
	float x;
	float y;
};

class Game {
public:
	Game();

	bool Initialize();
	//初始化游戏

	void RunLoop();
	//刷新游戏直到游戏结束

	void ShutDown();
	//结束游戏
private:
	//游戏刷新过程中的帮助函数 : 处理输入、刷新游戏、画面迭代
	void ProgressInput();
	void UpdateGame();
	void GenerateOutput();

	//被SDL创建的窗口
	SDL_Window *mWindow;

	//游戏应该继续运行
	bool mIsRunnning;

	//画面渲染器
	SDL_Renderer *mRenderer;

	//流逝时间
	Uint32 mTicksCount;

	//球拍位置,球的位置,球的速度
	vector2 mPaddlePos, mBallPos,mBallVel;
	//四面墙壁以及球和球拍
	SDL_Rect wall_top, wall_right, wall_left, wall_bottom;
	SDL_Rect ball, paddle;

	//球拍的运动方向
	int mPaddleDir;
};