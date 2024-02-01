#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
#include "types.h"
#include "Xoodoo.h"
#define NCHECK 100
#define NPLAINTEXT /*4262*/10370
/*4202*//*10348*//*8192*//*16777216*//*32768*//*268435456*//*262144*//*16384*/
//#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
//#define ROTR(a,b) (((a) >> (b)) | ((a) << (32 - (b))))

uint32_t leftRotate(const uint32_t &n, uint32_t d)
{
	return (n << d) | (n >> (32 - d));
}

uint32_t rightRotate(const uint32_t &n, uint32_t d)
{
	return (n >> d) | (n << (32 - d));
}

void RXDifLin()
{
	vector<LaneValue> diffIn(0);
	diffIn.resize(12);
	diffIn[0] = 0x484ccc80; diffIn[1]  = 0x484cc800; diffIn[2]  = 0x484cc800;
	diffIn[3] = 0x3ab9821a; diffIn[4]  = 0x3ab9821a; diffIn[5]  = 0x3ab9821a;
	diffIn[6] = 0x37b6cde9; diffIn[7]  = 0x37b6cde9; diffIn[8]  = 0x37b6cde9;
	diffIn[9] = 0x45a3f0cb; diffIn[10] = 0x45a3f0cb; diffIn[11] = 0x45a3f0cb;

	vector<LaneValue> diffIn5r(0);
	diffIn5r.resize(12);
	diffIn5r[0] = 0x7473334;  diffIn5r[1]  = 0x7473340;  diffIn5r[2]  = 0x7473340;
	diffIn5r[3] = 0x242b6751; diffIn5r[4]  = 0x242b6751; diffIn5r[5]  = 0x242b6751;
	diffIn5r[6] = 0x5cb8a32e; diffIn5r[7]  = 0x5cb8a32e; diffIn5r[8]  = 0x5cb8a32e;
	diffIn5r[9] = 0xbfdff2e5; diffIn5r[10] = 0xbfdff2e5; diffIn5r[11] = 0xbfdff2e5;
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	int fixedBits[202] = { 1,3,4,6,7,10,12,13,15,21,
		24,26,32,42,43,45,46,49,51,52,
		54,55,58,60,61,63,64,66,67,69,
		72,73,74,75,76,77,78,81,82,83,
		84,85,86,87,89,92,93,95,97,98,
		99,100,101,102,108,110,111,113,117,119,
		120,121,122,123,124,129,131,132,134,138,
		140,141,143,145,148,149,152,154,155,157,
		160,170,171,173,174,179,180,182,182,188,
		189,191,192,193,194,195,196,197,200,201,
		202,203,204,205,206,209,210,211,212,213,
		214,215,216,217,219,220,221,223,226,227,
		229,230,231,234,236,238,239,241,245,247,
		248,249,250,252,257,259,260,262,266,268,
		269,271,277,280,283,284,287,288,294,297,
		298,299,301,302,303,306,307,308,310,311,
		316,317,319,320,322,323,325,328,329,330,
		331,332,333,334,337,338,339,340,341,342,
		343,345,348,349,351,354,355,357,358,364,
		365,366,367,368,369,373,374,375,376,377,
		378,380 };

	int correlation[384] = {};
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	vector<LaneValue> l(0);
	XoodooParameters p(2, 8, 5, 14, 11);
	const Xoodoo temp(4, 32, p);
	XoodooState tempState(temp);
	l.resize(12);
	//l[0] = rc[7] << 1;
	//tempState.lanes = l;
	//tempState.getLanes()[0] ^= rc[7];
	//temp.inverseRhoWest(tempState);
	//temp.inverseTheta(tempState);
	//for (int f = 0; f < 12; f++)
	//	printf("%lx\n", tempState.getLanes()[f]);
	for (int testInd = 0; testInd < NCHECK; testInd++)
	{
		const Xoodoo aInstance(4, 32, p);
		const Xoodoo bInstance(4, 32, p);
		for (int j = 0; j < 12; j++)
			l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
		XoodooState astate(aInstance);
		astate.lanes = l;
		for (int fix = 0; fix < 202; fix++)
			astate.setBit(fixedBits[fix], 0);
		for (int j = 0; j < 12; j++)
			l[j] = (leftRotate(l[j], 1)) ^ (diffIn5r[j]);
		XoodooState bstate(bInstance);
		bstate.lanes = l;

		//aInstance.round(astate, rc[7]);
		//bInstance.round(bstate, rc[7]);
		//aInstance.theta(astate);
		//bInstance.theta(bstate);
		//aInstance.rhoWest(astate);
		//bInstance.rhoWest(bstate);
		//astate.getLanes()[0] ^= rc[8];
		//bstate.getLanes()[0] ^= rc[8];
		//for (int lna = 0; lna < 12; lna++)
		//	printf("%lx\n", ((leftRotate(astate.getLanes()[lna], 1)) ^ bstate.getLanes()[lna]));
		//printf("\n");

		aInstance.permute(astate, 5);
		bInstance.permute(bstate, 5);
		for (int maskInd = 0; maskInd < 384; maskInd++)
		{
			int index = maskInd;
			unsigned int z = index % 32;
			index /= 32;
			unsigned int x = index % 4;
			index /= 4;
			unsigned int y = index;
			LaneValue aaa = astate.getLane(x, y);
			LaneValue bbb = bstate.getLane(x, y);
			uint32_t mask = ((uint32_t)1 << maskInd);
			if (mask & ((leftRotate(aaa, 1)) ^ bbb))
				correlation[maskInd]++;
		}
		//LaneValue aaa = astate.getLane(3, 2);
		//LaneValue bbb = bstate.getLane(3, 2);
		//bool cor = 0x1000000 & ((leftRotate(aaa, 1)) ^ bbb);
		//p3 += cor;
		//aaa = astate.getLane(1, 0);
		//bbb = bstate.getLane(1, 0);
		//cor = 0x10000 & ((leftRotate(aaa, 1)) ^ bbb);
		//p1 += cor;
		//aaa = astate.getLane(1, 1);
		//bbb = bstate.getLane(1, 1);
		//cor = 0x20000 & ((leftRotate(aaa, 1)) ^ bbb);
		//p2 += cor;
	}
	//cout << "high:" << endl;
	//for (int rotInd = 0; rotInd < 384; rotInd++)
	//{
	//	for (int bitInd = 0; bitInd < 384; bitInd++)
	//	{
	//		if (correlation[rotInd][bitInd] > NCHECK - 10)
	//			printf("%d\t%d\n", rotInd, bitInd);
	//	}
	//}
	//cout << "low:" << endl;
	//for (int rotInd = 0; rotInd < 384; rotInd++)
	//{
	//	for (int bitInd = 0; bitInd < 384; bitInd++)
	//	{
	//		if (correlation[rotInd][bitInd] < NCHECK - 90)
	//			printf("%d\t%d\n", rotInd, bitInd);
	//	}
	//}
	int maxi = 0;
	int mini = 100;
	for (int i = 0; i < 384; i++)
	{
		if (correlation[i] > maxi)
			maxi = correlation[i];
		if (correlation[i] < mini)
			mini = correlation[i];
	}
	int jshd = 0;
}

void keyPerFixed4R2Goods(bool keyOptions[524288], uint64_t fixedValue)
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	XoodooParameters p(2, 8, 5, 14, 11);
	
	//int fixedBits1[31] = { 7, 11, 18, 32, 93, 114, 125,
	//	128, 135, 144, 146, 155, 198, 221, 230, 235, 242, 251, 253,
	//	256, 263, 272, 274, 283, 309, 326, 349, 358, 370, 379, 381 };
	//int fixedBits2[31] = { 4, 20, 25, 41, 79, 100, 111,
	//	132, 137, 144, 153, 155, 198, 207, 228, 230, 239, 244, 251,
	//	260, 265, 272, 281, 283, 318, 326, 335, 356, 358, 367, 379 };
	int fixedBits[57] = { 4, 7, 11, 16, 18, 20, 25, 27, 32, 41, 70, 79, 93, 100, 102, 111, 114, 123, 125,
		128, 132, 135, 137, 144, 146, 153, 155, 198, 207, 221, 228, 230, 235, 239, 242, 244, 251, 253,
		256, 260, 263, 265, 272, 274, 281, 283, 309, 318, 326, 335, 349, 356, 358, 367, 370, 379, 381 };
	//int commonKeysBits[5] = { 16,27,70,102,123 };
	//for (int k = 0; k < 4096; k++)
	//	key1Options[k] = true;
	uint32_t afterChi1[4][3] = {};
	afterChi1[0][2] = 0x800;
	afterChi1[1][1] = 1;
	uint32_t afterChi2[4][3] = {};
	afterChi2[0][2] = 1048576;;
	afterChi2[1][1] = 512;
	int goodInd1 = 0;
	int goodInd2 = 0;

	for (int keyOption = 0; keyOption < 524288; keyOption++)
	{
		vector<LaneValue> l1(12);
		Xoodoo temp(4, 32, p);
		XoodooState tempS(temp);
		tempS.lanes = l1;
		for (int j = 0; j < 19; j++)
			tempS.setBit(fixedBits[j], keyOption&((uint32_t)1 << j));
		for (int j = 19; j < 57; j++)
			tempS.setBit(fixedBits[j], fixedValue&((uint64_t)1 << (j - 19)));

		LaneValue diff1[12] = {};
		diff1[1] = 1;
		diff1[2] = 1;
		Xoodoo temp2(4, 32, p);
		XoodooState tempS2(temp2);
		tempS2.lanes = tempS.getLanes();
		for (int j = 0; j < 12; j++)
			tempS2.getLanes()[j] ^= diff1[j];

		LaneValue diff2[12] = {};
		diff2[1] = 512;
		diff2[2] = 512;
		Xoodoo temp3(4, 32, p);
		XoodooState tempS3(temp3);
		tempS3.lanes = tempS.getLanes();
		for (int j = 0; j < 12; j++)
			tempS3.getLanes()[j] ^= diff2[j];

		temp.theta(tempS);
		temp.rhoWest(tempS);
		tempS.getLanes()[0] ^= rc[8];
		temp.chi(tempS);

		temp2.theta(tempS2);
		temp2.rhoWest(tempS2);
		tempS2.getLanes()[0] ^= rc[8];
		temp2.chi(tempS2);

		temp3.theta(tempS3);
		temp3.rhoWest(tempS3);
		tempS3.getLanes()[0] ^= rc[8];
		temp3.chi(tempS3);

		bool isGood = true;
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 4; x++)
				if (((tempS.getLane(x, y) ^ tempS2.getLane(x, y)) != afterChi1[x][y])
					|| ((tempS.getLane(x, y) ^ tempS3.getLane(x, y)) != afterChi2[x][y]))
					isGood = false;
		keyOptions[keyOption] = isGood;
	}
}

void keyPerFixed4RfrstGood(bool keyOptions[4096], uint64_t fixedValue)
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	XoodooParameters p(2, 8, 5, 14, 11);
	int fixedBits1[12] = { 7, 11, 16, 18, 27, 32, 70, 93, 102, 114, 123, 125 };
	int fixedBits[57] = { 4, 7, 11, 16, 18, 20, 25, 27, 32, 41, 70, 79, 93, 100, 102, 111, 114, 123, 125,
		128, 132, 135, 137, 144, 146, 153, 155, 198, 207, 221, 228, 230, 235, 239, 242, 244, 251, 253,
		256, 260, 263, 265, 272, 274, 281, 283, 309, 318, 326, 335, 349, 356, 358, 367, 370, 379, 381 };

	uint32_t afterChi1[4][3] = {};
	afterChi1[0][2] = 0x800;
	afterChi1[1][1] = 1;

	for (int keyOption = 0; keyOption < 4096; keyOption++)
	{
		vector<LaneValue> l1(12);
		Xoodoo temp(4, 32, p);
		XoodooState tempS(temp);
		tempS.lanes = l1;
		for (int j = 0; j < 12; j++)
			tempS.setBit(fixedBits1[j], keyOption&((uint32_t)1 << j));
		for (int j = 19; j < 57; j++)
			tempS.setBit(fixedBits[j], fixedValue&((uint64_t)1 << (j - 19)));

		LaneValue diff1[12] = {};
		diff1[1] = 1;
		diff1[2] = 1;
		Xoodoo temp2(4, 32, p);
		XoodooState tempS2(temp2);
		tempS2.lanes = tempS.getLanes();
		for (int j = 0; j < 12; j++)
			tempS2.getLanes()[j] ^= diff1[j];

		temp.theta(tempS);
		temp.rhoWest(tempS);
		tempS.getLanes()[0] ^= rc[8];
		temp.chi(tempS);

		temp2.theta(tempS2);
		temp2.rhoWest(tempS2);
		tempS2.getLanes()[0] ^= rc[8];
		temp2.chi(tempS2);

		bool isGood = true;
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 4; x++)
				if (((tempS.getLane(x, y) ^ tempS2.getLane(x, y)) != afterChi1[x][y]))
					isGood = false;
		keyOptions[keyOption] = isGood;
	}
}

