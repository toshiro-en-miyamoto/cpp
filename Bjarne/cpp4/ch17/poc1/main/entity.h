#include <string>
#include <iostream>
#include <utility>

class Entity
{
public:
    explicit Entity(std::string);
    ~Entity();

    Entity(const Entity&);
    Entity& operator=(const Entity&);

    Entity(Entity&&);
    Entity& operator=(Entity&&);

private:
    std::string name_;
};

Entity f(Entity);
