/*
 *	$Id: township-roads.cpp,v 1.2 2007-07-28 20:46:05 mayhewn Exp $
 *
 *	Given two township roads and perpendicular distances from them,
 *	calculate the corresponding point.
 *
 *	Neil Mayhew - 2007-07-26 - neil_mayhew@users.sourceforge.net
 */

// 2D geometry support

#include <cmath>
#include <stdexcept>

struct Point
{
	double x, y;
	
	Point(double x_ = 0, double y_ = 0)
		: x(x_), y(y_) {}
	
	Point operator + (Point other)
	{
		return Point(x + other.x, y + other.y);
	}
	Point operator - (Point other)
	{
		return Point(x - other.x, y - other.y);
	}
	Point operator * (double scalar)
	{
		return Point(x * scalar, y * scalar);
	}
	Point operator / (double scalar)
	{
		return Point(x / scalar, y / scalar);
	}
};

typedef Point Vector;

struct Line
{
	Point first, second;
	
	Line(Point f, Point s)
		: first(f), second(s) {}
	Line() {}
	
	Line operator + (Vector v)
	{
		return Line(first + v, second + v);
	}
};

double modulus(Vector v) // aka length
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector rotate(Vector v) // 90 degrees CCW
{
	return Vector(-v.y, v.x);
}

Line parallel(Line l, double distance) // The parallel line 'distance' away
{
	Vector direction = (l.second - l.first) / modulus(l.second - l.first);
	Vector offset = rotate(direction) * distance;
	return l + offset;
}

Point intersect(Line l, Line m)
{
	Vector d = l.second - l.first;
	Vector e = m.second - m.first;
	Vector q = l.first - m.first;
	double r = d.x * e.y - d.y * e.x;
	
	if (std::abs(r) < 1e-9)
		throw std::runtime_error("intersecting parallel lines");

	return Point(
		(d.x * e.x * q.y - (d.y * e.x * l.first.x - d.x * e.y * m.first.x)) /  r,
		(d.y * e.y * q.x - (d.x * e.y * l.first.y - d.y * e.x * m.first.y)) / -r);
}

// Problem - dh, dv are perpendicular distances from lines h, v respectively

Point solve(Line h, double dh, Line v, double dv)
{
	// Note: -dv because we measure distance to the *E* of v, and to the N of h,
	// and assuming h is specified W-E and v, S-N
	return intersect(parallel(h, dh), parallel(v, -dv));
}

// Stream i/o of Points and Lines - format is x1,y1:x2,y2

#include <ostream>
#include <istream>

std::ostream& operator << (std::ostream& s, const Point& p)
{
	return s << p.x << ',' << p.y;
}

std::ostream& operator << (std::ostream& s, const Line& l)
{
	return s << l.first << ':' << l.second;
}

std::istream& operator >> (std::istream& s, Point& p)
{
	char c;
	return s >> p.x >> c >> p.y; // We don't check c
}

std::istream& operator >> (std::istream& s, Line& l)
{
	char c;
	return s >> l.first >> c >> l.second; // We don't check c
}

// Test

#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>

int main(int argc, char** argv)
{
	// A vector of string streams, one for each cmd-line arg
	std::vector<std::istringstream> arg(argv + 1, argv + argc);

	if (arg.size() != 4)
	{
		std::cerr
			<< "Usage: "<<argv[0]<<" h dh v dv\n"
			<< "   eg: "<<argv[0]<<" 1.1,-0.1:2.3,-0.2 3.2 -0.1,0.2:-0.5,9.7 1.8\n"
			<< "       (should give: 1.58071,3.07103)\n";
		return 1;
	}

	Line h;	   arg[0] >> h;
	double dh; arg[1] >> dh;
	Line v;	   arg[2] >> v;
	double dv; arg[3] >> dv;

	std::cout
		<< "solve("<<h<<", "<<dh<<", "<<v<<", "<<dv<<"): "
		<<  solve(h, dh, v, dv) << "\n";
}