void keyPerFixed4RscndGood(bool keyOptions[4096], uint64_t fixedValue)
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	XoodooParameters p(2, 8, 5, 14, 11);
	int fixedBits2[12] = { 4, 16, 20, 25, 27, 41, 70, 79, 100, 102, 111, 123 };
	int fixedBits[57] = { 4, 7, 11, 16, 18, 20, 25, 27, 32, 41, 70, 79, 93, 100, 102, 111, 114, 123, 125,
		128, 132, 135, 137, 144, 146, 153, 155, 198, 207, 221, 228, 230, 235, 239, 242, 244, 251, 253,
		256, 260, 263, 265, 272, 274, 281, 283, 309, 318, 326, 335, 349, 356, 358, 367, 370, 379, 381 };
	
	uint32_t afterChi2[4][3] = {};
	afterChi2[0][2] = 1048576;;
	afterChi2[1][1] = 512;

	for (int keyOption = 0; keyOption < 4096; keyOption++)
	{
		vector<LaneValue> l1(12);
		Xoodoo temp(4, 32, p);
		XoodooState tempS(temp);
		tempS.lanes = l1;
		for (int j = 0; j < 12; j++)
			tempS.setBit(fixedBits2[j], keyOption&((uint32_t)1 << j));
		for (int j = 19; j < 57; j++)
			tempS.setBit(fixedBits[j], fixedValue&((uint64_t)1 << (j - 19)));

		LaneValue diff2[12] = {};
		diff2[1] = 512;
		diff2[2] = 512;
		Xoodoo temp3(4, 32, p);
		XoodooState tempS3(temp3);
		tempS3.lanes = tempS.getLanes();
		for (int j = 0; j < 12; j++)
			tempS3.getLanes()[j] ^= diff2[j];

		temp.theta(tempS);
		temp.rhoWest(tempS);
		tempS.getLanes()[0] ^= rc[8];
		temp.chi(tempS);

		temp3.theta(tempS3);
		temp3.rhoWest(tempS3);
		tempS3.getLanes()[0] ^= rc[8];
		temp3.chi(tempS3);

		bool isGood = true;
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 4; x++)
				if (((tempS.getLane(x, y) ^ tempS3.getLane(x, y)) != afterChi2[x][y]))
					isGood = false;
		keyOptions[keyOption] = isGood;
	}
}

void setGoodFix(uint32_t goodFix[16384])
{
	for (int i = 0; i < 16384; i++)
		goodFix[i] = true;
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	vector<LaneValue> diff(12);
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;

	uint32_t afterD[4][3] = {};
	afterD[0][0] = 1;
	afterD[2][2] = 0x1000000;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
		219,224,231,240,266,275,277,298,307,338,347,359,368 };

	//ofstream file;
	//file.open("fixs.txt");
	vector<LaneValue> l1(12);
	XoodooParameters p(2, 8, 5, 14, 11);
	Xoodoo temp(4, 32, p);
	XoodooState tempS(temp);
	tempS.lanes = l1;
	uint32_t goodInd = 0;
	for (int i = 0; i < 262144; i++)
	{
		for (int j = 0; j < 28; j++)
			tempS.setBit(fixedBits[j], i&((uint32_t)1 << j));
		Xoodoo temp2(4, 32, p);
		XoodooState tempS2(temp2);
		tempS2.lanes = diff;
		for (int j = 0; j < 12; j++)
			tempS2.getLanes()[j] ^= tempS.getLanes()[j];
		temp.theta(tempS);
		temp.rhoWest(tempS);
		tempS.getLanes()[0] ^= rc[7];
		temp.chi(tempS);
		temp2.theta(tempS2);
		temp2.rhoWest(tempS2);
		tempS2.getLanes()[0] ^= rc[7];
		temp2.chi(tempS2);
		uint32_t afterR[4][3] = {};
		bool isGood = true;
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 4; x++)
				if ((tempS.getLane(x, y) ^ tempS2.getLane(x, y)) != afterD[x][y])
					isGood = false;
		if (isGood)
		{
			goodFix[goodInd] = i;
			goodInd++;
		}
	}
}

void setGoodFix(int goodFix1[512][64], int goodFix2[512][64])
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	vector<LaneValue> diff1(12);
	diff1[0] = 0xa8b23b19;
	diff1[1] = 0xa8b23b18;
	diff1[2] = 0xa8b23b18;
	diff1[3] = 0x98810919;
	diff1[4] = 0x98810919;
	diff1[5] = 0x98810919;
	diff1[6] = 0x52674513;
	diff1[7] = 0x52674513;
	diff1[8] = 0x52676513;
	diff1[9] = 0x95a876f3;
	diff1[10] = 0x95a876f3;
	diff1[11] = 0x95a876f3;
	vector<LaneValue> diff2(12);
	diff2[0] = 0xa0bb90a8;
	diff2[1] = 0xa0bb90a9;
	diff2[2] = 0xa09b90a9;
	diff2[3] = 0xf7583d32;
	diff2[4] = 0xf6583d32;
	diff2[5] = 0xf7583d32;
	diff2[6] = 0xc54b1bc8;
	diff2[7] = 0xc44b1bc8;
	diff2[8] = 0xc44b3bc8;
	diff2[9] = 0x6795484a;
	diff2[10] = 0x6795484a;
	diff2[11] = 0x6795484a;

	uint32_t afterD1[4][3] = {};
	afterD1[0][0] = 1;
	afterD1[2][2] = 0x1000000;
	//uint32_t afterD2[4][3] = {};
	//afterD2[0][0] = 1;
	//afterD2[0][2] = 1;
	//afterD2[2][0] = 0x1000000;
	//afterD2[2][1] = 0x1000000;
	//afterD2[2][2] = 0x1000000;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
		219,224,231,240,266,275,277,298,307,338,347,359,368 };

	vector<LaneValue> l1(12);
	for (int i = 0; i < 12; i++)
		l1[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
	XoodooParameters p(2, 8, 5, 14, 11);
	Xoodoo temp(4, 32, p);
	XoodooState tempS(temp);
	tempS.lanes = l1;

	for (uint32_t pre = 0; pre < 512; pre++)
	{
		uint32_t goodInd1 = 0;
		uint32_t goodInd2 = 0;
		for (uint32_t suf = 0; suf < 512; suf++)
		{
			uint32_t fixVal = pre | (suf << 9);
			for (int j = 0; j < 28; j++)
				tempS.setBit(fixedBits[j], fixVal & ((uint32_t)1 << j));

			Xoodoo temp2(4, 32, p);
			XoodooState tempS2(temp2);
			tempS2.lanes = diff1;
			for (int j = 0; j < 12; j++)
				tempS2.getLanes()[j] ^= tempS.getLanes()[j];

			Xoodoo temp3(4, 32, p);
			XoodooState tempS3(temp3);
			tempS3.lanes = diff2;
			for (int j = 0; j < 12; j++)
				tempS3.getLanes()[j] ^= tempS.getLanes()[j];

			temp.theta(tempS);
			temp.rhoWest(tempS);
			tempS.getLanes()[0] ^= rc[7];

			temp2.theta(tempS2);
			temp2.rhoWest(tempS2);
			tempS2.getLanes()[0] ^= rc[7];

			temp3.theta(tempS3);
			temp3.rhoWest(tempS3);
			tempS3.getLanes()[0] ^= rc[7];

			temp.chi(tempS);
			temp2.chi(tempS2);
			temp3.chi(tempS3);

			uint32_t afterR[4][3] = {};
			bool isGood1 = true;
			bool isGood2 = true;
			for (int y = 0; y < 3; y++)
				for (int x = 0; x < 4; x++)
				{
					if ((tempS.getLane(x, y) ^ tempS2.getLane(x, y)) != afterD1[x][y])
						isGood1 = false;
					if ((tempS.getLane(x, y) ^ tempS3.getLane(x, y)) != afterD1[x][y])
						isGood2 = false;
				}
			if (isGood1)
			{
				goodFix1[pre][goodInd1] = suf;
				goodInd1++;
			}
			if (isGood2)
			{
				//for (int y = 0; y < 3; y++)
				//{
				//	for (int x = 0; x < 4; x++)
				//		printf("%lx\t", (tempS3.getLane(x, y) ^ tempS.getLane(x, y)));
				//	printf("\n");
				//}
				//printf("\n");
				goodFix2[pre][goodInd2] = suf;
				goodInd2++;
			}
		}
	}
}

void setGoodFix(uint32_t goodFix[512][32], bool relatedFix[512][512])
{
	//ofstream file;
	//file.open("goodFixes.txt");
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 512; j++)
			relatedFix[i][j] = false;
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	//diffLin221Fixed(4);
	//diffLin2215r();

	vector<LaneValue> diff(12);
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;

	uint32_t afterD[4][3] = {};
	afterD[0][0] = 1;
	afterD[2][2] = 0x1000000;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
		219,224,231,240,266,275,277,298,307,338,347,359,368 };

	//ofstream file;
	//file.open("fixs.txt");
	vector<LaneValue> l1(12);
	XoodooParameters p(2, 8, 5, 14, 11);
	Xoodoo temp(4, 32, p);
	XoodooState tempS(temp);
	tempS.lanes = l1;
	
	//bool isUsed[512] = {};
	for (uint32_t pre = 0; pre < 512; pre++)
	{
		uint32_t goodInd = 0;
		for (uint32_t suf = 0; suf < 512; suf++)
		{
			//if (isUsed[suf])
			//	continue;
			uint32_t fixVal = pre | (suf << 9);
			for (int j = 0; j < 28; j++)
				tempS.setBit(fixedBits[j], fixVal & ((uint32_t)1 << j));
			Xoodoo temp2(4, 32, p);
			XoodooState tempS2(temp2);
			tempS2.lanes = diff;
			for (int j = 0; j < 12; j++)
				tempS2.getLanes()[j] ^= tempS.getLanes()[j];
			temp.theta(tempS);
			temp.rhoWest(tempS);
			tempS.getLanes()[0] ^= rc[7];
			temp.chi(tempS);
			temp2.theta(tempS2);
			temp2.rhoWest(tempS2);
			tempS2.getLanes()[0] ^= rc[7];
			temp2.chi(tempS2);
			uint32_t afterR[4][3] = {};
			bool isGood = true;
			for (int y = 0; y < 3; y++)
				for (int x = 0; x < 4; x++)
					if ((tempS.getLane(x, y) ^ tempS2.getLane(x, y)) != afterD[x][y])
						isGood = false;
			if (isGood)
			{
				//file << std::hex << pre << "\t" << suf << endl;
				goodFix[pre][goodInd] = suf;
				relatedFix[suf][pre] = true;
				goodInd++;
				//isUsed[suf] = true;
				//break;
			}
		}
	}
	//file.close();
}

bool ArrCompare(uint32_t arr1[32], uint32_t arr2[32])
{
	bool eq = true;
	for (int i = 0; i < 32; i++)
	{
		if (arr1[i] != arr2[i])
		{
			eq = false;
			break;
		}
	}
	return eq;
}

