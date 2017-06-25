
#include <iostream>
#include "Point.h"
#include <memory>
#include "Group.h"
#include <list>
#include <map>
#include <array>            // std::array
#include <functional>

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

using std::endl;
using namespace std;


template<class T, class GroupingFunc, typename P, size_t DIM>
struct MatrixGroupCollector {
	static void groupCollect(T* dest, size_t dest_size, const size_t* dest_dimensions, bool* visit, P type, GroupingFunc func, Group<DIM>& g, size_t* point) {
		ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) = true;
		//g.addPoint(new Point2D(x, y));

		for (size_t j = 0; j < DIM; j++) {
			size_t value = point[j];

			size_t newValue = value - 1;
			if (newValue >= 0 && newValue < dest_dimensions[j] && !ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) && func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point)) == type)
			{
				point[j] = newValue;
				groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			}
			newValue = value + 1;
			if (newValue >= 0 && newValue < dest_dimensions[j] && !ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) && func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point)) == type)
			{
				point[j] = newValue;
				groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			}

			point[j] = value;
		}
	}
};



template<class T, size_t DIMENSIONS, class GroupingFunc, typename P, size_t DIM>
struct MatrixGroupsCollector {
	static void groupsCollect(T* dest,  size_t dest_size, const size_t* dest_dimensions, bool* visit, std::map < P, std::list<Group<DIM>>>& types, GroupingFunc func, size_t* point, int indexDim) {
		size_t dest_size0 = dest_size / dest_dimensions[indexDim];
		for (size_t i = 0; i < dest_dimensions[indexDim]; ++i) {
			point[indexDim] = i;
			MatrixGroupsCollector<T, DIMENSIONS - 1, GroupingFunc,P, DIM>::groupsCollect(dest, dest_size0, dest_dimensions, visit, types,  func, point, indexDim+1);
		}
	}
};

template<class T, class GroupingFunc, typename P, size_t DIM>
struct MatrixGroupsCollector<T, 1, GroupingFunc, P, DIM> {
	static void groupsCollect(T* dest, size_t dest_size, const size_t* dest_dimensions, bool* visit, std::map < P, std::list<Group<DIM>>>& types, GroupingFunc func, size_t* point, int indexDim){
		for (size_t i = 0; i < dest_size; ++i) {
			point[indexDim] = i;
			P type = func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point));
			if (ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point)) {
				continue;
			}
			Group<DIM> g;
			MatrixGroupCollector<T, GroupingFunc, P, DIM>::groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			types[type].push_back(g);
		}
	}
};


//get item in place (x,y,z,w ... ) according to arrayOfIndexes
template<typename T, size_t DIM>
struct ItemFromMat {
	static T& getItemFromMat(T* mat, const size_t* dest_dimensions, size_t* arrayOfIndexes) {
		size_t index = 0;
		for (size_t i = 0; i < DIM; i++) {
			index += arrayOfIndexes[i] * dest_dimensions[i];
		}
		return mat[index];
	}
};

template<class T, size_t DIMENSIONS>
struct MatrixCopier {
	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {
		size_t dest_size0 = dest_size / dest_dimensions[0];
		size_t source_size0 = source_size / source_dimensions[0];
		for (size_t i = 0; i < source_dimensions[0]; ++i) {
			MatrixCopier<T, DIMENSIONS - 1>::copy(dest + (i * dest_size0), dest_size0, dest_dimensions + 1, source + (i * source_size0), source_size0, source_dimensions + 1);
		}
	}
};

template<class T>
struct MatrixCopier<T, 1> {
	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {
		for (size_t i = 0; i < source_size; ++i) {
			dest[i] = source[i];
		}
	}
};


template<class T, size_t DIMENSIONS>
struct MatrixPrinter {
	static void print(const T* values, size_t size, const size_t* dimensions, std::ostream& out = cout) {
		out << '{';
		size_t size0 = size / dimensions[0];
		MatrixPrinter<T, DIMENSIONS - 1>::print(values, size0, dimensions + 1, out);
		for (size_t i = 1; i < dimensions[0]; ++i) {
			out << ',';
			MatrixPrinter<T, DIMENSIONS - 1>::print(values + (i*size0), size0, dimensions + 1, out);
		}
		out << '}';
	}
};

template<class T>
struct MatrixPrinter<T, 1> {
	static void print(const T* values, size_t size, const size_t* dimensions, std::ostream& out = cout) {
		out << '{';
		out << values[0];
		for (size_t i = 1; i < size; ++i) {
			out << ',' << values[i];
		}
		out << '}';
	}
};

template<class T, size_t DIMENSIONS>
class Matrix {
	constexpr static size_t NUM_DIMENSIONS = DIMENSIONS;
	std::unique_ptr<T[]> _array = nullptr;
	size_t _dimensions[DIMENSIONS] = {};
	const size_t _size = 0;
	friend class Matrix<T, DIMENSIONS + 1>;
public:
	size_t size() const { return _size; }
	Matrix() {}

