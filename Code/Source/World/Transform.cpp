#include "Transform.h"
using namespace LateralEngine;

Transform::Transform()
{

}

Transform::~Transform()
{
}

void Transform::Update()
{

}

void Transform::addLocation(glm::vec3 Location)
{
	Position = glm::translate(Position, Location);
}

void Transform::addLocation(float x, float y, float z)
{
	Position = glm::translate(Position, glm::vec3(x,y,z));
}

void Transform::setLocation(glm::vec3 Location)
{
	Position = glm::translate(glm::mat4(1.0f), Location);
}

void Transform::setLocation(float x, float y, float z)
{
	Position = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Transform::addRotation(float angle, glm::vec3 Rotation)
{
	this->Rotation = glm::rotate(this->Rotation, glm::radians(angle), Rotation);
}

void Transform::setRotation(float angle, glm::vec3 Rotation)
{
	this->Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Rotation);
}

void Transform::setScale(glm::vec3 Scale)
{
	this->Scale = glm::scale(glm::mat4(1.0f), Scale);
}

void Transform::setLocationAndRotation(glm::vec3 Location, float angle, glm::vec3 Rotation)
{
	using namespace std;
	this->Position = glm::translate(glm::mat4(1.0f), Location);
	this->Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Rotation);
}

void Transform::setColor(glm::vec3 Color)
{
	this->Color = Color;
}

void Transform::addColor(glm::vec3 Color)
{
	this->Color += Color;
}