//void clasicalAttack()
//{
//	ofstream file;
//	file.open("suc.txt");
//	vector<LaneValue> diff(12);
//	diff[0] = 0xa8b23b19;
//	diff[1] = 0xa8b23b18;
//	diff[2] = 0xa8b23b18;
//	diff[3] = 0x98810919;
//	diff[4] = 0x98810919;
//	diff[5] = 0x98810919;
//	diff[6] = 0x52674513;
//	diff[7] = 0x52674513;
//	diff[8] = 0x52676513;
//	diff[9] = 0x95a876f3;
//	diff[10] = 0x95a876f3;
//	diff[11] = 0x95a876f3;
//
//	int suc = 0;
//
//	for (int testInd = 0; testInd < 100; testInd++)
//	{
//		vector<LaneValue> plnA(12);
//		vector<LaneValue> plnB(12);
//		for (int maskBit1 = 0; maskBit1 < 4; maskBit1++)
//			plnA[3 * maskBit1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//		bool keyRelevantBits[9] = {};
//		keyRelevantBits[0] = (plnA[0] & ((uint32_t)1 << (fixedBits[0] % 32)));
//		keyRelevantBits[1] = (plnA[0] & ((uint32_t)1 << (fixedBits[1] % 32)));
//		keyRelevantBits[2] = (plnA[3] & ((uint32_t)1 << (fixedBits[2] % 32)));
//		keyRelevantBits[3] = (plnA[3] & ((uint32_t)1 << (fixedBits[3] % 32)));
//		keyRelevantBits[4] = (plnA[6] & ((uint32_t)1 << (fixedBits[4] % 32)));
//		keyRelevantBits[5] = (plnA[6] & ((uint32_t)1 << (fixedBits[5] % 32)));
//		keyRelevantBits[6] = (plnA[6] & ((uint32_t)1 << (fixedBits[6] % 32)));
//		keyRelevantBits[7] = (plnA[9] & ((uint32_t)1 << (fixedBits[7] % 32)));
//		keyRelevantBits[8] = (plnA[9] & ((uint32_t)1 << (fixedBits[8] % 32)));
//		uint32_t theKey = 0;
//		//uint32_t keyOption;
//		//bool keyOptions[512];
//		//for (int k = 0; k < 512; k++)
//		//	keyOptions[k] = true;
//		for (int maskBit1 = 0; maskBit1 < 9; maskBit1++)
//			theKey |= ((uint32_t)keyRelevantBits[maskBit1] << maskBit1);
//		uint32_t sum[512] = {};
//		for (uint32_t suffix = 0; suffix < 512; suffix++)
//		{
//			uint32_t fix = (suffix << 9);
//			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
//			{
//				for (int maskBit1 = 0; maskBit1 < 4; maskBit1++)
//				{
//					plnA[3 * maskBit1 + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//					plnA[3 * maskBit1 + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//				}
//
//				XoodooParameters p(2, 8, 5, 14, 11);
//				const Xoodoo aInstance(4, 32, p);
//				XoodooState aState(aInstance);
//				aState.lanes = plnA;
//				for (int maskBit1 = 9; maskBit1 < 28; maskBit1++)
//					aState.setBit(fixedBits[maskBit1], fix & ((uint32_t)1 << maskBit1));
//				for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
//					plnB[maskBit1] = aState.getLanes()[maskBit1] ^ diff[maskBit1];
//				const Xoodoo bInstance(4, 32, p);
//				XoodooState bState(bInstance);
//				bState.lanes = plnB;
//				aInstance.permute(aState, 5);
//				bInstance.permute(bState, 5);
//				if (aState.getBit(0) == bState.getBit(0))
//					sum[suffix]++;
//			}
//		}
//		for (int suffix = 0; suffix < 512; suffix++)
//		{
//			sum[suffix] <<= 9;
//			sum[suffix] |= suffix;
//		}
//		std::sort(sum, sum + (sizeof(sum) / sizeof(sum[0])));
//		//for (int suffix = 0; suffix < 512; suffix++)
//		//	file << sum[suffix] << endl;
//		//int g = 0;
//		//for (int maskBit1 = 0; maskBit1 < 32; maskBit1++)
//		//{
//		//	uint32_t aGood = (sum[maskBit1] % 512);
//		//	for (int j = 0; j < 512; j++)
//		//		if (!relatedFix[aGood][j])
//		//			keyOptions[j] = false;
//		//	int s = 0;
//		//	for (int j = 0; j < 512; j++)
//		//		if (keyOptions[j])
//		//			s++;
//		//	int a = 0;
//		//}
//		uint32_t goods[32] = {};
//		for (int maskBit1 = 0; maskBit1 < 32; maskBit1++)
//			goods[maskBit1] = (sum[maskBit1] % 512);
//		std::sort(goods, goods + (sizeof(goods) / sizeof(goods[0])));
//		if (ArrCompare(goods, goodFix[theKey]))
//			suc++;
//	}
//	file << suc;
//	file.close();
//}

int arraysIntersection(int arr1[32], int arr2[32])
{
	int i = 0;
	int j = 0;
	int rVal = -1;
	while (i < 32 and j < 32)
	{
		if (arr1[i] == arr2[j]) { // found a common element.
			rVal = arr1[i]; // print it.
			i++; // move on.
			j++;
		}
		else if (arr1[i] > arr2[j])
			j++; // don't change maskBit1, move j.
		else
			i++; // don't change j, move maskBit1.
	}
	return rVal;
}

int commonKey(int k,int Ind[5])
{
	int CK = 0;
	for (int i = 0; i < 5; i++)
	{
		CK |= (((k >> Ind[i]) % 2) << i);
	}
	return CK;
}

