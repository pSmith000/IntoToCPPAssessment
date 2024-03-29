#include "CircleCollider.h"
#include "AABBCollider.h"
#include <Vector2.h>
#include "Actor.h"
#include "Transform2D.h"
#include "raylib.h"

CircleCollider::CircleCollider(Actor* owner) : Collider::Collider(owner, ColliderType::CIRCLE)
{
    MathLibrary::Vector2 size = getOwner()->getTransform()->getScale();
    m_collisionRadius = (size.x > size.y ? size.x : size.y) / 2;
}

CircleCollider::CircleCollider(float collisionRadius, Actor* owner) : Collider::Collider(owner, ColliderType::CIRCLE)
{
    m_collisionRadius = collisionRadius;
}

bool CircleCollider::checkCollisionCircle(CircleCollider* otherCollider)
{
    //Return if this actor is checking collision against itself
    if (otherCollider->getOwner() == getOwner())
        return false;

    //Find the length of the radii combined
    float combinedRadii = otherCollider->getCollisionRadius() + getCollisionRadius();
    //Find the distance between the circle centers
    float distance = (otherCollider->getOwner()->getTransform()->getWorldPosition() - getOwner()->getTransform()->getWorldPosition()).getMagnitude();

    return distance <= combinedRadii;
}

bool CircleCollider::checkCollisionAABB(AABBCollider* otherCollider)
{
    //Return if this actor is checking collision against itself
    if (otherCollider->getOwner() == getOwner())
        return false;

    //Get the direction from this collider to the AABB
    MathLibrary::Vector2 direction = getOwner()->getTransform()->getWorldPosition() - otherCollider->getOwner()->getTransform()->getWorldPosition();

    //Clamp the direction vector to be within the bounds of the AABB
    if (direction.x < -otherCollider->getWidth() / 2)
        direction.x = -otherCollider->getWidth() / 2;
    else if (direction.x > otherCollider->getWidth() / 2)
        direction.x = otherCollider->getWidth() / 2;

    if (direction.y < -otherCollider->getHeight() / 2)
        direction.y = -otherCollider->getHeight() / 2;
    else if (direction.y > otherCollider->getHeight() / 2)
        direction.y = otherCollider->getHeight() / 2;

    //Add the direction vector to the AABB center to get the closest point to the circle
    MathLibrary::Vector2 closestPoint = otherCollider->getOwner()->getTransform()->getWorldPosition() + direction;

    //Sets the collision normal of this collider
    setCollisionNormal((closestPoint - getOwner()->getTransform()->getWorldPosition()).getNormalized());

    //Sets the collision normal of the other collider
    otherCollider->setCollisionNormal((getOwner()->getTransform()->getWorldPosition() - closestPoint).getNormalized());

    //Get the distance between the circle and the closest point found
    float distanceFromClosestPoint = (getOwner()->getTransform()->getWorldPosition() - closestPoint).getMagnitude();

    //Return true if the distance is less than the collision radius
    if (distanceFromClosestPoint <= m_collisionRadius)
        return true;

    return false;
}

void CircleCollider::draw()
{
    //Draws the collider
    RAYLIB_H::DrawCircleLines((int)getOwner()->getTransform()->getWorldPosition().x, (int)getOwner()->getTransform()->getWorldPosition().y, m_collisionRadius, WHITE);
}