/*
 *	$Id: streamio.hpp,v 1.12 2007-11-16 05:34:37 mayhewn Exp $
 *
 *	2D Geometry - Stream I/O
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#ifndef GEOMETRY2D_STREAMIO_HPP
#define GEOMETRY2D_STREAMIO_HPP

#include "vector.hpp"
#include "point.hpp"
#include "line.hpp"

#include <istream>
#include <ostream>

namespace geometry2d
{
	// Stream i/o of Vectors, Points and Lines - format is x1,y1:x2,y2

	inline std::ostream& operator << (std::ostream& s, const Vector& v)
	{
		return s << v.x() << ',' << v.y();
	}

	inline std::ostream& operator << (std::ostream& s, const Point& p)
	{
		return s << p.x() << ',' << p.y();
	}

	inline std::ostream& operator << (std::ostream& s, const Line& l)
	{
		return s << l.first() << ':' << l.second();
	}

	inline std::istream& operator >> (std::istream& s, Vector& v)
	{
		double x, y; char c;
		if (s >> x >> c >> y) // We don't check c
			v = Vector(x, y);
		return s;
	}

	inline std::istream& operator >> (std::istream& s, Point& p)
	{
		Vector v;
		if (s >> v)
			p = Point() + v;
		return s;
	}

	inline std::istream& operator >> (std::istream& s, Line& l)
	{
		Point first, second; char c;
		if (s >> first >> c >> second) // We don't check c
			l = Line(first, second);
		return s;
	}
}

#endif//GEOMETRY2D_STREAMIO_HPP