void attack4RClasic()
{
	ofstream file;
	file.open("attack4Rclasic.txt");
	int relevantKeyBits[10] = { 11,102,125,70,93,18,27,32,7,16 };
	int keyLane[10] = { 0,9,9,6,6,0,0,3,0,0 };
	
	vector<LaneValue> diff(12);
	diff[1] = 1;
	diff[2] = 1;

	int suc = 0;

	for (int testInd = 0; testInd < NCHECK; testInd++)
	{
		uint32_t keys[1024] = {};
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 4; i++)
			plnA[3 * i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		uint32_t theKey = 0;
		for (int i = 0; i < 10; i++)
			if ((plnA[keyLane[i]] & ((uint32_t)1 << relevantKeyBits[i])))
				theKey |= ((uint32_t)1 << i);

		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			for (int i = 0; i < 4; i++)
			{
				plnA[3 * i + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				plnA[3 * i + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
			}

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			int relevantBits0_0_11[4] = { aState.getBit(230),aState.getBit(253),
				aState.getBit(358),aState.getBit(381) };
			int relevantBits0_1_11[5] = { aState.getBit(198),aState.getBit(221),
				aState.getBit(235),aState.getBit(326),aState.getBit(349) };
			int relevantBits1_0_0[4] = { aState.getBit(146),aState.getBit(155),
				aState.getBit(274),aState.getBit(283) };
			int relevantBits1_2_0[5] = { aState.getBit(135),aState.getBit(144),
				aState.getBit(263),aState.getBit(272),aState.getBit(309) };

			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;

			aInstance.permute(aState, 4);
			bInstance.permute(bState, 4);

			if (aState.getBit(143)==bState.getBit(143))
			{
				int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
				for (int i = 0; i < 4; i++)
				{
					a1 ^= relevantBits0_0_11[i];
					a2 ^= relevantBits1_0_0[i];
				}
					
				for (int i = 0; i < 5; i++)
				{
					b1 ^= relevantBits0_1_11[i];
					b2 ^= relevantBits1_2_0[i];
				}
					
				for (int kI1 = 0; kI1 < 8; kI1++)
				{
					for (int kI2 = 0; kI2 < 8; kI2++)
					{
						uint32_t gk = 0;
						if (a1)
						{
							gk += ((kI1 % 4) * 2);
							if ((kI1 % 4 == 0) || (kI1 % 4 == 3))
								gk += 1;
						}
						else
						{
							gk += 3 * (kI1 % 4);
							if ((kI1 % 4) == 3)
								gk -= 4;
						}
						if (b1)
							gk += ((3 * (kI1 / 4)) << 3);
						else
							gk += (((kI1 / 4) + 1) << 3);
						
						if (a2)
						{
							int k = 3 * (kI2 % 4);
							if ((kI2 % 4) == 3)
								k -= 4;
							gk += (k << 5);
						}
						else
						{
							int k = ((kI2 % 4) * 2);
							if ((kI2 % 4 == 0) || (kI2 % 4 == 3))
								k += 1;
							gk += (k << 5);
						}
						if (b2)
							gk += (((kI2 / 4) + 1) << 8);
						else
							gk += ((3 * (kI2 / 4)) << 8);
						keys[gk]++;
					}
				}
			}
		}
		int maxi = 0;
		int guessKey;
		for (int k = 0; k < 1024; k++)
		{
			if (keys[k] > maxi)
			{
				maxi = keys[k];
				guessKey = k;
			}
		}
		if (theKey == guessKey)
			suc++;
		else
		{
			file << keys[guessKey] << "\t" << keys[theKey] << endl;
		}
	}
	file << suc << endl;
	file.close();
}

void attack4RPNB()
{
	ofstream file;
	file.open("attack4RPNB.txt");
	int relevantKeyBits[10] = { 11,102,125,70,93,18,27,32,7,16 };
	int keyLane[10] = { 0,9,9,6,6,0,0,3,0,0 };

	vector<LaneValue> diff(12);
	diff[1] = 1;
	diff[2] = 1;

	int suc = 0;

	for (int testInd = 0; testInd < NCHECK; testInd++)
	{
		uint32_t keys[16] = {};
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 4; i++)
			plnA[3 * i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		uint32_t theKey = 0;
		for (int i = 0; i < 3; i++)
			theKey ^= (((plnA[keyLane[i]]) >> (relevantKeyBits[i] % 32)) & 1);
		for (int i = 0; i < 2; i++)
			theKey ^= ((((plnA[keyLane[3 + i]]) >> (relevantKeyBits[3 + i] % 32)) & 1) << 1);
		for (int i = 0; i < 3; i++)
			theKey ^= ((((plnA[keyLane[5 + i]]) >> (relevantKeyBits[5 + i] % 32)) & 1) << 2);
		for (int i = 0; i < 2; i++)
			theKey ^= ((((plnA[keyLane[8 + i]]) >> (relevantKeyBits[8 + i] % 32)) & 1) << 3);

		for (int prt = 0; prt < 16; prt++)
		{
			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
			{
				for (int i = 0; i < 4; i++)
				{
					plnA[3 * i + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
					plnA[3 * i + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				}

				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				aState.lanes = plnA;

				//aState.setBit(381, (prt % 2) ^ (aState.getBit(230)) ^
				//	(aState.getBit(253)) ^ (aState.getBit(358)));
				//aState.setBit(349, 1 ^ ((prt >> 1) % 2) ^ (aState.getBit(198)) ^
				//	(aState.getBit(221)) ^ (aState.getBit(235)) ^ (aState.getBit(326)));
				//aState.setBit(283, 1 ^ ((prt >> 2) % 2) ^ (aState.getBit(146)) ^
				//	(aState.getBit(155)) ^ (aState.getBit(274)));
				//aState.setBit(309, ((prt >> 3) % 2) ^ (aState.getBit(135)) ^
				//	(aState.getBit(144)) ^ (aState.getBit(263)) ^ (aState.getBit(272)));

				int otherBits = ((aState.getBit(230)) ^ (aState.getBit(253)) ^ (aState.getBit(358)));
				otherBits |= (((aState.getBit(198)) ^ (aState.getBit(221)) ^ (aState.getBit(235)) ^ (aState.getBit(326))) << 1);
				otherBits |= (((aState.getBit(146)) ^ (aState.getBit(155)) ^ (aState.getBit(274))) << 2);
				otherBits |= (((aState.getBit(135)) ^ (aState.getBit(144)) ^ (aState.getBit(263)) ^ (aState.getBit(272))) << 3);

				aState.setBit(381, (prt % 2));
				aState.setBit(349, ((prt >> 1) % 2));
				aState.setBit(283, ((prt >> 2) % 2));
				aState.setBit(309, ((prt >> 3) % 2));

				for (int i = 0; i < 12; i++)
					plnB[i] = aState.getLanes()[i] ^ diff[i];
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = plnB;

				aInstance.permute(aState, 4);
				bInstance.permute(bState, 4);
				if ((aState.getBit(143)) == (bState.getBit(143)))
				{
					int guesKey = ((prt % 2) ^ (otherBits % 2));
					guesKey |= (((prt >> 1) % 2) ^ ((otherBits >> 1) % 2) ^ 1) << 1;
					guesKey |= (((prt >> 2) % 2) ^ ((otherBits >> 2) % 2) ^ 1) << 2;
					guesKey |= (((prt >> 3) % 2) ^ ((otherBits >> 3) % 2)) << 3;
					keys[guesKey]++;
					guesKey ^= 8;
					keys[guesKey]++;
				}

				//if ((aState.getBit(143)) == (bState.getBit(143)))
				//	keys[prt]++;
			}
		}
		int maxi = 0;
		int guessKey;
		for (int k = 0; k < 16; k++)
		{
			if (keys[k] > maxi)
			{
				maxi = keys[k];
				guessKey = k;
			}
		}
		if (theKey == guessKey)
			suc++;
		else
		{
			file << keys[guessKey] << "\t" << keys[theKey] << endl;
		}
	}
	file << suc;
	file.close();
}

//void attack4RNB()
//{
//	ofstream file;
//	file.open("suc4R.txt");
//
//	int fixedBits[57] = { 4, 7, 11, 16, 18, 20, 25, 27, 32, 41, 70, 79, 93, 100, 102, 111, 114, 123, 125,
//	128, 132, 135, 137, 144, 146, 153, 155, 198, 207, 221, 228, 230, 235, 239, 242, 244, 251, 253,
//	256, 260, 263, 265, 272, 274, 281, 283, 309, 318, 326, 335, 349, 356, 358, 367, 370, 379, 381 };
//	//int commonKeyInd1[5] = { 2,4,6,8,10 };
//	//int commonKeyInd2[5] = { 1,4,6,9,11 };
//	uint32_t threshold = (NPLAINTEXT >> 1) - (NPLAINTEXT*0.0136);
//	vector<LaneValue> diff1(12);
//	diff1[1] = 1;
//	diff1[2] = 1;
//	vector<LaneValue> diff2(12);
//	diff2[1] = 512;
//	diff2[2] = 512;
//
//	for (int testInd = 0; testInd < NCHECK; testInd++)
//	{
//		vector<LaneValue> plnA(12);
//		vector<LaneValue> plnB(12);
//		vector<LaneValue> plnC(12);
//		for (int maskBit1 = 0; maskBit1 < 4; maskBit1++)
//			plnA[3 * maskBit1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//		uint32_t theKey = 0;
//		for (int k = 0; k < 8; k++)
//			theKey |= (plnA[0] & (((uint32_t)1 << fixedBits[k]) >> (fixedBits[k] - k)));
//		theKey |= ((plnA[3] & (uint32_t)1) << 8);
//		theKey |= (plnA[3] & (uint32_t)512);
//		theKey |= ((plnA[6] & (uint32_t)64) << 4);
//		theKey |= ((plnA[6] & (((uint32_t)1 << 15) >> 4)));
//		theKey |= ((plnA[6] & (((uint32_t)1 << 29) >> 17)));
//		theKey |= ((plnA[9] & (uint32_t)16) << 9);
//		theKey |= ((plnA[9] & (uint32_t)64) << 8);
//		theKey |= (plnA[9] & (((uint32_t)1 << 15)));
//		theKey |= ((plnA[9] & (((uint32_t)1 << 18) >> 2)));
//		theKey |= ((plnA[9] & (((uint32_t)1 << 27) >> 10)));
//		theKey |= ((plnA[9] & (((uint32_t)1 << 29) >> 11)));
//		bool key1[4096] = {};
//		bool key2[4096] = {};
//		for (int k = 0; k < 4096; k++) {
//			key1[k] = true;
//			key2[k] = true;
//		}
//		//int sumOptions = 32;
//		bool notYet1 = true;
//		bool notYet2 = true;
//		int numExp = 0;
//		while (notYet1||notYet2)
//		{
//			numExp++;
//			uint64_t fixedVal = rand() | (rand() << 15) | ((rand() % 256) << 30);
//			uint32_t sum1 = 0;
//			uint32_t sum2 = 0;
//			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
//			{
//				for (int maskBit1 = 0; maskBit1 < 4; maskBit1++)
//				{
//					plnA[3 * maskBit1 + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//					plnA[3 * maskBit1 + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//				}
//
//				XoodooParameters p(2, 8, 5, 14, 11);
//				const Xoodoo aInstance(4, 32, p);
//				XoodooState aState(aInstance);
//				aState.lanes = plnA;
//				for (int maskBit1 = 19; maskBit1 < 57; maskBit1++)
//					aState.setBit(fixedBits[maskBit1], fixedVal & ((uint64_t)1 << (maskBit1 - 19)));
//
//				for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
//					plnB[maskBit1] = aState.getLanes()[maskBit1] ^ diff1[maskBit1];
//				const Xoodoo bInstance(4, 32, p);
//				XoodooState bState(bInstance);
//				bState.lanes = plnB;
//
//				for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
//					plnC[maskBit1] = aState.getLanes()[maskBit1] ^ diff2[maskBit1];
//				const Xoodoo cInstance(4, 32, p);
//				XoodooState cState(cInstance);
//				cState.lanes = plnC;
//
//				aInstance.permute(aState, 4);
//				bInstance.permute(bState, 4);
//				cInstance.permute(cState, 4);
//
//				if (aState.getBit(82) == bState.getBit(82))
//					sum1++;
//				if (aState.getBit(91) == cState.getBit(91))
//					sum2++;
//			}
//			
//			bool key1Options[4096] = {};
//			bool key2Options[4096] = {};
//			keyPerFixed4RfrstGood(key1Options, fixedVal);
//			keyPerFixed4RscndGood(key2Options, fixedVal);
//
//			if (sum1 < threshold)
//			{
//				notYet1 = false;
//				for (int k = 0; k < 4096; k++)
//					if (!key1Options[k])
//						key1[k] = false;
//			}
//
//			if (sum2 < threshold)
//			{
//				notYet2 = false;
//				for (int k = 0; k < 4096; k++)
//					if (!key2Options[k])
//						key2[k] = false;
//			}
//		}
//		int hjfd = 0;
//	}
//}

void attack2Diffs()
{
	ofstream file;
	file.open("suc.txt");
	vector<LaneValue> diff1(12);
	diff1[0] = 0xa8b23b19;
	diff1[1] = 0xa8b23b18;
	diff1[2] = 0xa8b23b18;
	diff1[3] = 0x98810919;
	diff1[4] = 0x98810919;
	diff1[5] = 0x98810919;
	diff1[6] = 0x52674513;
	diff1[7] = 0x52674513;
	diff1[8] = 0x52676513;
	diff1[9] = 0x95a876f3;
	diff1[10] = 0x95a876f3;
	diff1[11] = 0x95a876f3;
	vector<LaneValue> diff2(12);
	diff2[0]  = 0xa0bb90a8;
	diff2[1]  = 0xa0bb90a9;
	diff2[2]  = 0xa09b90a9;
	diff2[3]  = 0xf7583d32;
	diff2[4]  = 0xf6583d32;
	diff2[5]  = 0xf7583d32;
	diff2[6]  = 0xc54b1bc8;
	diff2[7]  = 0xc44b1bc8;
	diff2[8]  = 0xc44b3bc8;
	diff2[9]  = 0x6795484a;
	diff2[10] = 0x6795484a;
	diff2[11] = 0x6795484a;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
		219,224,231,240,266,275,277,298,307,338,347,359,368 };
	int goodFix1[512][64] = {};
	int goodFix2[512][64] = {};
	setGoodFix(goodFix1, goodFix2);
	int suc = 0;

	for (int testInd = 0; testInd < 100; testInd++)
	{
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		vector<LaneValue> plnC(12);
		for (int i = 0; i < 4; i++)
			plnA[3 * i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		bool keyRelevantBits[9] = {};
		keyRelevantBits[0] = (plnA[0] & ((uint32_t)1 << (fixedBits[0] % 32)));
		keyRelevantBits[1] = (plnA[0] & ((uint32_t)1 << (fixedBits[1] % 32)));
		keyRelevantBits[2] = (plnA[3] & ((uint32_t)1 << (fixedBits[2] % 32)));
		keyRelevantBits[3] = (plnA[3] & ((uint32_t)1 << (fixedBits[3] % 32)));
		keyRelevantBits[4] = (plnA[6] & ((uint32_t)1 << (fixedBits[4] % 32)));
		keyRelevantBits[5] = (plnA[6] & ((uint32_t)1 << (fixedBits[5] % 32)));
		keyRelevantBits[6] = (plnA[6] & ((uint32_t)1 << (fixedBits[6] % 32)));
		keyRelevantBits[7] = (plnA[9] & ((uint32_t)1 << (fixedBits[7] % 32)));
		keyRelevantBits[8] = (plnA[9] & ((uint32_t)1 << (fixedBits[8] % 32)));
		int theKey = 0;
		//uint32_t keyOption;
		//bool keyOptions[512];
		//for (int k = 0; k < 512; k++)
		//	keyOptions[k] = true;
		for (int i = 0; i < 9; i++)
			theKey |= ((int)keyRelevantBits[i] << i);
		uint32_t sum1[512] = {};
		uint32_t sum2[512] = {};
		for (uint32_t suffix = 0; suffix < 512; suffix++)
		{
			uint32_t fix = (suffix << 9);
			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
			{
				for (int i = 0; i < 4; i++)
				{
					plnA[3 * i + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
					plnA[3 * i + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				}

				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				aState.lanes = plnA;
				for (int i = 9; i < 28; i++)
					aState.setBit(fixedBits[i], fix & ((uint32_t)1 << i));

				for (int i = 0; i < 12; i++)
					plnB[i] = aState.getLanes()[i] ^ diff1[i];
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = plnB;

				for (int i = 0; i < 12; i++)
					plnC[i] = aState.getLanes()[i] ^ diff2[i];
				const Xoodoo cInstance(4, 32, p);
				XoodooState cState(cInstance);
				cState.lanes = plnC;

				aInstance.permute(aState, 5);
				bInstance.permute(bState, 5);
				cInstance.permute(cState, 5);

				if (aState.getBit(0) == bState.getBit(0))
					sum1[suffix]++;
				if (aState.getBit(0) == cState.getBit(0))
					sum2[suffix]++;
			}
		}
		for (int suffix = 0; suffix < 512; suffix++)
		{
			sum1[suffix] <<= 9;
			sum1[suffix] |= suffix;
			sum2[suffix] <<= 9;
			sum2[suffix] |= suffix;
		}
		std::sort(sum1, sum1 + (sizeof(sum1) / sizeof(sum1[0])));
		std::sort(sum2, sum2 + (sizeof(sum2) / sizeof(sum2[0])));

		int goods1[32] = {};
		for (int i = 0; i < 32; i++)
			goods1[i] = (sum1[i] % 512);
		std::sort(goods1, goods1 + (sizeof(goods1) / sizeof(goods1[0])));

		int goods2[32] = {};
		for (int i = 0; i < 32; i++)
			goods2[i] = (sum2[i] % 512);
		std::sort(goods2, goods2 + (sizeof(goods2) / sizeof(goods2[0])));

		if (theKey == arraysIntersection(goods1, goods2))
			suc++;
		else
			file << arraysIntersection(goods1, goods2) << endl;
	}
	file << suc;
	file.close();
}

void attack5RPNB()
{
	ofstream file;
	file.open("attack5RPNB.txt");
	int relevantKeyBits[9] = { 82,91,103,112,42,51,88,10,19 };
	int keyLane[9] = { 6,6,9,9,3,3,6,0,0 };

	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	vector<LaneValue> diff(12);
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;

	int suc = 0;

	for (int testInd = 0; testInd < NCHECK; testInd++)
	{
		uint32_t keys[16] = {};
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 4; i++)
			plnA[3 * i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		uint32_t theKey = 0;
		for (int i = 0; i < 2; i++)
			theKey ^= (((plnA[keyLane[i]]) >> (relevantKeyBits[i] % 32)) & 1);
		for (int i = 0; i < 2; i++)
			theKey ^= ((((plnA[keyLane[2 + i]]) >> (relevantKeyBits[2 + i] % 32)) & 1) << 1);
		for (int i = 0; i < 3; i++)
			theKey ^= ((((plnA[keyLane[4 + i]]) >> (relevantKeyBits[4 + i] % 32)) & 1) << 2);
		for (int i = 0; i < 2; i++)
			theKey ^= ((((plnA[keyLane[7 + i]]) >> (relevantKeyBits[7 + i] % 32)) & 1) << 3);

		for (int guessKye = 0; guessKye < 16; guessKye++)
		{
			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
			{
				for (int i = 0; i < 4; i++)
				{
					plnA[3 * i + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
					plnA[3 * i + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				}

				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				aState.lanes = plnA;

				aState.setBit(347, (guessKye % 2) ^ (aState.getBit(210)) ^
					(aState.getBit(219)) ^ (aState.getBit(224)) ^ (aState.getBit(338)));
				aState.setBit(368, 1 ^ ((guessKye >> 1) % 2) ^ (aState.getBit(231)) ^
					(aState.getBit(240)) ^ (aState.getBit(277)) ^ (aState.getBit(359)));
				aState.setBit(307, ((guessKye >> 2) % 2) ^ (aState.getBit(170)) ^
					(aState.getBit(179)) ^ (aState.getBit(298)));
				aState.setBit(275, 1 ^ ((guessKye >> 3) % 2) ^ (aState.getBit(138)) ^
					(aState.getBit(147)) ^ (aState.getBit(184)) ^ (aState.getBit(266)));

				for (int i = 0; i < 12; i++)
					plnB[i] = aState.getLanes()[i] ^ diff[i];
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = plnB;

				//aInstance.theta(aState);
				//aInstance.rhoWest(aState);
				//aState.getLanes()[0] ^= rc[7];
				//bInstance.theta(bState);
				//bInstance.rhoWest(bState);
				//bState.getLanes()[0] ^= rc[7];

				//printf("%d\t%d\t%d\t%d\n", aState.getColumn(0, 0), bState.getColumn(0, 0),
				//	aState.getColumn(2, 24), bState.getColumn(2, 24));



				aInstance.permute(aState, 5);
				bInstance.permute(bState, 5);
				if ((aState.getBit(0)) == (bState.getBit(0)))
					keys[guessKye]++;
			}
		}
		//int maxi = 0;
		//int guessKey = 0;
		//for (int k = 0; k < 16; k++)
		//{
		//	if (keys[k] > maxi)
		//	{
		//		maxi = keys[k];
		//		guessKey = k;
		//	}
		//}
		int mini = NPLAINTEXT;
		int guessKey = 0;
		for (int k = 0; k < 16; k++)
		{
			if (keys[k] < mini)
			{
				mini = keys[k];
				guessKey = k;
			}
		}
		if (theKey == guessKey)
			suc++;
		//else
		//{
		//	file << keys[guessKey] << "\t" << keys[theKey] << endl;
		//}
	}
	file << suc;
	file.close();
}

void attack()
{
	ofstream file;
	file.open("suc.txt");
	vector<LaneValue> diff(12);
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
		219,224,231,240,266,275,277,298,307,338,347,359,368 };
	uint32_t goodFix[512][32] = {};
	bool relatedFix[512][512] = {};
	setGoodFix(goodFix, relatedFix);
	//int sumRelFix[512] = {};
	//for (int maskBit1 = 0; maskBit1 < 512; maskBit1++)
	//	for (int j = 0; j < 512; j++)
	//		sumRelFix[maskBit1] += relatedFix[maskBit1][j];
	int suc = 0;

	for (int testInd = 0; testInd < 100; testInd++)
	{
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 4; i++)
			plnA[3 * i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		bool keyRelevantBits[9] = {};
		keyRelevantBits[0] = (plnA[0] & ((uint32_t)1 << (fixedBits[0] % 32)));
		keyRelevantBits[1] = (plnA[0] & ((uint32_t)1 << (fixedBits[1] % 32)));
		keyRelevantBits[2] = (plnA[3] & ((uint32_t)1 << (fixedBits[2] % 32)));
		keyRelevantBits[3] = (plnA[3] & ((uint32_t)1 << (fixedBits[3] % 32)));
		keyRelevantBits[4] = (plnA[6] & ((uint32_t)1 << (fixedBits[4] % 32)));
		keyRelevantBits[5] = (plnA[6] & ((uint32_t)1 << (fixedBits[5] % 32)));
		keyRelevantBits[6] = (plnA[6] & ((uint32_t)1 << (fixedBits[6] % 32)));
		keyRelevantBits[7] = (plnA[9] & ((uint32_t)1 << (fixedBits[7] % 32)));
		keyRelevantBits[8] = (plnA[9] & ((uint32_t)1 << (fixedBits[8] % 32)));
		uint32_t theKey = 0;
		//uint32_t keyOption;
		//bool keyOptions[512];
		//for (int k = 0; k < 512; k++)
		//	keyOptions[k] = true;
		for (int i = 0; i < 9; i++)
			theKey |= ((uint32_t)keyRelevantBits[i] << i);
		uint32_t sum[512] = {};
		for (uint32_t suffix = 0; suffix < 512; suffix++)
		{
			uint32_t fix = (suffix << 9);
			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
			{
				for (int i = 0; i < 4; i++)
				{
					plnA[3 * i + 1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
					plnA[3 * i + 2] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				}

				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				aState.lanes = plnA;
				for (int i = 9; i < 28; i++)
					aState.setBit(fixedBits[i], fix & ((uint32_t)1 << i));
				for (int i = 0; i < 12; i++)
					plnB[i] = aState.getLanes()[i] ^ diff[i];
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = plnB;
				aInstance.permute(aState, 5);
				bInstance.permute(bState, 5);
				if (aState.getBit(0) == bState.getBit(0))
					sum[suffix]++;
			}
		}
		for (int suffix = 0; suffix < 512; suffix++)
		{
			sum[suffix] <<= 9;
			sum[suffix] |= suffix;
		}
		std::sort(sum, sum + (sizeof(sum) / sizeof(sum[0])));
		//for (int suffix = 0; suffix < 512; suffix++)
		//	file << sum[suffix] << endl;
		//int g = 0;
		//for (int maskBit1 = 0; maskBit1 < 32; maskBit1++)
		//{
		//	uint32_t aGood = (sum[maskBit1] % 512);
		//	for (int j = 0; j < 512; j++)
		//		if (!relatedFix[aGood][j])
		//			keyOptions[j] = false;
		//	int s = 0;
		//	for (int j = 0; j < 512; j++)
		//		if (keyOptions[j])
		//			s++;
		//	int a = 0;
		//}
		uint32_t goods[32] = {};
		for (int i = 0; i < 32; i++)
			goods[i] = (sum[i] % 512);
		std::sort(goods, goods + (sizeof(goods) / sizeof(goods[0])));
		if (ArrCompare(goods, goodFix[theKey]))
			suc++;
	}
	file << suc;
	file.close();
}

void diffLin122(int nRounds)
{
	ofstream file;
	switch (nRounds)
	{
	case 3:
		file.open("bestBiases3R.txt");
		break;
	case 4:
		file.open("bestBiases4R.txt");
		break;
	case 5:
		file.open("bestBiases5R.txt");
		break;
	default:
		file.open("problem.txt");
		break;
	}
	vector<LaneValue> l(0);
	l.resize(12);

	for (int diffBit = 0; diffBit < 384; diffBit++)
	{
		for (int maskBit1 = 0; maskBit1 < 383; maskBit1++)
		{
			for (int maskBit2 = maskBit1 + 1; maskBit2 < 384; maskBit2++)
			{
				int sum = 0;
				for (int plntxtInd = 0; plntxtInd < NPLAINTEXT; plntxtInd++)
				{
					//Create the first instance
					XoodooParameters p(2, 8, 5, 14, 11);
					const Xoodoo aInstance(4, 32, p);
					XoodooState aState(aInstance);
					for (int j = 0; j < 12; j++)
						l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
					aState.lanes = l;

					//Compute the difference Instance
					int index = diffBit;
					unsigned int z = index % 32;
					index /= 32;
					unsigned int x = index % 4;
					index /= 4;
					unsigned int y = index;
					const Xoodoo bInstance(4, 32, p);
					XoodooState bState(bInstance);
					bState.lanes = l;
					bState.invertBit(x, y, z);

					//Compute the ciphertexts and check the mask
					aInstance.permute(aState, nRounds);
					bInstance.permute(bState, nRounds);
					index = maskBit1;
					z = index % 32;
					index /= 32;
					x = index % 4;
					index /= 4;
					y = index;
					uint32_t mask1 = ((uint32_t)1 << z);
					bool parity1 = ((aState.getLane(x, y) & mask1) == (bState.getLane(x, y) & mask1));
					index = maskBit2;
					z = index % 32;
					index /= 32;
					x = index % 4;
					index /= 4;
					y = index;
					uint32_t mask2 = ((uint32_t)1 << z);
					bool parity2 = ((aState.getLane(x, y) & mask2) == (bState.getLane(x, y) & mask2));
					sum += (parity1 == parity2);
				}
				double dBias = abs((double)sum / (double)NPLAINTEXT - 0.5);
				if (dBias > 0.09)
					file << diffBit << "\t" << maskBit1 << "\t" << maskBit2 << "\t" <<
					sum << endl;
			}
		}
	}
	file.close();
}

void diffLin2215r()
{
	ofstream file;
	file.open("bias5R221.txt");
	//vector<LaneValue> l(12);

	//int fixedBits[28] = { 31,82,91,102,103,111,112,113,122,210,219,224,230,231,239,240,241,
	//	250,276,277,338,347,358,359,367,368,369,378 };

	uint32_t diff[12] = {};
	diff[0] = leftRotate(0xa8b23b19, 1);
	diff[1] = leftRotate(0xa8b23b18, 1);
	diff[2] = leftRotate(0xa8b23b18, 1);
	diff[3] = leftRotate(0x98810919, 1);
	diff[4] = leftRotate(0x98810919, 1);
	diff[5] = leftRotate(0x98810919, 1);
	diff[6] = leftRotate(0x52674513, 1);
	diff[7] = leftRotate(0x52674513, 1);
	diff[8] = leftRotate(0x52676513, 1);
	diff[9] = leftRotate(0x95a876f3, 1);
	diff[10] = leftRotate(0x95a876f3, 1);
	diff[11] = leftRotate(0x95a876f3, 1);
	uint32_t sum = 0;
	for (uint32_t plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
	{
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 12; i++)
			plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

		XoodooParameters p(2, 8, 5, 14, 11);
		const Xoodoo aInstance(4, 32, p);
		XoodooState aState(aInstance);
		aState.lanes = plnA;
		for (int i = 0; i < 12; i++)
			plnB[i] = plnA[i] ^ diff[i];
		const Xoodoo bInstance(4, 32, p);
		XoodooState bState(bInstance);
		bState.lanes = plnB;
		aInstance.permute(aState, 5);
		bInstance.permute(bState, 5);
		if ((aState.getBit(1)) == (bState.getBit(1)))
			sum++;
	}
	file << sum << endl;
	file.close();
}

void diffLin2214r()
{
	ofstream file;
	file.open("biases4R221.txt");
	//int problemBits[6] = { 8,53,56,59,94,114 };
	
	for (int diffOption = 1; diffOption < 2; diffOption++)
	{
		for (int diffBit = 0; diffBit < 1; diffBit++)
		{
			for (int maskBit = 0; maskBit < 192; maskBit++)
			{
			int pos1 = 3 * (diffBit / 32) + diffOption % 2;
			int pos2 = 3 * (diffBit / 32) + ((diffOption + 1) / 2) + 1;
			vector<LaneValue> diff(12);
			diff[pos1] = ((uint32_t)1 << (diffBit % 32));
			diff[pos2] = ((uint32_t)1 << (diffBit % 32));
			uint32_t sum = 0;
			for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
			{
				vector<LaneValue> plnA(12);
				vector<LaneValue> plnB(12);
				for (int i = 0; i < 12; i++)
					plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
				for (int i = 0; i < 12; i++)
					plnB[i] = plnA[i] ^ diff[i];
				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				aState.lanes = plnA;
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = plnB;
				aInstance.permute(aState, 4);
				bInstance.permute(bState, 4);
				if ((aState.getBit(maskBit)) == (bState.getBit(maskBit)))
					sum++;
			}
			file << sum << endl;
			//double bias = 0.5 - (((double)sum) / ((double)NPLAINTEXT));
			//if (abs(bias) > 0.0165)
			//file << diffOption << "\t" << diffBit << "\t" << maskBit << "\t" << sum << endl;
			}
		}
	}
	file.close();
}

void diffLin221Fixed(int nRounds)
{
	ofstream file;
	ofstream file2;
	file2.open("DL221Details.txt");
	switch (nRounds)
	{
	case 3:
		file.open("biases3R221.txt");
		break;
	case 4:
		file.open("biases4R221.txt");
		break;
	case 5:
		file.open("biases5R221.txt");
		break;
	default:
		file.open("problem.txt");
		break;
	}
	vector<LaneValue> l(0);
	l.resize(12);

	int diffBit1 = 0;
	int diffBit2 = 128;
	int masks[2] = { 93,328 };
	for (int maskBit = 0; maskBit < 2; maskBit++)
	{
		for (int fixInd = 0; fixInd < 16; fixInd++)
		{
			int sum = 0;
			for (int plntxtInd = 0; plntxtInd < NPLAINTEXT; plntxtInd++)
			{
				//Create the first instance
				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				for (int j = 0; j < 12; j++)
					l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
				aState.lanes = l;

				//Compute the difference Instance
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = l;

				aState.setBit(128, fixInd % 2);
				aState.setBit(256, (fixInd / 2) % 2);
				aState.setBit(32, (fixInd / 4) % 2);
				aState.setBit(288, (fixInd / 8) % 2);
				bState.setBit(128, fixInd % 2);
				bState.setBit(256, (fixInd / 2) % 2);
				bState.setBit(32, (fixInd / 4) % 2);
				bState.setBit(288, (fixInd / 8) % 2);

				int index = diffBit1;
				unsigned int z = index % 32;
				index /= 32;
				unsigned int x = index % 4;
				index /= 4;
				unsigned int y = index;
				bState.invertBit(x, y, z);
				index = diffBit2;
				z = index % 32;
				index /= 32;
				x = index % 4;
				index /= 4;
				y = index;
				bState.invertBit(x, y, z);
				if (plntxtInd == 0)
				{
					for (int i = 0; i < 12; i++)
						file2 << std::hex <<
						(aState.getLanes()[i] ^ bState.getLanes()[i]) << endl;
					file2 << endl;
				}
				
				//Compute the ciphertexts and check the mask
				aInstance.chi(aState);
				aInstance.rhoEast(aState);
				bInstance.chi(bState);
				bInstance.rhoEast(bState);
				if (plntxtInd == 0)
				{
					for (int i = 0; i < 12; i++)
						file2 << std::hex <<
						(aState.getLanes()[i] ^ bState.getLanes()[i]) << endl;
					file2 << endl;
				}
				aInstance.permute(aState, nRounds - 1);
				bInstance.permute(bState, nRounds - 1);

				index = masks[maskBit];
				z = index % 32;
				index /= 32;
				x = index % 4;
				index /= 4;
				y = index;
				uint32_t mask = ((uint32_t)1 << z);
				bool parity = ((aState.getLane(x, y) & mask) == (bState.getLane(x, y) & mask));
				sum += parity;
			}
			file << sum << endl;
		}
	}
	file.close();
}

void diffLin221(int nRounds)
{
	ofstream file;
	switch (nRounds)
	{
	case 3:
		file.open("bestBiases3R221.txt");
		break;
	case 4:
		file.open("bestBiases4R221.txt");
		break;
	case 5:
		file.open("bestBiases5R221.txt");
		break;
	default:
		file.open("problem.txt");
		break;
	}
	vector<LaneValue> l(0);
	l.resize(12);

	for (int diffBit1 = 0; diffBit1 < 383; diffBit1++)
	{
		for (int diffBit2 = diffBit1 + 1; diffBit2 < 384; diffBit2++)
		{
			for (int maskBit = 0; maskBit < 384; maskBit++)
			{
				int sum = 0;
				for (int plntxtInd = 0; plntxtInd < NPLAINTEXT; plntxtInd++)
				{
					//Create the first instance
					XoodooParameters p(2, 8, 5, 14, 11);
					const Xoodoo aInstance(4, 32, p);
					XoodooState aState(aInstance);
					for (int j = 0; j < 12; j++)
						l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
					aState.lanes = l;

					//Compute the difference Instance
					const Xoodoo bInstance(4, 32, p);
					XoodooState bState(bInstance);
					bState.lanes = l;
					int index = diffBit1;
					unsigned int z = index % 32;
					index /= 32;
					unsigned int x = index % 4;
					index /= 4;
					unsigned int y = index;
					bState.invertBit(x, y, z);
					index = diffBit2;
					z = index % 32;
					index /= 32;
					x = index % 4;
					index /= 4;
					y = index;
					bState.invertBit(x, y, z);

					//Compute the ciphertexts and check the mask
					aInstance.permute(aState, nRounds);
					bInstance.permute(bState, nRounds);
					
					index = maskBit;
					z = index % 32;
					index /= 32;
					x = index % 4;
					index /= 4;
					y = index;
					uint32_t mask = ((uint32_t)1 << z);
					bool parity = ((aState.getLane(x, y) & mask) == (bState.getLane(x, y) & mask));
					sum += parity;
				}
				double dBias = abs((double)sum / (double)NPLAINTEXT - 0.5);
				if (dBias > 0.09)
					file << diffBit1 << "\t" << diffBit2 << "\t" << maskBit << "\t" <<
					sum << endl;
			}
		}
	}
	file.close();
}

void specificDLCT()
{
	ofstream file;
	file.open("dlct.txt");
	vector<LaneValue> diff(12);
	diff[0] = 0x1402803;
	diff[1] = 0x2805006;
	diff[2] = 0xbc06046;
	diff[3] = 0x1e03d072;
	diff[4] = 0x3c07a0e4;
	diff[5] = 0x40300a01;
	diff[6] = 0x460bc060;
	diff[7] = 0x8c1780c0;
	diff[8] = 0x40280301;
	diff[9] = 0x140300a;
	diff[10] = 0x2806014;
	diff[11] = 0x3d0721e;
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	for (int testInd = 0; testInd < 100; testInd++)
	{
		int sum = 0;
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			aInstance.round(aState, rc[10]);
			bInstance.round(bState, rc[10]);
			aInstance.round(aState, rc[11]);
			bInstance.round(bState, rc[11]);
			if (aState.getBit(0) == bState.getBit(0))
				sum++;
		}
		file << sum << endl;
	}
	file.close();
}

void setDiff5R(LaneValue diff[12], int index)
{
	switch (index)
	{
	case 0:
		diff[0] = 0x1;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 1:
		diff[0] = 0x1;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 2:
		diff[0] = 0x1;
		diff[1] = 0x1000;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 3:
		diff[0] = 0x1;
		diff[1] = 0x1000;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 4:
		diff[0] = 0x1;
		diff[1] = 0x2;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 5:
		diff[0] = 0x1;
		diff[1] = 0x2;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 6:
		diff[0] = 0x1;
		diff[1] = 0x1002;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 7:
		diff[0] = 0x1;
		diff[1] = 0x1002;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 8:
		diff[0] = 0x801;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 9:
		diff[0] = 0x801;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 10:
		diff[0] = 0x801;
		diff[1] = 0x1000;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 11:
		diff[0] = 0x801;
		diff[1] = 0x1000;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 12:
		diff[0] = 0x801;
		diff[1] = 0x2;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 13:
		diff[0] = 0x801;
		diff[1] = 0x2;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 14:
		diff[0] = 0x801;
		diff[1] = 0x1002;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80000;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 15:
		diff[0] = 0x801;
		diff[1] = 0x1002;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 0;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 0x80100;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	default:
		break;
	}
}

void setDiff4R(LaneValue diff[12], int index)
{
	switch (index)
	{
	case 0:
		diff[0] = 0;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 1:
		diff[0] = 0;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 2:
		diff[0] = 0;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 3:
		diff[0] = 0;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 4:
		diff[0] = 2048;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 5:
		diff[0] = 2048;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 6:
		diff[0] = 2048;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 7:
		diff[0] = 2048;
		diff[1] = 0;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 8:
		diff[0] = 0;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 9:
		diff[0] = 0;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 10:
		diff[0] = 0;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 11:
		diff[0] = 0;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 12:
		diff[0] = 2048;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 13:
		diff[0] = 2048;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 0;
		break;
	case 14:
		diff[0] = 2048;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 0;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	case 15:
		diff[0] = 2048;
		diff[1] = 4096;
		diff[2] = 0;
		diff[3] = 1;
		diff[4] = 2;
		diff[5] = 0;
		diff[6] = 0;
		diff[7] = 0;
		diff[8] = 524288;
		diff[9] = 0;
		diff[10] = 0;
		diff[11] = 256;
		break;
	default:
		break;
	}
}

void bestThirdRDiff()
{
	unsigned int x[14] = { 0,0,0,1,1,1,1,2,2,2,3,3,3,3 };
	unsigned int z[14] = {0,1,24,5,14,16,25,5,14,30,1,3,12,24};
	const ColumnValue colIn[14] = { 1,2,2,1,1,4,4,2,2,4,1,4,4,1 };
	int ddt[5][4] = {};
	for (int i = 0; i < 4; i++)
	{
		ddt[1][i] = 2 * i + 1;
		ddt[2][i] = i + 2 * ((2 + i) / 2);
		ddt[4][i] = i + 4;
	}
	ofstream file;
	file.open("bestThirdRDiff.txt");
	int minActives = 128;
	for (uint32_t diff = 0; diff < NPLAINTEXT; diff++)
	{
		vector<LaneValue> plnA(12);
		XoodooParameters p(2, 8, 5, 14, 11);
		const Xoodoo aInstance(4, 32, p);
		XoodooState aState(aInstance);
		aState.lanes = plnA;
		for (int c = 0; c < 14; c++)
			aState.setColumn(x[c], z[c], (ddt[colIn[c]][((diff >> 2 * c) % 4)]));
		//for (int y = 0; y < 3; y++)
		//{
		//	for (int xx = 0; xx < 4; xx++)
		//		printf("%lx\t", aState.getLane(xx, y));
		//	printf("\n");
		//}
		//printf("\n");
		aInstance.rhoEast(aState);
		//for (int y = 0; y < 3; y++)
		//{
		//	for (int xx = 0; xx < 4; xx++)
		//		printf("%lx\t", aState.getLane(xx, y));
		//	printf("\n");
		//}
		//printf("\n");
		//for (int y = 0; y < 3; y++)
		//{
		//	for (int xx = 0; xx < 4; xx++)
		//		printf("%d\t", aState.getLane(xx, y));
		//	printf("\n");
		//}
		//printf("\n");
		aInstance.theta(aState);
		//for (int y = 0; y < 3; y++)
		//{
		//	for (int xx = 0; xx < 4; xx++)
		//		printf("%lx\t", aState.getLane(xx, y));
		//	printf("\n");
		//}
		//printf("\n");
		aInstance.rhoWest(aState);

		int sumActives = 0;
		for (int xx = 0; xx < 4; xx++)
			for (int zz = 0; zz < 32; zz++)
				if (aState.getColumn(xx, zz))
					sumActives++;
		//if (sumActives < minActives)
		//{
		//	minActives = sumActives;
		//	printf("%lx\t%d\n", diff, minActives);
		//}
		if (sumActives <= minActives)
		{
			minActives = sumActives;
			file << std::hex << diff << "\t" << minActives << endl;
		}
	}
	file.close();
}

void DL3Rfor6R()
{
	uint32_t diff[12] = {
		1,
		33554436,
		64,
		16416,
		0,
		1050624,
		0,
		32832,
		0,
		16777218,
		0,
		16777218,
	};
	//unsigned int x[14] = { 0,0,0,1,1,1,1,2,2,2,3,3,3,3 };
	//unsigned int z[14] = {0,1,24,5,14,16,25,5,14,30,1,3,12,24};
	//const ColumnValue colVal[14] = { 1,2,2,1,1,4,4,2,2,4,1,4,4,1 };
	ofstream file;
	file.open("diff2Rfor5R.txt");
	//uint32_t sum[192] = {};
	int sum[8] = {};

	for (uint32_t plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
	{
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 12; i++)
			plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
		for (int i = 0; i < 12; i++)
			plnB[i] = plnA[i] ^ diff[i];
		XoodooParameters p(2, 8, 5, 14, 11);
		const Xoodoo aInstance(4, 32, p);
		XoodooState aState(aInstance);
		aState.lanes = plnA;
		const Xoodoo bInstance(4, 32, p);
		XoodooState bState(bInstance);
		bState.lanes = plnB;
		aInstance.permute(aState, 2);
		bInstance.permute(bState, 2);
		aInstance.inverseRhoEast(aState);
		bInstance.inverseRhoEast(bState);
		//for (int maskBit = 0; maskBit < 192; maskBit++)
		//	sum[maskBit] += (aState.getBit(maskBit) ^ bState.getBit(maskBit));
		sum[(aState.getColumn(0, 0)) ^ (bState.getColumn(0, 0))]++;
	}
	for (int i = 0; i < 8; i++)
	{
		file << sum[i] << endl;
	}
	//for (int maskBit = 0; maskBit < 192; maskBit++)
	//	file << sum[maskBit] << endl;
	file.close();
}

void detailsofaRound()
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	ofstream file;
	file.open("detail.txt");
	for (int diffOption = 0; diffOption < 2; diffOption++)
	{
		LaneValue diff[12] = {};
		setDiff5R(diff, 2 * diffOption);
		for (int plnInd = 0; plnInd < 1; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
			for (int i = 0; i < 12; i++)
				plnB[i] = plnA[i] ^ diff[i];
			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			aInstance.theta(aState);
			aInstance.rhoWest(aState);
			aState.getLanes()[0] ^= rc[9];
			bInstance.theta(bState);
			bInstance.rhoWest(bState);
			bState.getLanes()[0] ^= rc[9];
			//aInstance.round(aState, rc[9]);
			//bInstance.round(bState, rc[9]);
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					file << (aState.getLane(x, y) ^ bState.getLane(x, y)) << "\t";
				}
				file << endl;
			}
			file << endl;

			for (int x = 0; x < 4; x++)
			{
				for (int z = 0; z < 32; z++)
				{
					if ((aState.getColumn(x, z)) ^ (bState.getColumn(x, z)))
						file << x << "\t" << z << "\t" <<
						((aState.getColumn(x, z)) ^ (bState.getColumn(x, z))) << endl;
				}
			}
			//aInstance.round(aState, rc[10]);
			//bInstance.round(bState, rc[10]);
			//for (int y = 0; y < 3; y++)
			//{
			//	for (int x = 0; x < 4; x++)
			//	{
			//		file << std::hex << (aState.getLane(x, y) ^ bState.getLane(x, y)) << "\t";
			//	}
			//	file << endl;
			//}
			//file << endl;
			//aInstance.round(aState, rc[11]);
			//bInstance.round(bState, rc[11]);
			//file << (aState.getBit(0) ^ bState.getBit(0)) << endl;
			//file << endl;
		}
	}
	file.close();
}

void diffLin5RWithFix()
{
	ofstream file;
	file.open("DL5R221WithFix.txt");
	//ofstream file2;
	//file2.open("DL221Details.txt");
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	LaneValue diff[12] = {};
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;

	int fixedBits[28] = { 10,19,42,51,82,88,91,103,112,138,147,170,179,184,210,
	219,224,231,240,266,275,277,298,307,338,347,359,368 };
	uint32_t goodFix[16384] = {};
	setGoodFix(goodFix);

	for (int testInd = 0; testInd < 100; testInd++)
	{
		uint32_t fix = goodFix[rand() % 16384];
		//LaneValue res[12] = {};
		//setDiff5R(diff, 2 * testInd);
		uint32_t sum = 0;
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 28; i++)
				aState.setBit(fixedBits[i], fix&((uint32_t)1 << i));
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			//for (int r = -4; r <= 0; r++)
			//{
			//	aInstance.round(aState, rc[r + 11]);
			//	bInstance.round(bState, rc[r + 11]);
			//	if (r == -3)
			//	{
			//		for (int l = 0; l < 12; l++)
			//			file2 << std::hex << ((aState.getLanes()[l]) ^ (bState.getLanes()[l])) << endl;
			//		file2 << endl;
			//	}
			//}
			aInstance.permute(aState, 5);
			bInstance.permute(bState, 5);
			if (aState.getBit(0) == bState.getBit(0))
				sum++;
		}
		file << sum << endl;
	}
	file.close();
	//file2.close();
}

void Diff3RFor4R()
{
	ofstream file;
	file.open("Diff3RFor4R.txt");
	LaneValue diff[12] = {};
	for (int diffInd = 0; diffInd < 16; diffInd++)
	{
		//int sum[4] = {};
		int sum[18432] = {};
		//if (diffInd)
		//	setDiff5R(diff, 14);
		//else
		//	setDiff5R(diff, 13);
		setDiff4R(diff, diffInd);
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			aInstance.permute(aState, 3);
			bInstance.permute(bState, 3);
			int maskInd = 0;
			for (int maskBit1 = 0; maskBit1 < 191; maskBit1++)
			{
				for (int maskBit2 = maskBit1 + 1; maskBit2 < 192; maskBit2++)
				{
					int aval = aState.getBit(maskBit1) ^ aState.getBit(maskBit2);
					int bval = bState.getBit(maskBit1) ^ bState.getBit(maskBit2);
					sum[maskInd++] += (aval^bval);
				}
			}
			//int aval = aState.getBit(0) + 2 * aState.getBit(129);
			//int bval = bState.getBit(0) + 2 * bState.getBit(129);
			//sum += (aState.getBit(143)) ^ (bState.getBit(143));
			//sum[aval^bval]++;
		}
		for (int i = 0; i < 18432; i++)
			file << sum[i] << endl;
		//for (int maskBit1 = 0; maskBit1 < 4; maskBit1++)
		//	file << sum[maskBit1] << "\t";
		//file << endl;
	}
}

