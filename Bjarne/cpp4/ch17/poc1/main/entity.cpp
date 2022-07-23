#include "entity.h"

Entity::Entity(std::string name)
: name_ {std::move(name)}
{
    std::cout << "ctor: " << name_ << '\n';
}

Entity::~Entity()
{
    std::cout << "dtor: " << name_ << '\n';
}

Entity::Entity(const Entity& other)
: name_ {other.name_}
{
    std::cout << "copy ctor: " << name_ << '\n';
}

Entity& Entity::operator=(const Entity& other)
{
    name_ = other.name_;
    std::cout << "copy asign: " << name_ << '\n';
    return *this;
}

Entity::Entity(Entity&& other)
: name_ {std::move(other.name_)}
{
    std::cout << "move ctor: " << name_ << '\n';
}

Entity& Entity::operator=(Entity&& other)
{
    name_ = std::move(other.name_);
    std::cout << "move asign: " << name_ << '\n';
    return *this;
}

Entity f(Entity arg)
{
    return arg;
}
