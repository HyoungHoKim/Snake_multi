#include "DefaultShared.hpp"

GameObject::GameObject() :
	mIndexInWorld(-1),
	mDoesWantToDie(false),
	mNetworkId(0),
	mColor(Colors::White),
{}

void GameObject::Update()
{}