void Diff3RFor5R()
{
	ofstream file;
	file.open("Diff3RFor5R.txt");
	LaneValue diff[12] = {};
	for (int diffInd = 0; diffInd < 16; diffInd++)
	{
		int sum[18432] = {};
		setDiff5R(diff, diffInd);
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			aInstance.permute(aState, 3);
			bInstance.permute(bState, 3);
			int maskInd = 0;
			for (int maskBit1 = 0; maskBit1 < 191; maskBit1++)
			{
				for (int maskBit2 = maskBit1 + 1; maskBit2 < 192; maskBit2++)
				{
					int aval = aState.getBit(maskBit1) ^ aState.getBit(maskBit2);
					int bval = bState.getBit(maskBit1) ^ bState.getBit(maskBit2);
					sum[maskInd++] += (aval^bval);
				}
			}
		}
		for (int i = 0; i < 18432; i++)
			file << sum[i] << endl;
	}
}

void diffLin3R()
{
	ofstream file;
	file.open("DL3RFor4RROT.txt");
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	uint32_t sum[128][8] = {};
	for (int diffBit = 0; diffBit < 128; diffBit++)
	{
		vector<LaneValue> tempDiff(12);
		tempDiff[2 + 3 * (diffBit / 32)] = leftRotate(0x800, diffBit % 32);
		tempDiff[(4 + 3 * (diffBit / 32)) % 12] = leftRotate(1, diffBit % 32);
		XoodooParameters p(2, 8, 5, 14, 11);
		Xoodoo temp(4, 32, p);
		XoodooState tempS(temp);
		tempS.lanes = tempDiff;
		temp.rhoEast(tempS);
		LaneValue diff[12] = {};
		for (int l = 0; l < 12; l++)
			diff[l] = tempS.getLanes()[l];
		
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(bInstance);
			bState.lanes = plnB;
			aInstance.permute(aState, 3);
			bInstance.permute(bState, 3);
			uint32_t x = diffBit / 32;
			uint32_t z = ((15 + diffBit) % 32);
			sum[diffBit][(aState.getColumn(x, z)) ^ (bState.getColumn(x, z))]++;
		}
		for (int s = 0; s < 8; s++)
			file << sum[diffBit][s] << "\t";
		file << endl;
	}


	//vector<LaneValue> tempDiff(12);
	////tempDiff[0] = ((uint32_t)1 << 11);
	////tempDiff[1] = ((uint32_t)(d1 / 2) << 20);
	//tempDiff[2] = ((uint32_t)1 << 11);
	////tempDiff[3] = 1;
	//tempDiff[4] = 1;
	////tempDiff[5] = ((uint32_t)(d2 / 2) << 9);
	//XoodooParameters p(2, 8, 5, 14, 11);
	//Xoodoo temp(4, 32, p);
	//XoodooState tempS(temp);
	//tempS.lanes = tempDiff;
	//temp.rhoEast(tempS);
	//LaneValue diff[12] = {};
	//for (int l = 0; l < 12; l++)
	//	diff[l] = tempS.getLanes()[l];
	//uint32_t sum[64] = {};
	//for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
	//{
	//	vector<LaneValue> plnA(12);
	//	vector<LaneValue> plnB(12);
	//	for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
	//		plnA[maskBit1] = rand() | (rand() << 15) | ((rand() % 4) << 30);

	//	XoodooParameters p(2, 8, 5, 14, 11);
	//	const Xoodoo aInstance(4, 32, p);
	//	XoodooState aState(aInstance);
	//	aState.lanes = plnA;
	//	for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
	//		plnB[maskBit1] = aState.getLanes()[maskBit1] ^ diff[maskBit1];
	//	const Xoodoo bInstance(4, 32, p);
	//	XoodooState bState(bInstance);
	//	bState.lanes = plnB;
	//	aInstance.permute(aState, 3);
	//	bInstance.permute(bState, 3);
	//	for (int maskBit = 0; maskBit < 32; maskBit++)
	//	{
	//		sum[maskBit] += (aState.getBit(maskBit) ^ bState.getBit(maskBit)
	//			^ aState.getBit(128 + ((maskBit + 1) % 32)) ^ bState.getBit(128 + ((maskBit + 1) % 32)));
	//		sum[maskBit + 32] += (aState.getBit(maskBit + 32) ^ bState.getBit(maskBit + 32)
	//			^ aState.getBit(160 + ((maskBit + 1) % 32)) ^ bState.getBit(160 + ((maskBit + 1) % 32)));
	//	}
	//	//if (aState.getBit(133) == bState.getBit(133))
	//	//	sum++;
	//}
	//for (int s = 0; s < 64; s++)
	//	file << sum[s] << endl;
	file.close();
}

