#pragma once

#include <random>

class RandomNumberGenerator
{
public:
	RandomNumberGenerator(const float min, const float max): distribution(min, max)
	{
		std::random_device rd;
		twister = std::mt19937(rd());
	}

	float next() 
	{
		return distribution(twister);
	}

private:
	std::mt19937 twister;
	std::uniform_real_distribution<float> distribution;
};
