#pragma once
//========================== Point ==================================
class Point
{
public:
	float x, y;
	Point(const Point&);
	explicit Point(float = 0, float = 0);
	Point(float, float, float, float);
	virtual~Point();
	bool operator == (const Point&) const;
	Point operator + (const Point&);
	Point operator - (const Point&);
	Point operator * (const Point&);
	void reverse();
	void normalize();
	int length();
};