void diffLin4RWithFix()
{
	ofstream file;
	file.open("diffLin4RWithFix.txt");
	vector<LaneValue> diff(12);
	diff[1] = 512;
	diff[2] = 512;

	//int fixedBits[36] = { 7, 11, 16, 18, 27, 32, 70, 93, 102, 114, 123, 125,
	//					128, 135, 144, 146, 155, 198, 221, 230, 235, 242, 251, 253,
	//					256, 263, 272, 274, 283, 309, 326, 349, 358, 370, 379, 381 }; // Fixed for bit 1

	int fixedBits[36] = { 4, 16, 20, 25, 27, 41, 70, 79, 100, 102, 111, 123,
						132, 137, 144, 153, 155, 198, 207, 228, 230, 239, 244, 251,
						260, 265, 272, 281, 283, 318, 326, 335, 356, 358, 367, 379 }; // Fixed for bit 9

	for (int testInd = 0; testInd < NCHECK; testInd++)
	{
		uint32_t sum = 0;
		uint64_t fix = rand() | (rand() << 15) | ((rand() % 64) << 30);
		int howToFix[36] = {};
		for (int i = 0; i < 36; i++)
			howToFix[i] = ((fix >> i) % 2);
		for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
		{
			vector<LaneValue> plnA(12);
			vector<LaneValue> plnB(12);
			for (int i = 0; i < 12; i++)
				plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);

			XoodooParameters p(2, 8, 5, 14, 11);
			const Xoodoo aInstance(4, 32, p);
			XoodooState aState(aInstance);
			aState.lanes = plnA;
			for (int i = 0; i < 36; i++)
				aState.setBit(fixedBits[i], howToFix[i]);
			for (int i = 0; i < 12; i++)
				plnB[i] = aState.getLanes()[i] ^ diff[i];
			const Xoodoo bInstance(4, 32, p);
			XoodooState bState(aInstance);
			bState.lanes = plnB;
			aInstance.permute(aState, 4);
			bInstance.permute(bState, 4);
			if ((aState.getBit(91)) == (bState.getBit(91)))
				sum++;
		}
		file << sum << endl;
	}
	file.close();
}

