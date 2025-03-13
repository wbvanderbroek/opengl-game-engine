#include "Player.h"
#include <iostream>

Player::Player()
{
}

void Player::Start()
{
	std::cout << "start called from player" << std::endl;
}

void Player::Update(float deltaTime)
{
	std::cout << "update called from player" << std::endl;
}

void Player::Quit()
{
}
