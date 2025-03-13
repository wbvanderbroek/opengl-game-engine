#pragma once
class GameObject
{
public:
	virtual ~GameObject() = default;

	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void Quit() {}
};