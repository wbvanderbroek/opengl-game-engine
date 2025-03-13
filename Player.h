#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();
	void Start() override;
	void Update(float deltaTime) override;
	void Quit() override;
};