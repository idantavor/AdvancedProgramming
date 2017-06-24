
#include <iostream>
#include "Point.h"
#include <memory>
#include "Group.h"
#include <list>
#include <map>
#include <array>            // std::array

#define STATIC_ASSERT( e ) static_assert( e, "!(" #e ")" )

using std::cout;
using std::endl;

template<class T, size_t DIMENSIONS>
class Matrix;

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

template<class T, class G, size_t DIMENSIONS>
struct MatrixValueInitilaizer {
	static void valueInitilaize(G* dest, const T* source, size_t source_size, const size_t* source_dimensions, G value) {
		size_t size0 = source_size / source_dimensions[0];
		for (size_t i = 0; i < source_dimensions[0]; ++i) {
			MatrixValueInitilaizer<T, DIMENSIONS - 1>::valueInitilaize(dest + (i * size0), source + (i * size0), size0, source_dimensions + 1, value);
		}
	}
};

template<class T, class G>
struct MatrixValueInitilaizer<T, G, 1> {
	static void valueInitilaize(G* dest, const T* source, size_t source_size, const size_t* source_dimensions, G value) {
		for (size_t i = 0; i < source_size; ++i) {
			dest[i] = value;
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

	template< typename G = T, size_t DIM = DIMENSIONS, typename Func>
	std::map<auto, std::list<Group<DIM>>> groupValues(Func func) {

		//Insilize visit board
		std::unique_ptr<bool[]> visitArray = std::make_unique<bool[]>(_size); // "zero initialized" - T()
		MatrixValueInitilaizer<T, bool, DIM>::valueInitilaize(&(visitArray[i * _size]), _array, _size, DIM, false);



		if (DIM == 2) {
			//create boards and do checks
			for (int row = 0; row < ROWS; row++) {
				for (int column = 0; column < COLS; column++)
				{
					auto type = func(_array[Row][column]);
					if (visitArray[row][column]) {
						continue;
					}
					Group<DIM> g();
					<Func> collectGroup(row, column, type, visitArray, func, g);
					bool typeNotExists = m.find(chck) == m.end();
					if (typeNotExists) {
						types[type] = std::list<Group<DIM>>(g);
					}
					else{
						types[type].push_back(g);
					}
				}
			}
		}

		else if (DIM == 3) {
			//create boards and do checks
			for (int depth = 0; row; row++) {
				for (int row = 0; row < ROWS; row++) {
					for (int column = 0; column < COLS; column++)
					{
						auto type = func(_array[depth][Row][column]);
						if (visitArray[depth][Row][column]) {
							continue;
						}
						Group<DIM> g();
						if (visitArray[depth][Row][column]) {
							<Func> collectGroup(depth, Row, column, type, visitArray, func, g);
							bool typeNotExists = m.find(chck) == m.end();
							if (typeNotExists) {
								types[type] = std::list<Group<DIM>>(g);
							}
							else {
								types[type].push_back(g);
							}

						}
					}
				}
			}

		}
		return types;
	}


	template<typename G = T, size_t DIM = DIMENSIONS, typename Func>
	void collectGroup(int x, int y , auto type, std::unique_ptr<bool[]> visitArray, Func func, Group<DIM>& g) const {
		STATIC_ASSERT(DIM == 2);
		visitBoard[x][y] = true;
		g.addPoint(new Point2D(x, y));
		int x_i = x + 1;
		if (x_i >= 0 && x_i < ROWS && !visitArray[x_i][y] && func(_array[x_i][y]) == type)
		{
			collectGroup(x_i, y, type, visitArray, func, g);
		}
		int y_i = y + 1;
		if (y_i >= 0 && y_i < COLS && !visitArray[x][y_i] && func(_array[x][y_i]) == type)
		{
			collectGroup(x, y_i, type, visitArray, func, g);
		}
		x_i = x - 1;
		if (x_i >= 0 && x_i < ROWS && !visitArray[x_i][y] && func(_array[x_i][y]) == type)
		{
			collectGroup(x_i, y, type, visitArray, func, g);
		}
		y_i = y - 1;
		if (y_i >= 0 && y_i < COLS && !visitArray[x][y_i] && func(_array[x][y_i]) == type)
		{
			collectGroup(x, y_i, type, visitArray, func, g);
		}
	}

	template<typename G = T, size_t DIM = DIMENSIONS, typename Func, typename P>
	void collectGroup(unsigned int x, unsigned int y, unsigned int z, P type, std::unique_ptr<bool[]> visitArray, Func func, Group<DIM>& g) const {
		STATIC_ASSERT(DIM == 3);
		visitBoard[z][x][y] = true;
		g.addPoint(new Point3D(x, y, z));
		unsigned int x_i = x + 1;
		if (x_i >= 0 && x_i < board->getRowSize() && !visitArray[z][x_i][y] && func(_array[x_i][y][z]) == type)
		{
			collectGroup(x_i, y, z, type, visitArray, func, g);
		}
		unsigned int y_i = y + 1;
		if (y_i >= 0 && y_i < board->getColSize() && !visitArray[z][x][y_i] && func(_array[x][y_i][z]) == type)
		{
			collectGroup(x, y_i, z, type, visitArray, func, g);
		}
		unsigned int z_i = z + 1;
		if (z_i >= 0 && z_i < board->getDepthSize() && !visitArray[z_i][x][y] && func(_array[x][ y][z_i]) == type)
		{
			collectGroup(x, y, z_i, type, visitArray, func, g);
		}
		x_i = x - 1;
		if (x_i >= 0 && x_i < board->getRowSize() && !visitArray[z][x_i][y] && func(_array[x_i][y][z])== type)
		{
			collectGroup(x_i, y, z, type, visitArray, func, g);
		}
		y_i = y - 1;
		if (y_i >= 0 && y_i < board->getColSize() && !visitArray[z][x][y_i] && func(_array[x][y_i][z]) == type)
		{
			collectGroup(x, y_i, z, type, visitArray, func, g);
		}
		z_i = z - 1;
		if (z_i >= 0 && z_i < board->getDepthSize() && !visitArray[z_i][x][y] && func(_array[x][y][ z_i]) == type)
		{
			collectGroup(x, y, z_i, type, visitArray, func, g);
		}
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