#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <functional>
#include <utility>

class Shape_concept
{
public:
    virtual ~Shape_concept() = default;
    virtual void draw() const = 0;
    // potentially more polymorphic operations
};

template<typename Shape_t>
class Shape_model : public Shape_concept
{
public:
    using Draw_strategy = std::function<void(Shape_t const&)>;

    explicit Shape_model(Shape_t shape, Draw_strategy drawer)
    : shape_ { std::move(shape) }
    , drawer_ { std::move(drawer) }
    {}

    void draw() const override { drawer_(shape_); }

private:
    Shape_t shape_;
    Draw_strategy drawer_;
};

#endif
