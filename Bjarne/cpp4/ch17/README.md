# 17. Construction, Cleanup, Copy, and Move

What are proper definitions of *copy* and *move*?

```c++
Entity f(Entity arg)    // 2. copy ctor copying s1 into arg
{
    return arg;         // 3. move ctor moving
}                       //    arg into a temporary variable

void test()
{
    Entity e1 {"foo"};
    e1 = f(e1);         // 4. move assignment moving
                        //    the temporary variable into s1
    Entity e2 {"bar"};
    e1 = e2;            // 8. copy assignment copying s2 into s1

    std::string_view expected {
        "ctor: foo\n"           // 1 init e1
        "copy ctor: foo\n"      // 2
        "move ctor: foo\n"      // 3
        "move assign: foo\n"    // 4
        "dtor: \n"              // 5 destroy arg
        "dtor: \n"              // 6 destroy temporary
        "ctor: bar\n"           // 7 init e2
        "copy assign: bar\n"    // 8
        "dtor: bar\n"           // 9 destroy e2
        "dtor: bar\n"           // A destroy e1
    };
}
```

Copy and move functions are declared:

```c++
class Entity
{
public:
    explicit Entity(std::string);
    ~Entity();

    Entity(const Entity&);              // copy ctor
    Entity& operator=(const Entity&);   // copy assign

    Entity(Entity&&);                   // move ctor
    Entity& operator=(Entity&&);        // move assign

private:
    std::string m_name;
};
```

They are defined as follows:

```c++
Entity::Entity(std::string name)
: m_name {std::move(name)} {}

Entity::~Entity() {}

Entity::Entity(const Entity& other)
: m_name {other.m_name} {}

Entity& Entity::operator=(const Entity& other)
{
    m_name = other.m_name;
    return *this;
}

Entity::Entity(Entity&& other)
: m_name {std::move(other.m_name)} {}

Entity& Entity::operator=(Entity&& other)
{
    m_name = std::move(other.m_name);
    return *this;
}
```
