#ifndef LightsOutBoard_H
#define LightsOutBoard_H

#include <bitset>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <sstream>

#include <boost/python.hpp>

namespace lightsout
{
template <uint64_t WIDTH, uint64_t HEIGHT>
class LightsOutBoard
{
private:
	typedef std::bitset<WIDTH * HEIGHT> BOARD_TYPE;
	BOARD_TYPE board, outputVertices;
	std::default_random_engine generator;
	
	//Linear Algebra Method
	std::vector<BOARD_TYPE> nullSpace;
	std::vector<BOARD_TYPE> boardMatrix;
	std::vector<BOARD_TYPE> invMatrix;

public:
	LightsOutBoard() noexcept;
	LightsOutBoard(const BOARD_TYPE& board) noexcept;
	LightsOutBoard(const BOARD_TYPE& board, const BOARD_TYPE& outputVertices) noexcept;
	LightsOutBoard(const LightsOutBoard& board) noexcept;

	bool isOn(uint64_t location) const noexcept;
	bool isOnCoords(uint64_t x, uint64_t y) const noexcept;
	bool isOutputVertex(uint64_t location) const noexcept;
	bool isOutputVertexCoords(uint64_t x, uint64_t y) const noexcept;
	void flip(uint64_t location) noexcept;
	void flipList(const boost::python::list& list) noexcept;
	void flipCoords(uint64_t x, uint64_t y) noexcept;
	void flipBoard(const LightsOutBoard& lightsOutBoard) noexcept;
	uint64_t getNumOn() const noexcept;
	void set(uint64_t location, bool on) noexcept;
	void setList(const boost::python::list& list) noexcept;
	void setCoords(uint64_t x, uint64_t y, bool on) noexcept;
	void setAll(bool on) noexcept;
	void setRandom(double probability) noexcept;
	void setOutputVertex(uint64_t location, bool on) noexcept;
	void setOutputVertexList(const boost::python::list& list) noexcept;
	void setOutputVertexCoords(uint64_t x, uint64_t y, bool on) noexcept;
	void setAllOutputVertices(bool on) noexcept;
	void setRandomOutputVertices(double probability) noexcept;
	void setRandomSeed(uint64_t seed) noexcept;
	std::string pretty() const noexcept;
	std::ostream& print(std::ostream& out) const noexcept;

	// Linear Algebra Method
	void rowReduce();
	void solveLinAlgOther(const LightsOutBoard& other);
	void solveLinAlg();
	uint64_t numberOfNullSpace();

private:
	static BOARD_TYPE getLeftEdgeMask() noexcept;
	static BOARD_TYPE getRightEdgeMask() noexcept;

	void flipBitset(const BOARD_TYPE& board) noexcept;

	// Linear Algebra Method
	void solveLinAlg(const BOARD_TYPE& b);	
	void buildMatrix();
	uint64_t findPivot(uint64_t startRow, uint64_t pivotCol);
	BOARD_TYPE matrixMulti(std::vector<BOARD_TYPE> inv, const BOARD_TYPE& b);

