#include <iostream>
#include <vector>

using std::vector;
using std::pair;
using std::make_pair;

pair<float, float> Hammersley10(uint32_t Seed)
{
	static constexpr uint32_t KOrder{ 10 };
	static const float KBase{ pow(2.0f, -(float)KOrder) };

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

int main()
{
	// This is a 2D Hammersley set with order of 10
	vector<pair<float, float>> vHammersleySet{};

	// Order of 10 = 2^10 = 1024 = 0x0400
	static const uint32_t KSeedCount{ (uint32_t)pow(2.0f, 10.0f) };

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
	
	for (uint32_t Seed = 0; Seed < KSeedCount; ++Seed)
	{
		vHammersleySet.emplace_back(Hammersley10(Seed));
	}

	return 0;
}