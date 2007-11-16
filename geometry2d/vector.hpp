/*
 *	$Id: vector.hpp,v 1.12 2007-11-16 05:34:37 mayhewn Exp $
 *
 *	2D Geometry - Vector
 *
 *	Neil Mayhew - 2007-11-08 - neil_mayhew@users.sourceforge.net
 */

#ifndef GEOMETRY2D_VECTOR_HPP
#define GEOMETRY2D_VECTOR_HPP

#include <cmath>

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

	inline double dot(const Vector& v, const Vector& w) // Dot product
	{
		return v.x() * w.x() + v.y() * w.y();
	}

	inline double operator * (const Vector& v, const Vector& w)
	{
		return dot(v, w);
	}

	inline double cross(const Vector& v, const Vector& w) // Cross or vector product
	{
		return v.x() * w.y() - v.y() * w.x(); // Same as dot(rotate(v), w)
	}

	inline double operator ^ (const Vector& v, const Vector& w)
	{
		return cross(v, w);
	}
}

#endif//GEOMETRY2D_VECTOR_HPP
