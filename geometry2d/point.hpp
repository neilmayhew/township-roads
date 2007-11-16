/*
 *	$Id: point.hpp,v 1.12 2007-11-16 05:34:37 mayhewn Exp $
 *
 *	2D Geometry - Point
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#ifndef GEOMETRY2D_POINT_HPP
#define GEOMETRY2D_POINT_HPP

#include "vector.hpp"

namespace geometry2d
{
	class Point
	{
		Vector displacement_;
	public:
		explicit Point(double x = 0, double y = 0)
			: displacement_(x, y) {}

		double x() const { return displacement_.x(); }
		double y() const { return displacement_.y(); }

		const Vector& displacement() const { return displacement_; }

		Point& operator += (const Vector& v)
		{
			displacement_ += v; return *this;
		}
		Point& operator -= (const Vector& v)
		{
			displacement_ -= v; return *this;
		}

		friend const Vector operator - (const Point&, const Point&);
	};

	inline const Point operator + (const Point& p, const Vector& v)
	{
		return Point(p) += v;
	}

	inline const Point operator - (const Point& p, const Vector& v)
	{
		return Point(p) -= v;
	}

	inline const Vector operator - (const Point& p, const Point& q)
	{
		return p.displacement_ - q.displacement_;
	}
}

#endif//GEOMETRY2D_POINT_HPP
