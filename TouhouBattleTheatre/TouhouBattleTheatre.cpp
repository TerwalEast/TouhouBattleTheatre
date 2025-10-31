// TouhouBattleTheatre.cpp: 定义应用程序的入口点。
//


#include "TouhouBattleTheatre.h"
#include "src/TestApplication.h"

#define STB_IMAGE_IMPLEMENTATION

int main()
{
	return TestApplication::GetInstance().run();
}
