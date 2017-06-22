#ifndef POINT_H_
#define POINT_H_

#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

template<int nDimensions = 2 >
class Point
{
private:
	std::array< int, nDimensions > elements_;

	Point(std::initializer_list<int> pos)
	{
		int i = 0;
		for (auto elem : pos)
		{
			elements_[i] = elem;
			i++;
		}
	}

public:
	//int ValueType;

	int& operator[](int const i)
	{
		return elements_[i];
	}

	int const& operator[](int const i) const
	{
		return elements_[i];
	}

	void operator+=(Point const& other)
	{
		for (int i = 0; i < nDimensions; ++i)
		{
			elements_[i] += other.elements_[i];
		}
	}

	void operator-=(Point const& other)
	{
		for (int i = 0; i < nDimensions; ++i)
		{
			elements_[i] -= other.elements_[i];
		}
	}

	friend Point operator+(Point const& a, Point const& b)
	{
		Point ret(a);

		ret += b;
		return ret;
	}

	friend Point operator-(Point const&a, Point const& b)
	{
		Point ret(a);

		ret -= b;
		return ret;
	}
	//Point() : elements_() {}

	template < typename ...ArgsT>
	Point(ArgsT ...pos)
	{
		Point({ pos... });
	}



	/*
	Point(int x, int y)
	{
		STATIC_ASSERT(nDimensions == 2);
		elements_[0] = x;
		elements_[1] = y;
	}

	Point(int x, int y, int z)
	{
		STATIC_ASSERT(nDimensions == 3);
		elements_[0] = x;
		elements_[1] = y;
		elements_[2] = z;
	}
	*/
};
/*

typedef Point< int, 2 > Point2D;
typedef Point< int, 3 > Point3D;
*/

#endif //POINT_H_