	// DIMENSIONS == 1
	// We want here a ctor with this signature:
	//    Matrix(const std::initializer_list<T>& values) {
	// but SFINAE is needed to block this ctor from being used by Matrix with DIMENSIONS > 1
	// The SFINAE results with the exact signature we want, but only for cases DIMENSIONS == 1
	template<typename G = T>
	Matrix(const std::initializer_list<typename std::enable_if_t<DIMENSIONS == 1, G>>& values) {
		const_cast<size_t&>(_size) = values.size();
		_dimensions[0] = _size;
		_array = std::make_unique<T[]>(_size);
		size_t i = 0;
		for (auto& val : values) {
			_array[i++] = val;
		}
	}
	// DIMENSIONS > 1
	// We want here a ctor with this signature:
	//    Matrix(const std::initializer_list<Matrix<T, DIMENSIONS-1>>& values) {
	// although this ctor is not useful and won't be used by Matrix<T, 1> it will still be in class
	// and thus would compile as part of Matrix<T, 1> resulting with a parameter of type:
	//          const std::initializer_list< Matrix<T, 0> >& values
	// having Matrix<T, 0> as a parameter - even for a function that is not in use, inside a class that is used
	// would require the compiler to instantiate Matrix<T, 0> class which results with a warning
	// the SFINAE below solves this warning.
	// The SFINAE results with the exact signature we want, but only for cases DIMENSIONS > 1
	template<typename G = T>
	Matrix(const std::initializer_list<Matrix<typename std::enable_if_t<(DIMENSIONS > 1), G>, DIMENSIONS - 1>>& values) {
		_dimensions[0] = values.size();
		for (auto& m : values) {
			for (size_t dim = 0; dim < DIMENSIONS - 1; ++dim) {
				if (m._dimensions[dim] > _dimensions[dim + 1]) {
					_dimensions[dim + 1] = m._dimensions[dim];
				}
			}
		}
		size_t size = 1;
		for (size_t dim = 0; dim < DIMENSIONS; ++dim) {
			size *= _dimensions[dim];
		}

		const_cast<size_t&>(_size) = size;
		_array = std::make_unique<T[]>(_size); // "zero initialized" - T()
		size_t i = 0;
		size_t dest_size = _size / _dimensions[0];
		for (auto& m : values) {
			MatrixCopier<T, DIMENSIONS - 1>::copy(&(_array[i * dest_size]), dest_size, _dimensions + 1, m._array.get(), m._size, m._dimensions);
			++i;
		}
	}

	Matrix(Matrix&& m) {
		*this = std::move(m);
	}

	auto& operator=(Matrix&& m) {
		std::swap(_array, m._array);
		std::swap(const_cast<size_t&>(_size), const_cast<size_t&>(m._size));
		std::swap(_dimensions, m._dimensions);
		return *this;
	}

	size_t getDimension(size_t i) const {
		return _dimensions[i];
	}

	friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
		MatrixPrinter<T, DIMENSIONS>::print(m._array.get(), m._size, m._dimensions, out);
		return out;
	}

	template< typename G = T, size_t DIM = DIMENSIONS, class GroupingFunc >
	auto groupValues(GroupingFunc groupingFunc) {

		using P = std::result_of_t<GroupingFunc(T&)>;
		//Insilize visit board

		size_t size = 1;
		for (size_t dim = 0; dim < DIMENSIONS; ++dim) {
			size *= _dimensions[dim];
		}

		std::unique_ptr<bool[]> visitArray = std::make_unique<bool[]>(_size); // "zero initialized" - T()
		//MatrixValueInitilaizer<T, bool, DIM>::valueInitilaize(&(visitArray[size]), _array.get(), _size, _dimensions, false);
		for (int i = 0; i < size;i++) {
			cout << visitArray.get()[i]  << " " ;
		}
		std::map < P, std::list<Group<DIM>>> types;

		std::unique_ptr<size_t[]> point = std::make_unique<size_t[]>(DIM);
		size_t i = 0;
		size_t dest_size = _size / _dimensions[0];
		MatrixGroupsCollector<G, DIM, GroupingFunc, P, DIM>::groupsCollect(&(_array[0]), dest_size, _dimensions, &(visitArray[0]), types, groupingFunc, &(point[0]), 0);

		return types;
	}
	
};

// defining Matrix2d<T> as Matrix<T, 2>
template<class T>
using Matrix2d = Matrix<T, 2>;

// defining Matrix3d<T> as Matrix<T, 3>
template<class T>
using Matrix3d = Matrix<T, 3>;

// defining IntVector as Matrix<int, 1>
using IntVector = Matrix<int, 1>;