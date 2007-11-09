/*
 *	$Id: township-roads.cpp,v 1.6 2007-11-09 04:44:46 mayhewn Exp $
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

double operator * (Vector v, Vector w) // Dot product
{
	return v.x * w.x + v.y * w.y;
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
	Vector e = rotate(m.second - m.first);
	Vector f = m.first - l.first;
	
	if (std::fabs(d * e) < 1e-9)
		throw std::runtime_error("intersecting parallel lines");

	return l.first + d * ((f * e) / (d * e));
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
			<< "   eg: "<<argv[0]<<" 1.1,0.2:6.3,-0.8 6.1 -0.2,0.2:-0.5,9.3 5.8\n"
			<< "       (should give: 5.42579,5.57989)\n";
		return 1;
	}

	Line h;	   arg[0] >> h;
	double dh; arg[1] >> dh;
	Line v;	   arg[2] >> v;
	double dv; arg[3] >> dv;

	std::cerr
		<< "solve("<<h<<", "<<dh<<", "<<v<<", "<<dv<<"): "
		<<  solve(h, dh, v, dv) << "\n";

#if SVG
	Line hp = parallel(h,  dh);
	Line vp = parallel(v, -dv);
	Point p = solve(h, dh, v, dv);

	Line xaxis(Point(-2, 0), Point(10, 0));
	Line yaxis(Point(0, -2), Point(0, 10));
	Line left(Point(-2, -2), Point(-2, 10));
	Line right(Point(10, -2), Point(10, 10));
	Line bottom(Point(-2, -2), Point(10, -2));
	Line top(Point(-2, 10), Point(10, 10));
	Line hfull(intersect(h, left), intersect(h, right));
	Line vfull(intersect(v, bottom), intersect(v, top));
	Line hpfull(intersect(hp, left), intersect(hp, right));
	Line vpfull(intersect(vp, bottom), intersect(vp, top));
	Vector hdir = h.second - h.first;
	Vector vdir = v.second - v.first;
	Line hperp(p, p + rotate(hdir / modulus(hdir)) * -dh); 
	Line vperp(p, p + rotate(vdir / modulus(vdir)) *  dv); 

	std::cout <<
"<?xml version='1.0' encoding='UTF-8'?>\n"
"<svg xmlns='http://www.w3.org/2000/svg'\n"
"   width='8in' height='8in'\n"
"   viewBox='-2 -10 12 12'\n"
"   overflow='hidden'>\n"
"  <g stroke='#000000' stroke-width='0.05'>\n"
"    <circle id='spot' fill='red' stroke='none'\n"
"       cx='"<<p.x<<"' cy='"<<-p.y<<"' r='0.1'/>\n"
"    <text id='label' font-size='0.25' stroke='none'\n"
"       x='"<<p.x<<"' y='"<<-p.y<<"' dx='0.5'>\n"
"      "<<p.x<<","<<p.y<<"\n"
"    </text>\n"
"    <line id='xaxis' stroke-width='0.01' stroke='#4444dd'\n"
"       x1='"<<xaxis.first.x<<"' y1='"<<-xaxis.first.y<<"' x2='"<<xaxis.second.x<<"' y2='"<<-xaxis.second.y<<"'/>\n"
"    <line id='yaxis' stroke-width='0.01' stroke='#4444dd'\n"
"       x1='"<<yaxis.first.x<<"' y1='"<<-yaxis.first.y<<"' x2='"<<yaxis.second.x<<"' y2='"<<-yaxis.second.y<<"'/>\n"
"    <line id='hfull' stroke-opacity='0.2'\n"
"       x1='"<<hfull.first.x<<"' y1='"<<-hfull.first.y<<"' x2='"<<hfull.second.x<<"' y2='"<<-hfull.second.y<<"'/>\n"
"    <line id='vfull' stroke-opacity='0.2'\n"
"       x1='"<<vfull.first.x<<"' y1='"<<-vfull.first.y<<"' x2='"<<vfull.second.x<<"' y2='"<<-vfull.second.y<<"'/>\n"
"    <line id='hpfull' stroke-opacity='0.2' stroke='#44dd44'\n"
"       x1='"<<hpfull.first.x<<"' y1='"<<-hpfull.first.y<<"' x2='"<<hpfull.second.x<<"' y2='"<<-hpfull.second.y<<"'/>\n"
"    <line id='vpfull' stroke-opacity='0.2' stroke='#44dd44'\n"
"       x1='"<<vpfull.first.x<<"' y1='"<<-vpfull.first.y<<"' x2='"<<vpfull.second.x<<"' y2='"<<-vpfull.second.y<<"'/>\n"
"    <line id='h'\n"
"       x1='"<<h.first.x<<"' y1='"<<-h.first.y<<"' x2='"<<h.second.x<<"' y2='"<<-h.second.y<<"'/>\n"
"    <line id='v'\n"
"       x1='"<<v.first.x<<"' y1='"<<-v.first.y<<"' x2='"<<v.second.x<<"' y2='"<<-v.second.y<<"'/>\n"
"    <line id='hp' stroke='#44dd44'\n"
"       x1='"<<hp.first.x<<"' y1='"<<-hp.first.y<<"' x2='"<<hp.second.x<<"' y2='"<<-hp.second.y<<"'/>\n"
"    <line id='vp' stroke='#44dd44'\n"
"       x1='"<<vp.first.x<<"' y1='"<<-vp.first.y<<"' x2='"<<vp.second.x<<"' y2='"<<-vp.second.y<<"'/>\n"
"    <line id='hperp' stroke-opacity='0.6' stroke-width='0.01'\n"
"       x1='"<<hperp.first.x<<"' y1='"<<-hperp.first.y<<"' x2='"<<hperp.second.x<<"' y2='"<<-hperp.second.y<<"'/>\n"
"    <line id='vperp' stroke-opacity='0.6' stroke-width='0.01'\n"
"       x1='"<<vperp.first.x<<"' y1='"<<-vperp.first.y<<"' x2='"<<vperp.second.x<<"' y2='"<<-vperp.second.y<<"'/>\n"
"  </g>\n"
"</svg>\n";
#endif //SVG
}
