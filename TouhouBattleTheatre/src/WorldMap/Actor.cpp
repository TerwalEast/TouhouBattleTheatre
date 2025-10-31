#include "Actor.h"

void ActorManager::AddActor(Actor* actor)
{
	_actors.push_back(actor);
}

void ActorManager::RemoveActor(Actor* actor)
{
	auto it = std::find(_actors.begin(), _actors.end(), actor);
	if (it != _actors.end())
	{
		_actors.erase(it);
	}
}

void ActorManager::UpdateActors(const float delta)
{
	for (auto& actor : _actors)
	{
		actor->Update(delta);
	}
}

void ActorManager::RenderActors()
{
	for (auto& actor : _actors)
	{
		actor->Render();
	}
}

void ActorManager::HandleInputActors()
{
	for (auto& actor : _actors)
	{
		actor->HandleInput();
	}
}


