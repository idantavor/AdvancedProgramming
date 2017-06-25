
#include <iostream>
#include <memory>
#include <list>
#include <map>
#include <array>            // std::array
#include <functional>


using std::endl;
using namespace std;


template<class T, class GroupingFunc, typename P, int DIM>
struct MatrixGroupCollector {

	static void groupCollect(T* dest, int dest_size, const int* dest_dimensions, bool* visit, P type, GroupingFunc func, std::list<std::list<int>>& g , int* point) {
		ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) = true;
		//for (int i = 0; i < 24; i++) { cout << visit[i] << ' '; }
		//cout << endl;
		std::list<int> newPoint;
		getPoint(point, newPoint, DIM);
		g.push_back(newPoint);
		for (int j = DIM; j >= 0; j--) {
			int value = point[j];
			int newValue = value - 1;
			point[j] = newValue;
			if (newValue >= 0 && newValue < dest_dimensions[j] && !ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) && func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point)) == type)
			{
				groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			}
			newValue = value + 1;
			point[j] = newValue;
			if (newValue >= 0 && newValue < dest_dimensions[j] && !ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point) && func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point)) == type)
			{
				groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			}

			point[j] = value;
		}
	}

	static void getPoint(int* pos, std::list<int>& point, int dim)
	{
		for (int i = 0; i<dim; i++) {
			point.push_back(pos[i]);
		}
	}
};



template<class T, int DIMENSIONS, class GroupingFunc, typename P, int DIM>
struct MatrixGroupsCollector {
	static void groupsCollect(T* dest,  int dest_size, const int* dest_dimensions, bool* visit, std::map < P, std::list<list<list<int>>>> &types, GroupingFunc func, int* point, int indexDim) {
		int dest_size0 = dest_size / dest_dimensions[indexDim];
		for (int i = 0; i < dest_dimensions[indexDim]; ++i) {
			point[indexDim] = i;
			MatrixGroupsCollector<T, DIMENSIONS - 1, GroupingFunc,P, DIM>::groupsCollect(dest, dest_size0, dest_dimensions, visit, types,  func, point, indexDim+1);
		}
	}
};

template<class T, class GroupingFunc, typename P, int DIM>
struct MatrixGroupsCollector<T, 1, GroupingFunc, P, DIM> {
	static void groupsCollect(T* dest, int dest_size, const int* dest_dimensions, bool* visit, std::map < P, std::list<list<list<int>>>>& types, GroupingFunc func, int* point, int indexDim){
		for (int i = 0; i < dest_size; ++i) {
			point[indexDim] = i;

			P type = func(ItemFromMat<T, DIM>::getItemFromMat(dest, dest_dimensions, point));
			if (ItemFromMat<bool, DIM>::getItemFromMat(visit, dest_dimensions, point)) {
				continue;
			}
			std::list<std::list<int>> g;
			//Group<DIM> g;
			MatrixGroupCollector<T, GroupingFunc, P, DIM>::groupCollect(dest, dest_size, dest_dimensions, visit, type, func, g, point);
			types[type].push_back(g);
		}
	}
};


//get item in place (x,y,z,w ... ) according to arrayOfIndexes
template<typename T, int DIM>
struct ItemFromMat {
	static T& getItemFromMat(T* mat, const int* dest_dimensions, int* arrayOfIndexes) {
		int index = 0;
		int mult = 1;
		for (int j = 0; j < DIM; j++)mult *= dest_dimensions[j];
		for (int i = 0; i < DIM-1; i++) {
			mult = mult / dest_dimensions[i];
			index += arrayOfIndexes[i]*mult;
		}
		index += arrayOfIndexes[DIM - 1];
		return mat[index];
	}
};

template<class T, int DIMENSIONS>
struct MatrixCopier {
	static void copy(T* dest, int dest_size, const int* dest_dimensions, const T* source, int source_size, const int* source_dimensions) {
		int dest_size0 = dest_size / dest_dimensions[0];
		int source_size0 = source_size / source_dimensions[0];
		for (int i = 0; i < source_dimensions[0]; ++i) {
			MatrixCopier<T, DIMENSIONS - 1>::copy(dest + (i * dest_size0), dest_size0, dest_dimensions + 1, source + (i * source_size0), source_size0, source_dimensions + 1);
		}
	}
};

