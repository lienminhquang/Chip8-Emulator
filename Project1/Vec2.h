#pragma once
struct Vec2
{
    Vec2(int x_, int y_) :
	   x(x_),
	   y(y_)
    {

    }

    Vec2():x(0),y(0) {}

    const Vec2& operator +(const Vec2& other)
    {
	   return { this->x + other.x, this->y + other.y };
    }

    int x;
    int y;
};