//void diffLin4RWithFix()
//{
//	ofstream file;
//	file.open("diffLin4RWithFix.txt");
//	vector<LaneValue> diff(12);
//	diff[0] = 0x101111;
//	diff[1] = 0x101110;
//	diff[2] = 0x101110;
//	diff[3] = 0x6466224;
//	diff[4] = 0x6466224;
//	diff[5] = 0x6466224;
//	diff[6] = 0x50454511;
//	diff[7] = 0x50454511;
//	diff[8] = 0x50454511;
//	diff[9] = 0x59ecf63b;
//	diff[10] = 0x59ecf63b;
//	diff[11] = 0x59ecf63b;
//
//	int fixedBits[14] = { 82,91,103,112,210,219,224,231,240,277,338,347,359,368 };
//
//	for (int testInd = 0; testInd < NCHECK; testInd++)
//	{
//		uint32_t sum = 0;
//		uint32_t fix = rand();
//		int howToFix[14] = {};
//		for (int maskBit1 = 0; maskBit1 < 14; maskBit1++)
//			howToFix[maskBit1] = ((fix / (1 << maskBit1)) % 2);
//		for (int plnInd = 0; plnInd < 2048; plnInd++)
//		{
//			vector<LaneValue> plnA(0);
//			plnA.resize(12);
//			vector<LaneValue> plnB(0);
//			plnB.resize(12);
//			for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
//				plnA[maskBit1] = rand() | (rand() << 15) | ((rand() % 4) << 30);
//
//			XoodooParameters p(2, 8, 5, 14, 11);
//			const Xoodoo aInstance(4, 32, p);
//			XoodooState aState(aInstance);
//			aState.lanes = plnA;
//			for (int maskBit1 = 0; maskBit1 < 14; maskBit1++)
//				aState.setBit(fixedBits[maskBit1], howToFix[maskBit1]);
//			for (int maskBit1 = 0; maskBit1 < 12; maskBit1++)
//				plnB[maskBit1] = aState.getLanes()[maskBit1] ^ diff[maskBit1];
//			const Xoodoo bInstance(4, 32, p);
//			XoodooState bState(aInstance);
//			bState.lanes = plnB;
//			aInstance.permute(aState, 4);
//			bInstance.permute(bState, 4);
//			if ((aState.getLanes()[0] & 1) == (bState.getLanes()[0] & 1))
//				sum++;
//		}
//		file << sum << endl;
//	}
//	file.close();
//}