template<class T>
struct MatrixCopier<T, 1> {
	static void copy(T* dest, int dest_size, const int* dest_dimensions, const T* source, int source_size, const int* source_dimensions) {
		for (int i = 0; i < source_size; ++i) {
			dest[i] = source[i];
		}
	}
};


template<class T, int DIMENSIONS>
struct MatrixPrinter {
	static void print(const T* values, int size, const int* dimensions, std::ostream& out = cout) {
		out << '{';
		int size0 = size / dimensions[0];
		MatrixPrinter<T, DIMENSIONS - 1>::print(values, size0, dimensions + 1, out);
		for (int i = 1; i < dimensions[0]; ++i) {
			out << ',';
			MatrixPrinter<T, DIMENSIONS - 1>::print(values + (i*size0), size0, dimensions + 1, out);
		}
		out << '}';
	}
};

template<class T>
struct MatrixPrinter<T, 1> {
	static void print(const T* values, int size, const int* dimensions, std::ostream& out = cout) {
		out << '{';
		out << values[0];
		for (int i = 1; i < size; ++i) {
			out << ',' << values[i];
		}
		out << '}';
	}
};

template<class T, int DIMENSIONS>
class Matrix {
	constexpr static int NUM_DIMENSIONS = DIMENSIONS;
	std::unique_ptr<T[]> _array = nullptr;
	int _dimensions[DIMENSIONS] = {};
	const int _size = 0;
	friend class Matrix<T, DIMENSIONS + 1>;
public:
	int size() const { return _size; }
	Matrix() {}

	// DIMENSIONS == 1
	// We want here a ctor with this signature:
	//    Matrix(const std::initializer_list<T>& values) {
	// but SFINAE is needed to block this ctor from being used by Matrix with DIMENSIONS > 1
	// The SFINAE results with the exact signature we want, but only for cases DIMENSIONS == 1
	template<typename G = T>
	Matrix(const std::initializer_list<typename std::enable_if_t<DIMENSIONS == 1, G>>& values) {
		const_cast<int&>(_size) = static_cast<int>(values.size());
		_dimensions[0] = _size;
		_array = std::make_unique<T[]>(_size);
		int i = 0;
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
		_dimensions[0] = static_cast<int>(values.size());
		for (auto& m : values) {
			for (int dim = 0; dim < DIMENSIONS - 1; ++dim) {
				if (m._dimensions[dim] > _dimensions[dim + 1]) {
					_dimensions[dim + 1] = m._dimensions[dim];
				}
			}
		}
		int size = 1;
		for (int dim = 0; dim < DIMENSIONS; ++dim) {
			size *= _dimensions[dim];
		}

		const_cast<int&>(_size) = size;
		_array = std::make_unique<T[]>(_size); // "zero initialized" - T()
		int i = 0;
		int dest_size = _size / _dimensions[0];
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
		std::swap(const_cast<int&>(_size), const_cast<int&>(m._size));
		std::swap(_dimensions, m._dimensions);
		return *this;
	}

	int getDimension(int i) const {
		return _dimensions[i];
	}

	friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {
		MatrixPrinter<T, DIMENSIONS>::print(m._array.get(), m._size, m._dimensions, out);
		return out;
	}

	template< typename G = T, int DIM = DIMENSIONS, class GroupingFunc >
	auto groupValues(GroupingFunc groupingFunc) {

		using P = std::result_of_t<GroupingFunc(T&)>;
		//Insilize visit board

		int size = 1;
		for (int dim = 0; dim < DIMENSIONS; ++dim) {
			size *= _dimensions[dim];
		}
		std::unique_ptr<bool[]> visitArray = std::make_unique<bool[]>(_size); // "zero initialized" - T()
		//MatrixValueInitilaizer<T, bool, DIM>::valueInitilaize(&(visitArray[size]), _array.get(), _size, _dimensions, false);
		std::map < P, std::list<list<list<int>>>> types;

		std::unique_ptr<int[]> point = std::make_unique<int[]>(DIM);
		MatrixGroupsCollector<G, DIM, GroupingFunc, P, DIM>::groupsCollect(&(_array[0]), _size, _dimensions, &(visitArray[0]), types, groupingFunc, &(point[0]), 0);
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