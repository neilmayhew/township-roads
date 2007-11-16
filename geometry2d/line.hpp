/*
 *	$Id: line.hpp,v 1.12 2007-11-16 05:34:37 mayhewn Exp $
 *
 *	2D Geometry - Line
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#ifndef GEOMETRY2D_LINE_HPP
#define GEOMETRY2D_LINE_HPP

#include "point.hpp"
#include "vector.hpp"

#include <cmath>
#include <stdexcept>

namespace geometry2d
{
	class Line
	{
		Point first_, second_;
	public:
		Line(Point f, Point s)
			: first_(f), second_(s) {}
		Line() {}

		Point first()  const { return first_; }
		Point second() const { return second_; }

		Line& operator += (const Vector& v)
		{
			first_ += v; second_ += v; return *this;
		}

		Line& operator -= (const Vector& v)
		{
			first_ -= v; second_ -= v; return *this;
		}

		operator Vector() const
		{
			return second_ - first_;
		}
	};

	inline Line operator + (const Line& l, const Vector& v)
	{
		return Line(l) += v;
	}

	inline Line operator - (const Line& l, const Vector& v)
	{
		return Line(l) -= v;
	}

	inline Line offset(const Line& l, double distance) // The parallel line 'distance' away
	{
		return l + normalize(rotate(l)) * distance;;
	}

	inline Line operator || (const Line& l, double d) // Looks like parallel lines
	{
		return offset(l, d);
	}

	inline Point intersect(const Line& l, const Line& m)
	{
		Vector d = l;
		Vector e = rotate(m);
		Vector f = m.first() - l.first();

		if (std::fabs(d * e) < 1e-9)
			throw std::runtime_error("trying to intersect parallel lines");

		return l.first() + d * ((f * e) / (d * e));
	}

	inline Point operator & (const Line& l, const Line& m)
	{
		return intersect(l, m);
	}
}

#endif//GEOMETRY2D_LINE_HPP