void diffLin4R()
{
	ofstream file;
	//file.open("diffLin4R.txt");
	file.open("truncatedDiff3R.txt");
	vector<LaneValue> diff(12);
	diff[1] = 1;
	diff[2] = 1;
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	uint32_t sum = 0;
	//uint32_t sum[8] = {};
	for (int plnInd = 0; plnInd < NPLAINTEXT; plnInd++)
	{
		vector<LaneValue> plnA(12);
		vector<LaneValue> plnB(12);
		for (int i = 0; i < 12; i++)
		{
			plnA[i] = rand() | (rand() << 15) | ((rand() % 4) << 30);
			plnB[i] = plnA[i] ^ diff[i];
		}

		XoodooParameters p(2, 8, 5, 14, 11);
		const Xoodoo aInstance(4, 32, p);
		XoodooState aState(aInstance);
		aState.lanes = plnA;
		const Xoodoo bInstance(4, 32, p);
		XoodooState bState(bInstance);
		bState.lanes = plnB;
		aInstance.permute(aState, 4);
		bInstance.permute(bState, 4);

		//aInstance.inverseRhoEast(aState);
		//bInstance.inverseRhoEast(bState);
		//aInstance.theta(aState);
		//aInstance.rhoWest(aState);
		//aState.getLanes()[0] ^= rc[11];
		//bInstance.theta(bState);
		//bInstance.rhoWest(bState);
		//bState.getLanes()[0] ^= rc[11];
		//sum[(aState.getColumn(0, 14) ^ bState.getColumn(0, 14))]++;

		if ((aState.getBit(143)) ^ (bState.getBit(143))
			^ (aState.getBit(14)) ^ (bState.getBit(14)))
			sum++;

		//if ((aState.getBit(143)) == (bState.getBit(143)))
		//	sum++;
	}
	//for (int s = 0; s < 8; s++)
	//	file << sum[s] << endl;
	file << sum << endl;
	
	file.close();
}

void diffLin0to0Withdetails()
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	//ofstream resFile, aStateFile, bStateFile;
	vector<LaneValue> l(12);
	for (int plntxtInd = 0; plntxtInd < NPLAINTEXT; plntxtInd++)
	{
		//Create the instances
		XoodooParameters p(2, 8, 5, 14, 11);
		const Xoodoo aInstance(4, 32, p);
		XoodooState aState(aInstance);
		for (int j = 0; j < 12; j++)
			l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
		aState.lanes = l;

		const Xoodoo bInstance(4, 32, p);
		XoodooState bState(bInstance);
		bState.lanes = l;
		bState.invertBit(0, 0, 0);

		//Compute the intermediate values
		for (int i = 1 - 3; i <= 0; i++)
		{
			aInstance.round(aState, rc[i + 11]);
			bInstance.round(bState, rc[i + 11]);
			for (int j = 0; j < 12; j++)
				printf("%lx\n", (aState.getLanes()[j]) ^ (bState.getLanes()[j]));
			printf("\n");
		}
		printf("\n");
	}
}

void diffLinBitToItself()
{
	ofstream file;
	file.open("bestBiases3R.txt");
	vector<LaneValue> l(0);
	l.resize(12);

	for (int theBit = 0; theBit < 384; theBit++)
	{
		uint32_t bias[NCHECK] = {};
		for (int testInd = 0; testInd < NCHECK; testInd++)
		{
			for (int plntxtInd = 0; plntxtInd < NPLAINTEXT; plntxtInd++)
			{
				//Create the first instance
				XoodooParameters p(2, 8, 5, 14, 11);
				const Xoodoo aInstance(4, 32, p);
				XoodooState aState(aInstance);
				for (int j = 0; j < 12; j++)
					l[j] = (rand() | (rand() << 15) | ((rand() % 4) << 30));
				aState.lanes = l;

				//Compute the difference Instance
				int index = theBit;
				unsigned int z = index % 32;
				index /= 32;
				unsigned int x = index % 4;
				index /= 4;
				unsigned int y = index;
				const Xoodoo bInstance(4, 32, p);
				XoodooState bState(bInstance);
				bState.lanes = l;
				bState.invertBit(x, y, z);

				//Compute the ciphertexts and check the mask
				aInstance.permute(aState, 3);
				bInstance.permute(bState, 3);
				index = theBit;
				z = index % 32;
				index /= 32;
				x = index % 4;
				index /= 4;
				y = index;
				uint32_t mask = ((uint32_t)1 << z);
				bool parity = ((aState.getLane(x, y) & mask) == (bState.getLane(x, y) & mask));
				bias[testInd] += parity;
			}
		}
		uint32_t sum = 0;
		for (int i = 0; i < 100; i++)
			sum += bias[i];
		double avg = (((double)sum) / (100.0));
		double stdv = 0.;
		for (int i = 0; i < 100; i++)
			stdv += pow((double)bias[i] - avg, 2.);
		stdv = sqrt(stdv / 100.);
		file << avg << "\t" << stdv << endl;
	}
	file.close();
}

void beginningDiff()
{
	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};
	ofstream file;
	file.open("beginningDiff.txt");
	bool activeBits[128] = {};
	for (int i = 0; i < 128; i++)
		activeBits[i] = true;
	for (int diff1 = 0; diff1 < 4; diff1++)
	{
		for (int diff2 = 0; diff2 < 4; diff2++)
		{
			file << diff1 << "\t" << diff2 << endl;
			vector<LaneValue> diff(12);
			diff[0] = 1;
			diff[1] = diff1 % 2;
			diff[2] = diff1 / 2;
			diff[6] = ((uint32_t)(diff2 % 2) << 24);
			diff[7] = ((uint32_t)(diff2 / 2) << 24);
			diff[8] = 0x1000000;
			XoodooParameters p(2, 8, 5, 14, 11);
			Xoodoo temp(4, 32, p);
			XoodooState tempS(temp);
			tempS.lanes = diff;
			//tempS.getLanes()[0] ^= rc[7];
			temp.inverseRhoWest(tempS);
			temp.inverseTheta(tempS);
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 4; x++)
					file << std::hex << tempS.getLane(x, y) << "\t";
				file << endl;
			}
			int sumActives = 0;
			for (uint32_t i = 0; i < 128; i++)
			{
				if (tempS.getLanes()[(i / 32) * 3] & ((uint32_t)1 << (i % 32)))
					sumActives++;
				else
					activeBits[i] = false;
			}
			file << std::dec << sumActives << endl;
			file << endl;
		}
	}
	for (int i = 0; i < 128; i++)
		if (activeBits[i])
			file << std::dec << i << endl;
	file.close();
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	time_t ttt;
	srand((unsigned)time(&ttt));
	uint32_t pp = 0x7fffffff;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, pp - 1);
	//attack2Diffs();
	//beginningDiff();
	int sbox[8] = { 0,3,6,1,5,4,2,7 };
	int ddt[8][8] = {};
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			ddt[i ^ j][sbox[i] ^ sbox[j]]++;
		}
	}
	//ofstream file;
	//file.open("forSage.txt");
	//for (int i = 0; i < 320; i++)
	//	file << "v" << i << ", ";
	//for (int i = 0; i < 29; i++)
	//	file << "u" << i << ", ";
	//file.close();

	//Diff3RFor4R();
	//diffLin3R();

	//attack4RClasic();
	attack4RPNB();
	attack5RPNB();

	//for (int diffOption = 0; diffOption < 3; diffOption++)
	//{
	//	for (int diffBit = 0; diffBit < 128; diffBit++)
	//	{
	//		int pos1 = 3 * (diffBit / 32) + diffOption % 2;
	//		int pos2 = 3 * (diffBit / 32) + ((diffOption + 1) / 2) + 1;
	//		printf("%d\t%d\n", pos1, pos2);
	//	}
	//	printf("\n");
	//}

	//diffLin0to0Withdetails();
	//diffLin122(3);
	//diffLin122(4);
	//diffLin5RWithFix();
	//uint32_t goodFix[512] = {};
	//setGoodFix(goodFix);
	//attack();
	//specificDLCT();
	//diffLin2215r();
	//diffLin2214r();
	//detailsofaRound();
	//diffLin3R();
	//diffLin4RWithFix();
	//attack4R();
	//diffLin4R();

	//DL3Rfor6R();
	//bestThirdRDiff();

	//bool k[4096];
	//uint32_t f = rand() | ((rand() % 512) << 15);
	//keyPerFixed4R(k, f);
	//int sum = 0;
	//for (int maskBit1 = 0; maskBit1 < 4096; maskBit1++)
	//	sum += k[maskBit1];

	const LaneValue rc[12] = {
		0x00000058,
		0x00000038,
		0x000003C0,
		0x000000D0,
		0x00000120,
		0x00000014,
		0x00000060,
		0x0000002C,
		0x00000380,
		0x000000F0,
		0x000001A0,
		0x00000012
	};

	//ofstream file;
	///*file.open("relevantBits.txt");*/
	//file.open("rotDiffs.txt");
	//vector<LaneValue> l1(12);
	//for (int maskBit1 = 0; maskBit1 < 128; maskBit1++)
	//{
	//	file << "maskBit1= " << maskBit1 << endl;
	//	XoodooParameters p(2, 8, 5, 14, 11);
	//	Xoodoo temp(4, 32, p);
	//	XoodooState tempS(temp);
	//	tempS.lanes = l1;
	//	tempS.setBitToOne(maskBit1 / 32, 1, maskBit1 % 32);
	//	tempS.setBitToOne(maskBit1 / 32, 2, maskBit1 % 32);
	//	temp.theta(tempS);
	//	temp.rhoWest(tempS);
	//	for (int y = 0; y < 3; y++)
	//	{
	//		for (int x = 0; x < 4; x++)
	//			file << std::hex << tempS.getLane(x, y) << "\t";
	//		file << endl;
	//	}
	//	file << endl;
	//}

	vector<LaneValue> diff(12);
	diff[0] = 0xa8b23b19;
	diff[1] = 0xa8b23b18;
	diff[2] = 0xa8b23b18;
	diff[3] = 0x98810919;
	diff[4] = 0x98810919;
	diff[5] = 0x98810919;
	diff[6] = 0x52674513;
	diff[7] = 0x52674513;
	diff[8] = 0x52676513;
	diff[9] = 0x95a876f3;
	diff[10] = 0x95a876f3;
	diff[11] = 0x95a876f3;
	//ofstream file;
	//file.open("relevantBits.txt");
	vector<LaneValue> l1(12);
	XoodooParameters p(2, 8, 5, 14, 11);
	Xoodoo temp(4, 32, p);
	XoodooState tempS(temp);
	tempS.lanes = l1;
	tempS.setBitToOne(0, 1, 0);
	temp.theta(tempS);
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 4; x++)
			printf("%lx\t", tempS.getLane(x, y));
		printf("\n");
	}
	temp.rhoWest(tempS);
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 4; x++)
			printf("%lx\t", tempS.getLane(x, y));
		printf("\n");
	}
	////tempS.lanes = diff;
	////for (int maskBit1 = 0; maskBit1 < 384; maskBit1++)
	////{
	////	if (tempS.getBit(maskBit1))
	////		printf("%d\n", maskBit1);
	////}
	////tempS.setBitToOne(0, 0, 0);
	////tempS.setBitToOne(0, 2, 0);
	//temp.rhoEast(tempS);
	//temp.theta(tempS);
	//temp.rhoWest(tempS);
	//temp.rhoEast(tempS);
	//for (int maskBit1 = 0; maskBit1 < 384; maskBit1++)
	//{
	//	if (tempS.getBit(maskBit1))
	//		printf("%d\n", maskBit1);
	//}
	//for (int y = 0; y < 3; y++)
	//{
	//	for (int x = 0; x < 4; x++)
	//		printf("%lx\t", tempS.getLane(x, y));
	//	printf("\n");
	//}
	//tempS.lanes = l1;
	//temp.theta(tempS);
	//temp.rhoWest(tempS);
	//tempS.getLanes()[0] ^= rc[8];
	////printf("%d\n%d\n%d\n%d\n", tempS.getBit(0, 0, 11), tempS.getBit(0, 1, 11)
	////	, tempS.getBit(1, 0, 0), tempS.getBit(1, 2, 0));
	//for (int maskBit1 = 0; maskBit1 < 384; maskBit1++)
	//{
	//	vector<LaneValue> l2(12);
	//	tempS.lanes = l2;
	//	tempS.setBit(maskBit1, 1);
	//	temp.theta(tempS);
	//	temp.rhoWest(tempS);
	//	tempS.getLanes()[0] ^= rc[8];
	//	if ((tempS.getBit(0, 0, 11)) || (tempS.getBit(0, 1, 11)) ||
	//		(tempS.getBit(1, 0, 0)) || (tempS.getBit(1, 2, 0)))
	//	{
	//		file << "bit " << maskBit1 << endl;
	//		file << tempS.getBit(0, 0, 11) << endl;
	//		file << tempS.getBit(0, 1, 11) << endl;
	//		file << tempS.getBit(1, 0, 0) << endl;
	//		file << tempS.getBit(1, 2, 0) << endl;
	//		file << endl;
	//	}
	//}
	//file.close();

	//diffLin4R();
	//diffLin5RWithFix();

	return EXIT_SUCCESS;
}