
#include <iostream>
#include "Point.h"
#include <memory>
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

	template< typename G = T, typename Func>
	void GameData::collectGroup(Point p, G typeOFGroup, bool& visitBoard, Func func) const {

		for(auto )
	}
	
	template< typename G = T, typename Func>
	void groupValues(Func func) {

		//Insilize visit board
		visitArray = std::make_unique<bool[]>(_size); // "zero initialized" - T()
		MatrixValueInitilaizer<T, bool, DIMENSIONS>::valueInitilaize(&(visitArray[i * _size]), _array, _size, _dimensions, false);


		//create boards and do checks
		for (int row = 0; row < ROWS; row++) {
			string matrixRow = matrix[row];
			for (int column = 0; column < COLS; column++)
			{
				T type = matrixRow[column];
				if (visitBoard[row][column]) {
					continue;
				}

				<T, Func> collectGroup(row, column, type, visitBoard, func);
			}
		}
	}


	template<typename G = T, typename Func>
	void Matrix::collectGroup(Point p , T type,  visitMatrix, Func func) const {
		STATIC_ASSERT(DIMENSIONS == 2);
		visitBoard[x][y] = true;
		ship.addPointToTheList(x, y);
		int x_i = x + 1;
		if (x_i >= 0 && x_i < ROWS && !visitBoard[x_i][y] && func(matrix[x_i][y]) == func(type))
		{
			shipCollectChars(x_i, y, type, visitBoard, ship);
		}
		int y_i = y + 1;
		if (y_i >= 0 && y_i < COLS && !visitBoard[x][y_i] && func(matrix[x][y_i]) == func(type))
		{
			shipCollectChars(x, y_i, type, visitBoard, ship);
		}
		x_i = x - 1;
		if (x_i >= 0 && x_i < ROWS && !visitBoard[x_i][y] && func(matrix[x_i][y]) == func(type))
		{
			shipCollectChars(x_i, y, type, visitBoard, ship);
		}
		y_i = y - 1;
		if (y_i >= 0 && y_i < COLS &&
			!visitBoard[x][y_i] && func(matrix[x][y_i]) == func(type))
		{
			shipCollectChars(x, y_i, type, visitBoard, ship);
		}
	}


	void GameData::shipCollectChars(unsigned int x, unsigned int y, unsigned int z, T type, visitMatrix, Func func) const {
		STATIC_ASSERT(DIMENSIONS == 3);
		visitBoard[z][x][y] = true;
		ship.addPointToTheList(x, y, z);
		unsigned int x_i = x + 1;
		if (x_i >= 0 && x_i < board->getRowSize() && !visitBoard[z][x_i][y] && func(matrix[x_i][y][z]) == type)
		{
			shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
		}
		unsigned int y_i = y + 1;
		if (y_i >= 0 && y_i < board->getColSize() && !visitBoard[z][x][y_i] && func(matrix[x][y_i][z] == type)
		{
			shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
		}
		unsigned int z_i = z + 1;
		if (z_i >= 0 && z_i < board->getDepthSize() && !visitBoard[z_i][x][y] && func(matrix[x][ y][z_i] == type)
		{
			shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
		}
		x_i = x - 1;
		if (x_i >= 0 && x_i < board->getRowSize() && !visitBoard[z][x_i][y] && board->getCharAt(x_i, y, z) == shipChar)
		{
			shipCollectChars(x_i, y, z, shipChar, visitBoard, ship);
		}
		y_i = y - 1;
		if (y_i >= 0 && y_i < board->getColSize() && !visitBoard[z][x][y_i] && board->getCharAt(x, y_i, z) == shipChar)
		{
			shipCollectChars(x, y_i, z, shipChar, visitBoard, ship);
		}
		z_i = z - 1;
		if (z_i >= 0 && z_i < board->getDepthSize() && !visitBoard[z_i][x][y] && board->getCharAt(x, y, z_i) == shipChar)
		{
			shipCollectChars(x, y, z_i, shipChar, visitBoard, ship);
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



