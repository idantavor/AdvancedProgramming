#ifndef POINT_H_
#define POINT_H_

#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

template<int nDimensions = 2 >
class Point
{
private:
	std::array< int, nDimensions > elements_;


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

	std::array< int, nDimensions > getElements() {
		return elements_;
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

	Point(int* pos)
	{
		for(int i=0; i<nDimensions; i++){
			elements_[i] = pos[i];
		}
	}

};
#endif //POINT_H_
