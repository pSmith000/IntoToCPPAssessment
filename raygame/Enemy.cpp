#include "Enemy.h"
#include "UIText.h"
#include "HealthComponent.h"
#include "Transform2D.h"
#include "MoveComponent.h";
#include "SpriteComponent.h";
#include "CircleCollider.h"
#include "Engine.h"

Enemy::Enemy(float x, float y, const char* name, float health) : Actor:: Actor(x, y, name)
{
	m_maxhealth = health;
}

Enemy::~Enemy()
{
}

void Enemy::start()
{
	//Health Component
	setHealthComponent(dynamic_cast<HealthComponent*>(addComponent(new HealthComponent())));
	m_healthComponent->setUIText(m_healthText);
	m_healthComponent->setCurrHealth(m_health);
	//Call base start method
	//Set the starting values
	m_health = m_maxhealth;

	m_enemyCollider = new CircleCollider(25, this);

	setCollider(m_enemyCollider);

	m_isAlive = true;

	Actor::start();
}

void Enemy::update(float deltaTime)
{
	MathLibrary::Vector2 HealthTextPos = MathLibrary::Vector2{ (getTransform()->getLocalPosition().x - 20),
		(getTransform()->getLocalPosition().y - 50) };
	m_healthText->getTransform()->setLocalPosition(HealthTextPos);
	Actor::update(deltaTime);
}

void Enemy::draw()
{
	Actor::draw();
	getCollider()->draw();
}

void Enemy::onCollision(Actor* other)
{
	if (other->getName() == "PlayerBullet") 
	{
		m_health -= 5;
		Engine::destroy(other);
	}
	if (other->getName() == "PlayerUpgradedBullet")
	{
		m_health -= 10;
		Engine::destroy(other);
	}
}


	