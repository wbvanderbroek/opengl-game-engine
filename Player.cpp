#include "Player.h"
#include <iostream>

Player::Player()
{
}

void Player::OnCreate()
{
	std::cout << "oncreate called from player" << std::endl;
}


void Player::Start()
{
	std::cout << "start called from player" << std::endl;


}

void Player::Update(float deltaTime)
{
}

void Player::Quit()
{
}
