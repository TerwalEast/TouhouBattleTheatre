#include "Explorer.h"
#include "../TextureManager.h"


Explorer::Explorer()
{
	TextureManager::GetInstance().LoadTexture("explorer", "assets/actor/explorer.png");
	_ui._showPortrait = true;
	_ui._actorName = "Shinki";
	_ui._actorDescription = "Test dummy";
	_ui._portraitCode = "Shinki";
	TextureManager::GetInstance().LoadTexture("",_ui._portraitCode.c_str());
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
