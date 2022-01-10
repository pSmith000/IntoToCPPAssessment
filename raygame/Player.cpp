#include "Player.h"
#include "InputComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "HealthComponent.h"
#include "Transform2D.h"
#include "Bullet.h"
#include "Engine.h"
#include "CircleCollider.h"
#include "Powerups.h"
#include <stdlib.h> 
#include <ctime>
#include <iostream>

void Player::start()
{
	Actor::start();

	//Input and move components
	m_inputComponent = dynamic_cast<InputComponent*>(addComponent(new InputComponent()));
	m_moveComponent = dynamic_cast<MoveComponent*>(addComponent(new MoveComponent()));
	m_moveComponent->setMaxSpeed(10);
	m_spriteComponent = dynamic_cast<SpriteComponent*>(addComponent(new SpriteComponent("images/player.png")));

	m_powerupStatus = false;
	m_currentTime = 0;
	//Health Component
	m_healthComponent = dynamic_cast<HealthComponent*>(addComponent(new HealthComponent()));
	m_healthComponent->setCurrHealth(m_health);
	m_healthComponent->setUIText(m_playerHealthText);
}

void Player::update(float deltaTime)
{
	Actor::update(deltaTime);

	if (m_health <= 0)
	{
		getTransform()->setLocalPosition({ 50, 100 });
		m_health = 20;
	}

	m_startTime = clock();

	if (m_startTime - m_currentTime > 15000)
	{
		float randomX = rand() % 900 + 50;
		float randomY = rand() % 700 + 50;
		Powerups* powerup = new Powerups(randomX, randomY, "powerup");
		Scene* currentScene = Engine::getCurrentScene();
		powerup->getTransform()->setScale({ 50, 50 });
		currentScene->addActor(powerup);

		m_currentTime = m_startTime;
	}

	MathLibrary::Vector2 moveDirection = m_inputComponent->getMoveAxis();

	if (m_inputComponent->getSpacePress() && m_powerupStatus == true) 
	{
		Scene* currentScene = Engine::getCurrentScene();
		Bullet* bullet = new Bullet(this, 500, getTransform()->getForward(), getTransform()->getLocalPosition().x, getTransform()->getLocalPosition().y, "PlayerUpgradedBullet");
		bullet->getTransform()->setScale({ 200, 200 });
		CircleCollider* m_bulletCollider = new CircleCollider(30, bullet);
		bullet->setCollider(m_bulletCollider);
		currentScene->addActor(bullet);
	}
	else if (m_inputComponent->getSpacePress())
	{
		Scene* currentScene = Engine::getCurrentScene();
		Bullet* bullet = new Bullet(this, 500, getTransform()->getForward(), getTransform()->getLocalPosition().x, getTransform()->getLocalPosition().y, "PlayerBullet");
		bullet->getTransform()->setScale({ 50, 50 });
		currentScene->addActor(bullet);
	}

	//If the velocity is greater than 0...
	if (m_moveComponent->getVelocity().getMagnitude() > 0)
		//...Rotate the player
		getTransform()->setForward(m_moveComponent->getVelocity());

	//Set the players velocity 
	m_moveComponent->setVelocity(moveDirection.getNormalized() * 200);


	//Sets the current health to the players current health
	m_healthComponent->setCurrHealth(m_health);


	//Set the health text to always be following the player
	MathLibrary::Vector2 HealthTextPos = MathLibrary::Vector2{ (getTransform()->getLocalPosition().x - 20),
		(getTransform()->getLocalPosition().y - 50) };
	m_playerHealthText->getTransform()->setLocalPosition(HealthTextPos);
	
}

void Player::draw()
{
	Actor::draw();
	getCollider()->draw();
}

void Player::onCollision(Actor* other)
{
	if (other->getName() == "enemy")
	{
		Engine::destroy(other);
		m_health -= 10;
	}
	if (other->getName() == "EnemyBullet")
	{
		Engine::destroy(other);
		m_health -= 5;
	}
	if (other->getName() == "powerup")
	{
		Engine::destroy(other);
		m_powerupStatus = true;
	}
	if (other->getName() == "house" || other->getName() == "house1" || other->getName() == "house2" || other->getName() == "wall")
	{
		getTransform()->setWorldPosition(getTransform()->getWorldPosition() - (m_moveComponent->getVelocity().getNormalized() * (CircleCollider(this).getCollisionRadius() / 3)));
	}
	if (other->getName() == "rotator")
	{
		m_health -= 5;
	}
		
}
