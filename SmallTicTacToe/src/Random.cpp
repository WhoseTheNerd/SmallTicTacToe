#include "Random.hpp"

namespace TicTacToe {

	int Random::Generate(int low, int high)
	{
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<int> dist(low, high);
		return dist(generator);
	}
}