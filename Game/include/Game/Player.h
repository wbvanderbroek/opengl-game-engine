#include <Engine/GameObject.h>

class Player : public GameObject
{
public:
	explicit Player(ObjectStorage* storage);

	void OnCreate() override;
	void Start() override;
	void Update(float deltaTime) override;
	void OnDestroy() override;
	void OnQuit() override;
};