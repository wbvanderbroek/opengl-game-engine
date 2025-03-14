#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void OnCreate() override;
	void Start() override;
	void Update(float deltaTime) override;
	void Quit() override;
};