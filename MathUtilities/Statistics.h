#pragma once
#include <assert.h>
#include <math.h>
#include <vector>
#include <ctime>

template<class T> inline double Variance(T in[], int N)
{
	double mean = 0;
	double var = 0;
	for (int i = 0; i < N; i++)
	{
		mean += in[i];
		var += in[i] * in[i];
	}
	mean /= N;
	var /= N;
	return var - mean * mean;
}

// calculate the variance of a partial sequence within certain distance from the center point
template<class _RanIt> inline double PartialVariance(_RanIt first, _RanIt last,
	_RanIt current, int radius)
{
	assert(first < last);
	double mean = 0;
	double var = 0;
	int count = 0;

	// totally 2 * radius + 1 elements are involved
	int _clamped_foffset, _clamped_boffset;
	_clamped_foffset = first - current < -radius ? -radius : first - current;
	_clamped_boffset = last - current - 1 > radius ? radius : last - current - 1;

	for (int i = _clamped_foffset; i <= _clamped_boffset; i++)
	{
		_RanIt itpar = current + i;
		mean += *itpar;
		var += (*itpar) * (*itpar);
		count++;
	}
	mean /= count;
	var /= count;
	return var - mean * mean;
}

// returns a random number in [0, 1]
inline double RandDouble(void)
{
	static bool didSeeding = false;

	if (!didSeeding) 
	{
		srand((unsigned)time(NULL));
		didSeeding = true;
	}
	return rand() / (RAND_MAX + 1.0);
}

// generate a random permutation of length "inNum"
inline void RandPerm(const int &inNum, std::vector<int> &outVect)
{
	outVect.resize(inNum);
	int randIndex, tempIndex;
	for (int nFeat = 0; nFeat < inNum; nFeat++)
	{
		outVect[nFeat] = nFeat;
	}
	for (register int nFeat = 0; nFeat < inNum; nFeat++) 
	{
		randIndex = (int) floor(((double) inNum - nFeat) * RandDouble()) + nFeat;
		if (randIndex == inNum) 
		{
			randIndex--;
		}
		tempIndex = outVect[nFeat];
		outVect[nFeat] = outVect[randIndex];
		outVect[randIndex] = tempIndex;
	}
}
