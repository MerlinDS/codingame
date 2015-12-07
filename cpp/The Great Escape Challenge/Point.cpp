#include <cmath>
#include "Point.h"
#include <iostream>
//========================== Point ==================================
Point::Point(const Point& p) :x(p.x), y(p.y){}
Point::Point(float ix, float iy) :x(ix), y(iy){}
Point::Point(float x0, float x1, float y0, float y1)
{
	this->x = (x1 - x0) / 2 + x0;
	this->y = (y1 - y0) / 2 + y0;
}
Point::~Point(){}
bool Point::operator==(const Point& point) const{
	return this->x == point.x && this->y == point.y;
}

Point Point::operator+(const Point& point)
{
	return Point(this->x + point.x, this->y + point.y);
}

Point Point::operator-(const Point& point)
{
	return Point(this->x - point.x, this->y - point.y);
}

Point Point::operator*(const Point& point)
{
	return Point(this->x * point.x, this->y * point.y);
}

void Point::reverse()
{
	auto tmp = this->x;
	this->x = this->y;
	this->y = tmp;
}

void Point::normalize()
{
	this->x = this->x != 0 ? this->x / abs(this->x) : 0;
	this->y = this->y != 0 ? this->y / abs(this->y) : 0;
}

int Point::length()
{
	return this->x + this->y;
}