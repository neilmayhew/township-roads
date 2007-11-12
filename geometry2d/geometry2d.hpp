/*
 *	$Id: geometry2d.hpp,v 1.9 2007-11-12 00:33:24 mayhewn Exp $
 *
 *	2D Geometry
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cmath>
#include <stdexcept>
#include <istream>
#include <ostream>

namespace geometry2d
{
	class Vector
	{
		double x_, y_;
	public:
		explicit Vector(double x = 0, double y = 0)
			: x_(x), y_(y) {}

		double x() const { return x_; }
		double y() const { return y_; }

		Vector& operator += (const Vector& other)
		{
			x_ += other.x_; y_ += other.y_; return *this;
		}
		Vector& operator -= (const Vector& other)
		{
			x_ -= other.x_; y_ -= other.y_; return *this;
		}
		Vector& operator *= (double scalar)
		{
			x_ *= scalar; y_ *= scalar; return *this;
		}
		Vector& operator /= (double scalar)
		{
			x_ /= scalar; y_ /= scalar; return *this;
		}

		double modulus() const
		{
			return std::sqrt(x_ * x_ + y_ * y_);
		}
		double length() const
		{
			return modulus();
		}
		operator double() const
		{
			return modulus();
		}
	};

	inline const Vector operator + (const Vector& v, const Vector& w)
	{
		return Vector(v) += w;
	}

	inline const Vector operator - (const Vector& v, const Vector& w)
	{
		return Vector(v) -= w;
	}

	inline const Vector operator * (const Vector& v, double s)
	{
		return Vector(v) *= s;
	}

	inline const Vector operator / (const Vector& v, double s)
	{
		return Vector(v) /= s;
	}

	inline double modulus(const Vector& v)
	{
		return v.modulus();
	}

	inline Vector normalize(const Vector& v)
	{
		return v / v.modulus();
	}

	inline Vector rotate(const Vector& v) // 1/4 turn CCW
	{
		return Vector(-v.y(), v.x());
	}

	inline Vector rotate(const Vector& v, double angle) // Radians CCW
	{
		double c = std::cos(angle);
		double s = std::sin(angle);
		return Vector(c * v.x() - s * v.y(), s * v.x() + c * v.y());
	}

	inline double operator * (const Vector& v, const Vector& w) // Dot product
	{
		return v.x() * w.x() + v.y() * w.y();
	}

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

	// Stream i/o of Points and Lines - format is x1,y1:x2,y2

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

	// Computations

	inline Line parallel(const Line& l, double distance) // The parallel line 'distance' away
	{
		return l + normalize(rotate(l)) * distance;;
	}

	inline Line operator || (const Line& l, double d)
	{
		return parallel(l, d);
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

#endif//GEOMETRY_HPP