	BOARD_TYPE LEFT_EDGE_MASK;
	BOARD_TYPE RIGHT_EDGE_MASK;
};

template <uint64_t W, uint64_t H>
LightsOutBoard<W, H>::LightsOutBoard(const BOARD_TYPE& board, const BOARD_TYPE& outputVertices) noexcept
	: board(board),
	  outputVertices(outputVertices),
	  LEFT_EDGE_MASK(getLeftEdgeMask()),
	  RIGHT_EDGE_MASK(getRightEdgeMask()),
	  generator(std::random_device()())
{
}

template <uint64_t W, uint64_t H>
LightsOutBoard<W, H>::LightsOutBoard() noexcept
	: LightsOutBoard(BOARD_TYPE{}, BOARD_TYPE{})
{
}

template <uint64_t W, uint64_t H>
LightsOutBoard<W, H>::LightsOutBoard(const LightsOutBoard& board) noexcept
	: LightsOutBoard(board.board, board.outputVertices)
{
}

template <uint64_t W, uint64_t H>
bool LightsOutBoard<W, H>::isOn(uint64_t location) const noexcept
{
	return board[location];
}

template <uint64_t W, uint64_t H>
bool LightsOutBoard<W, H>::isOnCoords(uint64_t x, uint64_t y) const noexcept
{
	return isOn(x + W * y);
}

template <uint64_t W, uint64_t H>
bool LightsOutBoard<W, H>::isOutputVertex(uint64_t location) const noexcept
{
	return outputVertices[location];
}

template <uint64_t W, uint64_t H>
bool LightsOutBoard<W, H>::isOutputVertexCoords(uint64_t x, uint64_t y) const noexcept
{
	return isOutputVertex(x + W * y);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::set(uint64_t location, bool on) noexcept
{
	board[location] = on;
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setList(const boost::python::list& list) noexcept
{
	auto length = boost::python::len(list);
	for (uint64_t i = 0uLL; i < length; ++i)
	{
		board[i] = boost::python::extract<bool>(list[i]);
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setCoords(uint64_t x, uint64_t y, bool on) noexcept
{
	return set(x + W * y, on);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setAll(bool on) noexcept
{
	if (on)
	{
		board.set();
	} else
	{
		board.reset();
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setRandom(double probability) noexcept
{
	std::bernoulli_distribution dist(probability);
	for (uint64_t i = 0uLL; i < W * H; ++i)
	{
		board[i] = dist(generator);
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setRandomSeed(uint64_t seed) noexcept
{
	generator.seed(seed);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setOutputVertex(uint64_t location, bool on) noexcept
{
	outputVertices[location] = on;
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setOutputVertexList(const boost::python::list& list) noexcept
{
	auto length = boost::python::len(list);
	for (uint64_t i = 0uLL; i < length; ++i)
	{
		outputVertices[i] = boost::python::extract<bool>(list[i]);
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setOutputVertexCoords(uint64_t x, uint64_t y, bool on) noexcept
{
	return setOutputVertex(x + W * y, on);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setAllOutputVertices(bool on) noexcept
{
	if (on)
	{
		outputVertices.set();
	} else
	{
		outputVertices.reset();
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::setRandomOutputVertices(double probability) noexcept
{
	std::bernoulli_distribution dist(probability);
	for (uint64_t i = 0uLL; i < W * H; ++i)
	{
		outputVertices[i] = dist(generator);
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::flip(uint64_t location) noexcept
{
	BOARD_TYPE board;
	board[location] = true;
	flipBitset(board);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::flipList(const boost::python::list& list) noexcept
{
	LightsOutBoard<W, H> board;
	board.setList(list);
	flipBoard(board);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::flipCoords(uint64_t x, uint64_t y) noexcept
{
	flip(x + W * y);
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::flipBitset(const BOARD_TYPE& otherBoard) noexcept
{
	board ^= otherBoard; // toggle centers
	board ^= (otherBoard << W); // toggle above
	board ^= (otherBoard >> W); // toggle below
	board ^= ((otherBoard & ~LEFT_EDGE_MASK) >> 1uLL); // toggle left if not on left edge
	board ^= ((otherBoard & ~RIGHT_EDGE_MASK) << 1uLL); // toggle right if not on right edge
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::flipBoard(const LightsOutBoard& lightsOutBoard) noexcept
{
	flipBitset(lightsOutBoard.board);
}

template <uint64_t W, uint64_t H>
uint64_t LightsOutBoard<W, H>::getNumOn() const noexcept
{
	return (board & ~outputVertices).count();
}

template <uint64_t W, uint64_t H>
typename LightsOutBoard<W, H>::BOARD_TYPE LightsOutBoard<W, H>::getLeftEdgeMask() noexcept {
	typename LightsOutBoard<W, H>::BOARD_TYPE leftEdgeMask;

	for (uint64_t location = 0uLL; location < leftEdgeMask.size(); location += W)
	{
		leftEdgeMask.set(location);
	}

	return leftEdgeMask;
}

template <uint64_t W, uint64_t H>
typename LightsOutBoard<W, H>::BOARD_TYPE LightsOutBoard<W, H>::getRightEdgeMask() noexcept {
	return getLeftEdgeMask() << (W - 1uLL);
}

template <uint64_t W, uint64_t H>
std::string LightsOutBoard<W, H>::pretty() const noexcept {
	std::stringstream out;
	for (uint64_t y = 0uLL; y < H; ++y)
	{
		for (uint64_t x = 0uLL; x < W; ++x)
		{
			char symbol = isOutputVertexCoords(x, y) ? (isOnCoords(x, y) ? 'o' : 'x') : (isOnCoords(x, y) ? 'O' : 'X');
			out << ' ' << symbol;
		}

		out << '\n';
	}
	return out.str();
}


template <uint64_t W, uint64_t H>
std::ostream& LightsOutBoard<W, H>::print(std::ostream& out) const noexcept {
	out << board.to_string();
	return out;
}

template <uint64_t W, uint64_t H>
std::ostream& operator<<(std::ostream& out, const LightsOutBoard<W, H>& board)
{
	return board.print(out);
}

}

// ------------------------ 
// Linear Algebra Method
template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::buildMatrix()
{
	std::vector<BOARD_TYPE> v (W*H, BOARD_TYPE{});
	for (uint64_t i = 0; i < v.size(); ++i)
	{
		BOARD_TYPE otherBoard;
		otherBoard[i] = true;
		v[i] ^= otherBoard; // toggle centers
		v[i] ^= (otherBoard << W); // toggle above
		v[i] ^= (otherBoard >> W); // toggle below
		v[i] ^= ((otherBoard & ~LEFT_EDGE_MASK) >> 1uLL); // toggle left if not on left edge
		v[i] ^= ((otherBoard & ~RIGHT_EDGE_MASK) << 1uLL);
		
	}
	boardMatrix = v;
}

template <uint64_t W, uint64_t H>
uint64_t LightsOutBoard<W, H>::findPivot(uint64_t startRow, uint64_t pivotCol)
{
	for (uint64_t row = startRow; row < W * H; ++row)
      if (boardMatrix[row][pivotCol])
        return row;

    return  W * H;
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::rowReduce()
{
	buildMatrix();
	std::vector<BOARD_TYPE> identity (W*H, BOARD_TYPE{});
	for (uint64_t i = 0; i < identity.size(); ++i)
	{
		identity[i][i] = true;
	}
	uint64_t nextFreeRow = 0;
	for (uint64_t col = 0; col < W*H; ++col)
	{
		uint64_t pivotRow = findPivot(nextFreeRow, col);
		if (pivotRow == W * H) continue;

		// perform swapping of (boardMatrix[pivotRow], boardMatrix[nextFreeRow]), (identity[pivotRow], identity[nextFreeRow]);
		if (pivotRow != nextFreeRow)
		{
			boardMatrix[pivotRow] ^= boardMatrix[nextFreeRow];
			boardMatrix[nextFreeRow] ^= boardMatrix[pivotRow];
			boardMatrix[pivotRow] ^= boardMatrix[nextFreeRow];

			identity[pivotRow] ^= identity[nextFreeRow];
			identity[nextFreeRow] ^= identity[pivotRow];
			identity[pivotRow] ^= identity[nextFreeRow];
		}
		



		for (uint64_t row = pivotRow + 1; row < W*H; ++row)
		{
			if (boardMatrix[row][col])
			{
				boardMatrix[row] ^= boardMatrix[nextFreeRow];
				identity[row] ^= identity[nextFreeRow]; 
			}
		}
		++nextFreeRow;
	}

	// reduce more, get null_space
	int64_t pivotMark = W * H - 1;
	int64_t k = W * H - pivotMark;
	std::vector<int64_t> freeCol;

	for (int64_t row = W * H - 1; row >= 0; --row)
	{
		if (boardMatrix[row].none()) continue;
		if ((boardMatrix[row] << k).any())
		{
			while((boardMatrix[row] << k).any())
			{
				freeCol.push_back(int64_t(W*H - k));
				++k;
				
			}
		} 
		pivotMark = W*H - k;
		++k;
		
		for (int64_t moverow = row - 1; moverow >=0; --moverow)
		{
			if (boardMatrix[moverow][pivotMark])
				{
					boardMatrix[moverow] ^= boardMatrix[row];
					identity[moverow] ^= identity[row]; 
				}
		}
	}
	invMatrix = identity;
	nullSpace = std::vector<BOARD_TYPE>();
	for (int64_t i: freeCol)
	{
		nullSpace.push_back(invMatrix[i]);
	}
}

template <uint64_t W, uint64_t H>
typename LightsOutBoard<W, H>::BOARD_TYPE 
LightsOutBoard<W, H>::matrixMulti(std::vector<LightsOutBoard<W, H>::BOARD_TYPE> inv, const BOARD_TYPE& b)
{
	BOARD_TYPE res;
	for (uint64_t i = 0; i < W*H; ++i)
	{
		BOARD_TYPE s = inv[i] & b;
		res[i] = s.count() % 2;
	}
	return res;
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::solveLinAlg(const BOARD_TYPE& b)
{
	if (nullSpace.size() == 0)
	{
		board = matrixMulti(invMatrix, b);
	}
	else
	{

		// check if there is perfect solution
		bool check = false;
		for (uint64_t i = 0; i < nullSpace.size(); i++)
		{
			BOARD_TYPE s = nullSpace[i] & b;
			if (s.count() % 2)
			{
				std::cout << "no perfect solution" << std::endl;
				break;
			}
		}
		// -----------------------------------
		// particular solution
		BOARD_TYPE parSol;
		parSol = matrixMulti(invMatrix, b);
		auto len = nullSpace.size();
		for (int i = 0; i < pow(2, len); ++i)
		{
			std::bitset<20> bs (i);
			BOARD_TYPE genSol;
			for (int j = 0; j < len; j++)
			{
				if(bs[j])
				{
					genSol ^= nullSpace[j];
				}
			}
			genSol ^= parSol;
			if (parSol.count() > genSol.count()) parSol = genSol;
		}
		board = parSol;
	}
}

template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::solveLinAlg()
{
	solution(board);
}


template <uint64_t W, uint64_t H>
void LightsOutBoard<W, H>::solveLinAlgOther(const LightsOutBoard& other)
{
	solution(other.board);
}

template <uint64_t W, uint64_t H>
uint64_t LightsOutBoard<W, H>::numberOfNullSpace()
{
	return nullSpace.size();
}

//Linear Algebra Method
// ------------------------------------

#endif
