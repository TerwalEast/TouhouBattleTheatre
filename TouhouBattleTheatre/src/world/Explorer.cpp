#include "Explorer.h"
#include "../TextureManager.h"


Explorer::Explorer()
{
	TextureManager::GetInstance().LoadTexture("explorer", "assets/actor/explorer.png");
}

void Explorer::Update(const float delta)
{

}

void Explorer::Render()
{
	
	if (_showDetail)
	{
		if (_ui._showPortrait)
		{

		}

	}
}

void Explorer::HandleInput()
{
}
