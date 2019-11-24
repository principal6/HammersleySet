#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::make_pair;

pair<float, float> Hammersley10(uint32_t Seed)
{
	const uint32_t KOrder{ 10 };
	const float KBase{ pow(2.0f, -(float)KOrder) };

	uint32_t InvertedBits
	{
		(Seed & 0x200) >> (KOrder - 1) | (Seed & 0x001) << (KOrder - 1) |
		(Seed & 0x100) >> (KOrder - 3) | (Seed & 0x002) << (KOrder - 3) |
		(Seed & 0x080) >> (KOrder - 5) | (Seed & 0x004) << (KOrder - 5) |
		(Seed & 0x040) >> (KOrder - 7) | (Seed & 0x008) << (KOrder - 7) |
		(Seed & 0x020) >> (KOrder - 9) | (Seed & 0x010) << (KOrder - 9)
	};

	float X{ KBase * (float)Seed };
	float Y{ KBase * (float)InvertedBits };

	return make_pair(X, Y);
}

pair<float, float> HammersleyReference(uint32_t Seed, uint32_t SampleCount)
{
	uint32_t InvertedBits{};
	InvertedBits = (Seed << 16) | (Seed >> 16);
	InvertedBits = (InvertedBits & 0x55555555) << 1 | (InvertedBits & 0xAAAAAAAA) >> 1;
	InvertedBits = (InvertedBits & 0x33333333) << 2 | (InvertedBits & 0xCCCCCCCC) >> 2;
	InvertedBits = (InvertedBits & 0x0F0F0F0F) << 4 | (InvertedBits & 0xF0F0F0F0) >> 4;
	InvertedBits = (InvertedBits & 0x00FF00FF) << 8 | (InvertedBits & 0xFF00FF00) >> 8;

	float X{ (float)Seed / (float)SampleCount };
	float Y{ (float)InvertedBits * 2.3283064365386963e-10f };

	return make_pair(X, Y);
}

uint32_t GetHammersleyOrder(uint32_t SampleCount)
{
	return (uint32_t)log2((float)(SampleCount - 1)) + 1;
}

float GetHammersleyBase(uint32_t Order)
{
	return pow(2.0f, -(float)Order);
}

pair<float, float> Hammersley(uint32_t Seed, uint32_t SampleCount, uint32_t Order, float Base)
{
	uint32_t InvertedBits{};
	uint32_t ShiftStep{ Order - 1 };
	while (true)
	{
		InvertedBits |=
			((Seed & 0x1 << ((Order / 2 + (ShiftStep + 1) / 2) - 1)) >> ShiftStep) |
			((Seed & 0x1 << (Order / 2 - (ShiftStep + 1) / 2)) << ShiftStep);

		if (ShiftStep <= 1) break;

		ShiftStep -= 2;
	}

	float X{ Base * (float)Seed };
	float Y{ Base * (float)InvertedBits };

	return make_pair(X, Y);
}

int main()
{
	// This is a 2D Hammersley set with order of 10
	vector<pair<float, float>> vHammersleySetReference{};
	vector<pair<float, float>> vHammersleySet{};
	
	// FOR EXAMPLE,
	// SEED(10)	= DEC(2)	= DEC(10)			Hammerseley
	// 0(10)	= 0.000(2)	= 0.0	(10)		0.000(2)
	// 1(10)	= 0.001(2)	= 0.125	(10)		0.100(2)
	// 2(10)	= 0.010(2)	= 0.25	(10)		0.010(2)
	// 3(10)	= 0.011(2)	= 0.375	(10)		0.110(2)
	// 4(10)	= 0.100(2)	= 0.5	(10)		0.001(2)
	// 5(10)	= 0.101(2)	= 0.625	(10)		0.101(2)
	// 6(10)	= 0.110(2)	= 0.75	(10)		0.011(2)
	// 7(10)	= 0.111(2)	= 0.875	(10)		0.111(2)

	/*
	for (uint32_t Seed = 0; Seed < 1024; ++Seed)
	{
		vHammersleySet.emplace_back(Hammersley10(Seed));
	}
	*/

	uint32_t SeedCount{ 128 };
	
	for (uint32_t Seed = 0; Seed < SeedCount; ++Seed)
	{
		vHammersleySetReference.emplace_back(HammersleyReference(Seed, SeedCount));
	}

	uint32_t Order{ GetHammersleyOrder(SeedCount) };
	float Base{ GetHammersleyBase(Order) };
	for (uint32_t Seed = 0; Seed < SeedCount; ++Seed)
	{
		vHammersleySet.emplace_back(Hammersley(Seed, SeedCount, Order, Base));
	}

	return 0;
}