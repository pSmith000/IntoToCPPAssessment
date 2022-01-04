#include "Enemy.h"
#include "UIText.h"
#include "HealthComponent.h"

Enemy::Enemy(float x, float y, const char* name, float health) : Actor:: Actor(x, y, name)
{
	m_maxhealth = health;
	m_health = m_maxhealth;
	m_isAlive = true;
}

Enemy::~Enemy()
{
}

void Enemy::start()
{
	//Health Component
	/*setHealthComponent(dynamic_cast<HealthComponent*>(addComponent(new HealthComponent())));
	m_healthComponent->setMinHealth(m_health);
	m_healthComponent->setMaxHealth(m_maxhealth);
	m_healthComponent->setUIText(m_healthText);*/
	//Call base start method
	Actor::start();
}
