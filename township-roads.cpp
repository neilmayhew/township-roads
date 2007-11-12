/*
 *	$Id: township-roads.cpp,v 1.8 2007-11-12 00:16:12 mayhewn Exp $
 *
 *	Test for 2D Geometry - township-roads problem
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#include "geometry.hpp"

#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <string>

using geometry2d::Point;
using geometry2d::Line;
using geometry2d::Vector;

// Problem - dh, dv are perpendicular distances from lines h, v respectively

inline Point solve(const Line& h, double dh, const Line& v, double dv)
{
	// Note: -dv because we measure distance to the *E* of v, and to the N of h,
	// and assuming h is specified W-E and v, S-N
	return (h || dh) & (v || -dv);
}

std::string svg(const Line& l, std::string id, std::string attrs = std::string());
std::string itoa(int);

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
	Point p = solve(h, dh, v, dv);

	Line hp = h ||  dh;
	Line vp = v || -dv;
	
	Point min(-2, -2);
	Point max(10, 10);

	Line xaxis(Point(min.x(), 0), Point(max.x(), 0));
	Line yaxis(Point(0, min.y()), Point(0, max.y()));

	Line left(min, Point(min.x(), max.y()));
	Line right(Point(max.x(), min.y()), max);
	Line bottom(min, Point(max.x(), min.y()));
	Line top(Point(min.x(), max.y()), max);

	Line hfull(h & left, h & right);
	Line vfull(v & bottom, v & top);
	Line hpfull(hp & left, hp & right);
	Line vpfull(vp & bottom, vp & top);

	Vector hdir = h;
	Vector vdir = v;

	Line hperp(p, p + rotate(hdir / modulus(hdir)) * -dh); 
	Line vperp(p, p + rotate(vdir / modulus(vdir)) *  dv); 

	std::cout
	<< "<?xml version='1.0' encoding='UTF-8'?>\n"
	<< "<svg xmlns='http://www.w3.org/2000/svg'\n"
	<< "   width='8in' height='8in'\n"
	<< "   viewBox='"<<min.x()<<" "<<-max.y()<<" "<<(max - min).x()<<" "<<(max - min).y()<<"'\n"
	<< "   overflow='hidden'>\n"
	<< "  <g stroke='#000000' stroke-width='0.05'>\n"
	<< "    <circle id='spot' fill='red' stroke='none'\n"
	<< "       cx='"<<p.x()<<"' cy='"<<-p.y()<<"' r='0.1'/>\n"
	<< "    <text id='label' font-size='0.25' font-weight='bold' stroke='none'\n"
	<< "       x='"<<p.x()<<"' y='"<<-p.y()<<"' dx='0.5' dy='-0.25'>\n"
	<< "      "<<p.x()<<","<<p.y()<<"\n"
	<< "    </text>\n"
	<< svg(xaxis, "xaxis", "stroke-width='0.01' stroke='#4444dd'")
	<< svg(yaxis, "yaxis", "stroke-width='0.01' stroke='#4444dd'");
	
	int count = 0;
	for (double x = min.x() + 1.0; x < max.x(); x += 1.0)
	{
		if (x == 0.0)
			continue;
		Line tick(Point(x, -0.1), Point(x, 0.1));
		std::cout << svg(tick, "tick" + itoa(++count),
			"stroke-width='0.01' stroke='#4444dd'");
	}
	
	for (double y = min.y() + 1.0; y < max.y(); y += 1.0)
	{
		if (y == 0.0)
			continue;
		Line tick(Point(-0.1, y), Point(0.1, y));
		std::cout << svg(tick, "tick" + itoa(++count),
			"stroke-width='0.01' stroke='#4444dd'");
	}

	std::cout
	<< svg(hfull, "hfull", "stroke-opacity='0.2'")
	<< svg(vfull, "vfull", "stroke-opacity='0.2'")
	<< svg(hpfull, "hpfull", "stroke-opacity='0.2' stroke='#44dd44'")
	<< svg(vpfull, "vpfull", "stroke-opacity='0.2' stroke='#44dd44'")
	<< svg(hperp, "hperp", "stroke-opacity='0.6' stroke-width='0.01'")
	<< svg(vperp, "vperp", "stroke-opacity='0.6' stroke-width='0.01'")
	<< svg(h, "h")
	<< svg(v, "v")
	<< svg(hp, "hp", "stroke='#44dd44'")
	<< svg(vp, "vp", "stroke='#44dd44'")
	<< "  </g>\n"
	<< "</svg>\n";
#endif //SVG
}

std::string svg(const Line& l, std::string id, std::string attrs)
{
	std::ostringstream s;
	s <<
"    <line id='"<<id<<"'"<<(attrs.size()?" ":"")<<attrs<<"\n"
"       x1='"<<l.first().x()<<"' y1='"<<-l.first().y()<<"' x2='"<<l.second().x()<<"' y2='"<<-l.second().y()<<"'/>\n";
	return s.str();
}

std::string itoa(int i)
{
	std::ostringstream s; s << i; return s.str();
}

