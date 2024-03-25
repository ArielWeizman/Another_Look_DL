#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <fstream>
#include <bitset>
#include "des.h"
#include <math.h>
#include <cmath>
const uint32_t KEYTEST = 100;
const uint32_t TESTN = 9216;/*8704;*//*1528;*//*6144;*//*12288;*//*24576;*//*49152;*///4608;// 3072;// 6656;//9216;//7680;
//#define SBOXA /*3*/6 /*1*/
//#define LSBOX /*1*/ 5
using namespace std;

int SYCONSBOX[32] = { 8, 19, 30, 7, 6, 25, 16, 13, 22, 15, 3, 24, 17, 12, 4, 27,
					11, 0, 29, 20, 1, 14, 23, 26, 28, 21, 9, 2, 31, 18, 10, 5 };

int IP[64] = {	58,    50,   42,    34,    26,   18,    10,    2,
				60,    52,   44,    36,    28,   20,    12,    4,
				62,    54,   46,    38,    30,   22,    14,    6,
				64,    56,   48,    40,    32,   24,    16,    8,
				57,    49,   41,    33,    25,   17,     9,    1,
				59,    51,   43,    35,    27,   19,    11,    3,
				61,    53,   45,    37,    29,   21,    13,    5,
				63,    55,   47,    39,    31,   23,    15,    7};

int IPR[64] = {	40,     8,   48,    16,    56,   24,    64,   32,
				39,     7,   47,    15,    55,   23,    63,   31,
				38,     6,   46,    14,    54,   22,    62,   30,
				37,     5,   45,    13,    53,   21,    61,   29,
				36,     4,   44,    12,    52,   20,    60,   28,
				35,     3,   43,    11,    51,   19,    59,   27,
				34,     2,   42,    10,    50,   18,    58,   26,
				33,     1,   41,     9,    49,   17,    57,   25};

static int Ebox[48] =	{32, 1, 2, 3, 4, 5,
						4, 5, 6, 7, 8, 9,
						8, 9, 10, 11, 12, 13,
						12,	13,	14,	15,	16,	17,
						16,	17,	18,	19,	20,	21,
						20,	21,	22,	23,	24,	25,
						24,	25,	26,	27,	28,	29,
						28,	29,	30,	31,	32,	1};

static int per[32] =	{16, 7 ,20  ,21,
						29 ,12 , 28 , 17,
						1 , 15,  23,  26,
						5, 18, 31, 10,
						2, 8, 24, 14,
						32, 27, 3, 9,
						19, 13, 30, 6,
						22, 11, 4, 25};

const int sb[8][4][16]=
{
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
	    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},


	{
	    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
	    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
	    {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
	    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
	    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
	    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};

uint64_t ipow(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
		base *= base;
    }

    return result;
}

static int ddt[8][64][16];

// Initialize the ddts:
static void ddtInit()
{
	for(int sNum = 0; sNum < 8; sNum++)
	{
		//Init all the entires to zero
		for(int in = 0; in < 64; in++) 
		{
		  for(int out = 0; out < 16; out++)
		  {
		    ddt[sNum][in][out] = 0;
		  }
		}
		
		// this makes us go through all the possible value of p1
		for(int p1 = 0; p1 < 64; p1++) 
		{
			//Compute the value of p1 after the sbox
			int p1Row = (p1 % 2) + 2*(p1/32);
			int p1Col = 0;
			int p1Temp = p1/2;
			p1Col += p1Temp % 2;
			int power = 2;
			for (int i=0; i<3; i++)
			{
				p1Temp /= 2;
				int bit = p1Temp % 2;
				p1Col += power*bit;
				power *= 2;
			}
			int sresp1 = sb[sNum][p1Row][p1Col];

		  // this makes us go through all the possible value of p2
		  for(int p2 = 0; p2 < 64; p2++)
		  {
		    int XOR_IN = p1 ^ p2;
						
			int p2Row = (p2 % 2) + 2*(p2/32);
			int p2Col = 0;
			int p2Temp = p2/2;
			p2Col += p2Temp % 2;
			power = 2;
			for (int i=0; i<3; i++)
			{
				p2Temp /= 2;
				int bit = p2Temp % 2;
				p2Col += power*bit;
				power *= 2;
			}
			int sresp2 = sb[sNum][p2Row][p2Col];

		    int XOR_OUT = sresp1 ^ sresp2;

		    ddt[sNum][XOR_IN][XOR_OUT]++;
		  }
		}
	}
}

static int serpentDDTS2[16][16];

static void serpentDDTS2Init()
{
	//Init all the entires to zero
	for (int in = 0; in < 16; in++)
	{
		for (int out = 0; out < 16; out++)
		{
			serpentDDTS2[in][out] = 0;
		}
	}

	// this makes us go through all the possible value of p1
	for (int p1 = 0; p1 < 16; p1++)
	{
		int sresp1 = sb[0][2][p1];

		// this makes us go through all the possible value of p2
		for (int p2 = 0; p2 < 16; p2++)
		{
			int XOR_IN = p1 ^ p2;

			int sresp2 = sb[0][2][p2];

			int XOR_OUT = sresp1 ^ sresp2;

			serpentDDTS2[XOR_IN][XOR_OUT]++;
		}
	}
}

bool parity(uint64_t x)
{
	uint64_t count = 0, b = 1;
	
	for(int i = 0; i < 64; i++)
	{
	    if( x & (b << i) )
			count++;
	}
	
	if (count % 2)
		return true;
	
	return false;
}

static int lat[8][64][16] = {};

void latInit()
{
	for (int sbox = 0; sbox < 8; sbox++)
	{
		for(int x_i = 0; x_i < 64; x_i++)
		{
		    for(int y_i = 0; y_i < 16; y_i++)
			{
		        for(int x_j = 0; x_j < 64; x_j++)
				{
					int x_j_row = (x_j % 2) + 2*(x_j / 32);
					int x_j_col = 0;
					int x_jT = x_j/2;
					for (int t = 0; t < 4; t++)
					{
						x_j_col += ipow(2, t)*(x_jT % 2);
						x_jT /= 2;
					}
		            int y_j = sb[sbox][x_j_row][x_j_col];
		            y_j &= y_i;
		            int temp = x_j & x_i;
		            int parity_11 = (parity(temp) + parity(y_j)) % 2;
		            lat[sbox][x_i][y_i] += parity_11;
				}
			}
		}
	}
	for (int sb = 0; sb < 8; sb++)
		for (int b4l = 0; b4l < 64; b4l++)
			for (int afl = 0; afl < 16; afl++)
				lat[sb][b4l][afl] = 32 - lat[sb][b4l][afl];
}

static int dlct[8][64][16] = {};

void dlctInit()
{
	for (int sbox = 0; sbox < 8; sbox++)
	{
		for(int x_i = 0; x_i < 64; x_i++)
		{
			int x_iRow = (x_i % 2) + 2*(x_i / 32);
			int x_iCol = 0;
			int x_iT = x_i/2;
			for (int t = 0; t < 4; t++)
			{
				x_iCol += ipow(2, t)*(x_iT % 2);
				x_iT /= 2;
			}
			int sx_i = sb[sbox][x_iRow][x_iCol];

		    for(int x_j = 0; x_j < 64; x_j++)
			{
				int xorin = x_i ^ x_j;

				int x_j_row = (x_j % 2) + 2*(x_j / 32);
				int x_j_col = 0;
				int x_jT = x_j/2;
				for (int t = 0; t < 4; t++)
				{
					x_j_col += ipow(2, t)*(x_jT % 2);
					x_jT /= 2;
				}
		        int sx_j = sb[sbox][x_j_row][x_j_col];

				for (int mask = 0; mask < 16; mask++)
				{
					if (!(parity(sx_i & mask) ^ parity(sx_j & mask)))
						dlct[sbox][xorin][mask]++;
				}
			}
		}
	}
}


//string hex2bin(string p)//hexadecimal to binary
//{
//	string ap="";
//	int l=p.length();
//	for(int KExp=0;KExp<l;KExp++)
//	{
//		string st="";
//		if(p[KExp]>='0'&&p[KExp]<='9')
//		{
//			int te=int(p[KExp])-48;
//			while(te>0)
//			{
//				st+=char(te%2+48);
//				te/=2;
//			}
//			while(st.length()!=4)
//				st+='0';
//			for(int j=3;j>=0;j--)
//				ap+=st[j];
//		}
//		else
//		{
//			int te=p[KExp]-'A'+10;
//			while(te>0)
//			{
//				st+=char(te%2+48);
//				te/=2;
//			}
//			for(int j=3;j>=0;j--)
//				ap+=st[j];
//		}
//	}
//	return ap;
//}

static uint64_t strToInt(std::string &str, int numBits)
{
	uint64_t result = 0;
	for(int i = 0; i < numBits; i++)
	{
		result = result<<1;
		unsigned int digit = str[i]-'0';
		//if (str[KExp]<58) digit = str[KExp]-'0';
		//else
		//	digit = 10+str[KExp]-'A';
		result ^= digit;
	}
	return result;
}

void int2BinS (uint64_t n, int numBits, std::string &s)
{
	//std::string sb;
	s.resize(numBits);
	for (int i = 0; i < numBits; i++)
	{
		uint64_t temp = n%2;
		char a = (char)(48+temp);
		//sb.insert(sb.begin(),a);
		s[numBits-i-1] = a;
		n /= 2;
	}
	//return sb;
}

void prtInt64AsHex (uint64_t n, char* ar)
{
	uint64_t temp = n;
	for (int i = 0; i < 16; i++)
	{
		char a;
		int val = temp % 16;
		if (val < 10)
			a = (char)(48+val);
		else
			a = (char)(55+val);
		ar[15 - i] = a;
		temp /= 16;
	}
}

void LDiffTo8SDiffs (unsigned long long int diff, int sDiffIn[8])
{
	std::string diffstr;
	int2BinS(diff, 32, diffstr);
	std::string ep;
	// The Expansion box Ebox
	ep.resize(48);
	for (int i=0;i<48;i++)
	{
	   ep[i] = diffstr[Ebox[i]-1];
	}
	for (int i = 0; i < 8; i++)
	{
		sDiffIn[i] = strToInt(ep.substr(6*i,6),6);
	}
}

//struct sOutput
//{
//	int out;
//	int nEvents;
//}; typedef sOutput sOutput;

void beforedlct(double state[64], uint64_t thediff, int diffActiveSB, int linActiveSB)
{
	ddtInit();
	int sDiffIn[8];
	
	LDiffTo8SDiffs(thediff, sDiffIn);
	int snDiffOut[8][16];
	for (int i = 0; i < 8; i++)
		for (int k = 0; k < 16; k++)
			snDiffOut[i][k] = ddt[i][sDiffIn[i]][k];

	int sDiffIn2[8];
	int snDiffOut2[8][16];
	int oops = 0;
	for (uint64_t round1out = 0; round1out < 16; round1out++)
	{
		if (snDiffOut[diffActiveSB][round1out]==0)
			continue;
		double round1Prob = (double)(snDiffOut[diffActiveSB][round1out])/64.0;
		uint64_t r1Diff = round1out<<(7 - diffActiveSB)*4;
		std::string diffstr;
		int2BinS(r1Diff, 32, diffstr);
		std::string pc;
		pc.resize(32);
		for(int i=0;i<32;i++)
			pc[i]=diffstr[per[i]-1];
		LDiffTo8SDiffs(strToInt(pc, 32), sDiffIn2);
		for (int sb = 0; sb < 8; sb++)
		{
			for (int k = 0; k < 16; k++)
			{
				snDiffOut2[sb][k] = ddt[sb][sDiffIn2[sb]][k];
			}
		}

		//For the specific experiment
		int notActiveSB[2] = {};
		switch (diffActiveSB)
		{
		case 0:
			notActiveSB[0] = 0;
			notActiveSB[1] = 6;
			break;
		case 1:
			notActiveSB[0] = 1;
			notActiveSB[1] = 5;
			break;
		case 2:
			notActiveSB[0] = 0;
			notActiveSB[1] = 2;
			break;
		case 3:
			notActiveSB[0] = 1;
			notActiveSB[1] = 3;
			break;
		case 4:
			notActiveSB[0] = 4;
			notActiveSB[1] = 7;
			break;
		case 5:
			notActiveSB[0] = 3;
			notActiveSB[1] = 5;
			break;
		case 6:
			notActiveSB[0] = 4;
			notActiveSB[1] = 6;
			break;
		case 7:
			notActiveSB[0] = 2;
			notActiveSB[1] = 7;
			break;
		default:
			break;
		}
		for (uint64_t i = 0; i < 16777216; i++)
		{
			bool possible=true;
			double round2Prob = 1.0;
			uint64_t sout = 0;
			uint64_t temp = i;
			for (int sb=7;sb>=0;sb--)
			{
				if (sb == notActiveSB[0] || sb == notActiveSB[1]/*sb==0||sb==6*/ /*sb==3||sb==5*/)
					continue;
				if (snDiffOut2[sb][temp%16]==0)
				{
					possible=false;
					break;
				}
				round2Prob *= ((double)(snDiffOut2[sb][temp%16])/64.0);
				sout += (temp%16)<<4*(7-sb);
				temp/=16;
			}
			if(!possible)
				continue;
			round2Prob *= round1Prob;
			int mpos = 0;
			int2BinS(sout, 32, diffstr);
			//if(diffstr[per[16]-1]-48)
			//	int oops=1;
			for(int t=0;t<6;t++)
				mpos += ipow(2, 5 - t)*((int)diffstr[per[((linActiveSB) * 4 - 1 + t + 32) % 32] - 1] - 48);
			state[mpos] += round2Prob;
		}
	}
}

double dlctProb (double stateProb[64], int linActiveSB)
{
	double prob = 0.;
	int dlct[64];
	for (int i=0;i<64;i++)
		dlct[i]=0;
	for (int p1 = 0; p1 < 64; p1++)
	{
		int row = (p1 % 2) +2*(p1/32);
		int col = 0;
		int p1Temp=p1/2;
		for (int c=0;c<4;c++)
		{
			col += ipow(2,c)*(p1Temp%2);
			p1Temp /= 2;
		}
		int temp=sb[linActiveSB][row][col];
		bool p1MP = false;	//the mask parity of the sbox output of p1
		for (int a = 0; a < 4; a++)
		{
			p1MP ^= (temp % 2);
			temp/=2;
		}
		
		for (int p2 = 0; p2 < 64; p2++)
		{
			int xorin = p1 ^ p2;
			row = (p2 % 2) +2*(p2/32);
			col = 0;
			int p2Temp=p2/2;
			for (int c=0;c<4;c++)
			{
				col += ipow(2,c)*(p2Temp%2);
				p2Temp /= 2;
			}
			temp=sb[linActiveSB][row][col];
			bool p2MP = false;	//the mask parity of the sbox output of p2
			for (int a = 0; a < 4; a++)
			{
				p2MP ^= (temp % 2);
				temp/=2;
			}
			if (p1MP == p2MP)
				dlct[xorin]++;
		}
	}
	for (int diff = 0; diff < 64; diff++)
		prob += ((double)(dlct[diff])/64.)*stateProb[diff];
	return prob;
}

void keySchedule(unsigned char key[16][6], string &kp)
{
	int key1[64];

	for (int i = 0; i<64; i++)
		key1[i] = kp[i] - '0';
	//getting 56 bit key from 64 bit using the parity bits
	int keyp[56] = { 57  , 49   , 41   ,33 ,   25   , 17 ,   9,
		1 ,  58    ,50  , 42 ,   34  ,  26  , 18,
		10 ,   2   , 59 ,  51 ,  43  ,  35  , 27,
		19  , 11   ,  3  , 60  ,  52  ,  44  , 36,
		63  , 55  ,  47  , 39  ,  31   , 23  , 15,
		7  , 62   , 54  , 46  ,  38  ,  30  , 22,
		14  ,  6   , 61  , 53  ,  45   , 37  , 29,
		21  , 13  ,   5  , 28   , 20  ,  12  ,  4 };
	//D box configuration each 28 to 24 bit
	int key2[48] = { 14,    17,   11,    24,     1,    5,
		3   , 28 ,  15    , 6  ,  21  , 10 ,
		23    ,19  , 12   ,  4 ,  26  ,  8,
		16     ,7  , 27  ,  20  ,  13  ,  2,
		41   , 52 ,  31  ,  37  ,  47  , 55,
		30   , 40  , 51  ,  45  ,  33 , 48,
		44    ,49 ,  39  ,  56  ,  34 ,  53,
		46    ,42 ,  50  ,  36 ,   29 ,  32 };
	int keyl[28], keyr[28], nshift, temp1, temp2, pkey[56];

	//key generation
	//int t = 1, j, row, col, temp, round = nRounds;
	//j = 0;
	for (int i = 0; i < 56; i++)
		pkey[i] = key1[keyp[i] - 1];
	for (int i = 0; i < 28; i++)
		keyl[i] = pkey[i];
	for (int i = 0; i < 28; i++)
		keyr[i] = pkey[i + 28];
	//round key generation
	for (int i = 0; i < 16; i++)
	{
		if (i == 0 || i == 1 || i == 8 || i == 15)
			nshift = 1;
		else
			nshift = 2;
		while (nshift--)
		{
			temp1 = keyl[0];
			temp2 = keyr[0];
			for (int j = 0; j < 27; j++)
			{
				keyl[j] = keyl[j + 1];
				keyr[j] = keyr[j + 1];
			}
			keyl[27] = temp1;
			keyr[27] = temp2;
		}
		for (int j = 0; j < 3; j++)
		{
			uint8_t subkey = 0;
			for (int tt = 0; tt < 8; tt++)
				subkey |= ((keyl[key2[tt + 8 * j] - 1] << (7 - tt)));
			key[i][j] = subkey;
		}
		for (int j = 3; j < 6; j++)
		{
			uint8_t subkey = 0;
			for (int tt = 0; tt < 8; tt++)
				subkey |= ((keyr[key2[tt + 8 * j] - 1 - 28] << (7 - tt)));
			key[i][j] = subkey;
		}
	}
}

string des(string &kp, string &p, int nRounds)
{
	std::string l,r,ap="",ke,rtem, afterRounds;
    int key1[64];

		for(int i=0;i<64;i++)
		    key1[i]=kp[i]-'0';
		//getting 56 bit key from 64 bit using the parity bits
		int keyp[56]={57  , 49   , 41   ,33 ,   25   , 17 ,   9,
		       1 ,  58    ,50  , 42 ,   34  ,  26  , 18,
		      10 ,   2   , 59 ,  51 ,  43  ,  35  , 27,
		      19  , 11   ,  3  , 60  ,  52  ,  44  , 36,
		      63  , 55  ,  47  , 39  ,  31   , 23  , 15,
		       7  , 62   , 54  , 46  ,  38  ,  30  , 22,
		      14  ,  6   , 61  , 53  ,  45   , 37  , 29,
		      21  , 13  ,   5  , 28   , 20  ,  12  ,  4};
		//D box configuration each 28 to 24 bit
		int key2[48]={14,    17,   11,    24,     1,    5,
		3   , 28 ,  15    , 6  ,  21  , 10 ,
		23    ,19  , 12   ,  4 ,  26  ,  8,
		16     ,7  , 27  ,  20  ,  13  ,  2,
		41   , 52 ,  31  ,  37  ,  47  , 55,
		30   , 40  , 51  ,  45  ,  33 , 48,
		44    ,49 ,  39  ,  56  ,  34 ,  53,
		46    ,42 ,  50  ,  36 ,   29 ,  32};
		int key[16][48],keyl[28],keyr[28],nshift,temp1,temp2,pkey[56];

		//key generation
		int t=1,j,row,col,temp,round=nRounds;
		j=0;
		for(int i=0;i<56;i++)
		    pkey[i]=key1[keyp[i]-1];
		for(int i=0;i<28;i++)
		    keyl[i]=pkey[i];
		for(int i=0;i<28;i++)
		    keyr[i]=pkey[i+28];
		//round key generation
		for(int i=0;i<16;i++)
		{

		    if(i==0||i==1||i==8||i==15)
		        nshift=1;
		    else
		        nshift=2;
		    while(nshift--)
		    {
		        temp1=keyl[0];
			    temp2=keyr[0];
			    for(j=0;j<27;j++)
			    {
			        keyl[j]=keyl[j+1];
			        keyr[j]=keyr[j+1];
			    }
			    keyl[27]=temp1;
			    keyr[27]=temp2;
			}
			for(j=0;j<24;j++)
			    key[i][j]=keyl[key2[j]-1];
			for(j=24;j<48;j++)
			    key[i][j]=keyr[key2[j]-1-28];
		}

		

			l=p.substr(0,32);
			r=p.substr(32,32);
			//DES Encryption
			//std::string output;
			bool difflin=false;
			while(round--)
			{
			   rtem=r;
			   t=1;
			   std::string ep="",xorout="",sout="";
			   // The Expansion box Ebox
			   ep.resize(48);
			   for (int i=0;i<48;i++)
			   {
				   ep[i] = r[Ebox[i]-1];
			   }
			   ////the expansion P box
			   //ep+=r[31];
			   //for(KExp=0;KExp<32;KExp++)
			   //{
			   //    if((t+1)%6==0)
			   //    {
					 //  ep+=r[4*((t+1)/6)];
			   //        t++;
			   //    }
			   //    if(t%6==0&&KExp!=0)
			   //    {
			   //        ep+=r[4*(t/6)-1];
			   //        t++;
			   //     }
			   //    ep+=r[KExp];
			   //    t++;
			   //}
			   //ep+=r[0];
			   //Key xor with output of expansion p box
			   for(int i=0;i<48;i++)
			       xorout+=char(((int(ep[i])-48)^key[16-round-1][i])+48);
			   //sbox compression 48bit to 32 bit
			   int pos = 0;
			   for(int i=0;i<48;i+=6)
			   {
					row=(int(xorout[i+5])-48)+(int(xorout[i])-48)*2;
					col= (int(xorout[i+1])-48)*8+(int(xorout[i+2])-48)*4+(int(xorout[i+3])-48)*2+(int(xorout[i+4])-48);
					temp=sb[i/6][row][col];
					for (int a = 0; a < 4; a++)
					{
						sout.insert(sout.begin()+pos, char(temp%2+48));
						temp/=2;
					}
					pos += 4;
					//while(temp>0)
					//{
					//    soutt+=char(temp%2+48);
					//    temp/=2;
					//}
					//while(soutt.length()!=4)
					//    soutt+='0';
					//for(j=soutt.length()-1;j>=0;j--)
					//    sout+=soutt[j];
				}
			   //straight pbox that is permutation of the sbox output
			   if (round)
			   {
				char pc[32];
				for(int i=0;i<32;i++)
				   pc[i]=sout[per[i]-1];
			   
				r="";
				for(int i=0;i<32;i++)
				   r+=char(((int(pc[i])-48)^(int(l[i])-48))+48);
			   }
			   else
				   r=sout;
				l=rtem;
			
			//cout<<"Output after Round"<<16-round<<endl;


			   if(!round)
			   {
				   afterRounds.resize(64);
				   for(int i = 0; i < 32; i++)
				   {
					   afterRounds[i] = r[i];
					   afterRounds[32+i] = l[i];
				   }
			   }

			   
			   //if (round == 15)
			   //{
				  // if (testN % 2)
				  // {
					 //  r11=strToInt(r,32);
					 //  if(r10==r11)
					 //  {
						//   count1++;
						//   difflin = true;
					 //  }
					 //  //bool r1check = true;
					 //  //for (int KExp = 0; KExp < 32; KExp++)
					 //  //{
						//  // if(r[KExp]!=rprev[KExp])
						//	 //  r1check=false;
					 //  //}
					 //  //if (r1check)
						//  // count2++;
					 //  //if(!r.compare(rprev))
						//  // count1++;
				  // }
				  // else
				  // {
					 //  //for (int iii=0;iii<32;iii++)
						//  // rprev[iii]=r[iii];
					 //  r10=strToInt(r,32);
				  // }
				  // //round = 16;
				  // //break;
			   //}
			   //if (round == 11)
			   //{
				  // if (testN % 2)
				  // {
					 //  stest2 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
					 //  if (stest1 == stest2)
					 //  {
						//   count++;
						//   //if(difflin)
						//	  // count2++;
					 //  }
				  // }
				  // else
					 //  stest1 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
				  // //round = 16;
				  // //break;
			   //}

			   //if (round == 9)
			   //{
				  // if (testN % 2)
				  // {
					 //  stest2 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
					 //  if (stest1 == stest2)
					 //  {
						//   count2++;
						//   //if(difflin)
						//	  // count2++;
					 //  }
				  // }
				  // else
					 //  stest1 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
				  // round = 16;
				  // break;
			   //}

			   //if(!round)
				  // cout<<cip<<endl;
			}

			//output.resize(64);
			//for(int KExp = 0; KExp < 64; KExp++)
			//	output[KExp] = afterRounds[IPR[KExp]-1];

			//string cip="";
			//for(int KExp=0;KExp<64;KExp+=4)
			//{
			//    int te;
			//    te=(int(output[KExp])-48)*8+(int(output[KExp+1])-48)*4+(int(output[KExp+2])-48)*2+(int(output[KExp+3])-48);
			//    if(te<10)
			//        cip+=char(te+48);
			//    else
			//        cip+=char(te+55);
			//}
			////for(int KExp=0;KExp<32;KExp+=4)
			////{
			////    int te;
			////    te=(int(r[KExp])-48)*8+(int(r[KExp+1])-48)*4+(int(r[KExp+2])-48)*2+(int(r[KExp+3])-48);
			////    if(te<10)
			////        cip+=char(te+48);
			////    else
			////        cip+=char(te+55);
			////}
			//system("pause");



//test case
/*
PLAIN=
14A7D67818CA18AD
KEY=
AABB09182736CCDD
*/

    //DES decryption
		//round=16;
		//string ltem;
		//while(round--)
		//{
		//    ltem=l;
		//    t=1;
		//    string ep="",xorout="",sout="",soutt;
		//    //the expansion P box
		//    ep+=l[31];
		//    for(KExp=0;KExp<32;KExp++)
		//    {
		//        if((t+1)%6==0)
		//        {
		//            ep+=l[4*((t+1)/6)];
		//            t++;
		//        }
		//        if(t%6==0&&KExp!=0)
		//        {
		//            ep+=l[4*(t/6)-1];
		//            t++;
		//        }
		//        ep+=l[KExp];
		//        t++;
		//    }
		//    ep+=l[0];
		//    //Key xor with output of expansion p box
		//    for(KExp=0;KExp<48;KExp++)
		//        xorout+=char(((int(ep[KExp])-48)^key[round][KExp])+48);
		//    //sbox compression 48bit to 32 bit
		//    for(KExp=0;KExp<48;KExp+=6)
		//    {
		//        row=(int(xorout[KExp+5])-48)+(int(xorout[KExp])-48)*2;
		//        col= (int(xorout[KExp+1])-48)*8+(int(xorout[KExp+2])-48)*4+(int(xorout[KExp+3])-48)*2+(int(xorout[KExp+4])-48);
		//        temp=sb[KExp/6][row][col];
		//        soutt="";
		//        while(temp>0)
		//        {
		//            soutt+=char(temp%2+48);
		//            temp/=2;
		//        }
		//        while(soutt.length()!=4)
		//            soutt+='0';
		//        for(j=soutt.length()-1;j>=0;j--)
		//            sout+=soutt[j];
		//    }
		//    //straight pbox that is permutation of the sbox output
		//    char pc[32];
		//    for(KExp=0;KExp<32;KExp++)
		//        pc[KExp]=sout[per[KExp]-1];
		//    l="";
		//    for(KExp=0;KExp<32;KExp++)
		//        l+=char(((int(pc[KExp])-48)^(int(r[KExp])-48))+48);
		//    r=ltem;
		//    cout<<"Decrypted Output after Round"<<16-round<<endl;
		//    string cip="";
		//    for(KExp=0;KExp<32;KExp+=4)
		//    {
		//        int te;
		//        te=(int(l[KExp])-48)*8+(int(l[KExp+1])-48)*4+(int(l[KExp+2])-48)*2+(int(l[KExp+3])-48);
		//        if(te<10)
		//            cip+=char(te+48);
		//        else
		//            cip+=char(te+55);
		//    }
		//    for(KExp=0;KExp<32;KExp+=4)
		//    {
		//        int te;
		//        te=(int(r[KExp])-48)*8+(int(r[KExp+1])-48)*4+(int(r[KExp+2])-48)*2+(int(r[KExp+3])-48);
		//        if(te<10)
		//            cip+=char(te+48);
		//        else
		//            cip+=char(te+55);
		//    }
		//    //cout<<cip<<endl;
		//}
        //system("pause");
	return afterRounds;
}

void cmptFullDiff(uint64_t halfdiff, uint64_t *diff, int sbox)	//Compute all the possible diffs after the active sbox
{
	uint64_t rdiff = halfdiff << (4 * (8 - sbox));
	uint64_t ldiff = 0;
	for (uint64_t i = 0; i < 16; i++)
	{
		uint64_t indiff = halfdiff << 1;
		if (ddt[sbox-1][indiff][i] < 2)
		{
			diff[i] = 0xffffffffffffffff;	//This diff is not relevant!!
			continue;
		}
		unsigned char aftrs[4];
		for (int pos = 0; pos < 4; pos++)
		{
			if (pos==(sbox-1)/2)
			{
				if (sbox % 2)
					aftrs[pos] = i << 4;
				else
					aftrs[pos] = i;
			}
			else
			{
				aftrs[pos] = 0;
			}
		}
		unsigned char aftrp[4];
		for (int i = 0; i < 4; i++)
			aftrp[i] = 0;
		for (int j = 0; j<32; j++)
		{
			int shift_size = per[j];
			unsigned char shift_byte = 0x80 >> ((shift_size - 1) % 8);
			shift_byte &= aftrs[(shift_size - 1) / 8];
			shift_byte <<= ((shift_size - 1) % 8);

			aftrp[j / 8] |= (shift_byte >> j % 8);
		}

		ldiff = 0;
		for (int j = 0; j < 4; j++)
			ldiff += (uint64_t)(aftrp[j]) << 8 * (3 - j);
		diff[i] = (ldiff << 32) + rdiff;
	}
}

void cmptFullMask(uint64_t halfmask, int sbox, uint64_t *fullmask)
{
	uint32_t rmask = (halfmask << (4 * (6 - sbox) + 3)) | (halfmask >> (32 - (4 * (6 - sbox) + 3)));
	for (uint64_t maskOut = 0; maskOut < 16; maskOut++)
	{
		//if (abs(lat[sbox][halfmask][maskOut]) < 10)
		//{
		//	fullmask[maskOut] = 0xffffffffffffffff;
		//	continue;
		//}
		
		unsigned char b4p[4];
		for (int pos = 0; pos < 4; pos++)
		{
			if (pos == (sbox / 2))
			{
				if (sbox % 2)
					b4p[pos] = maskOut;
				else
					b4p[pos] = maskOut << 4;
			}
			else
			{
				b4p[pos] = 0;
			}
		}
		unsigned char aftrp[4];
		for (int i = 0; i < 4; i++)
			aftrp[i] = 0;
		for (int j = 0; j<32; j++)
		{
			int shift_size = per[j];
			unsigned char shift_byte = 0x80 >> ((shift_size - 1) % 8);
			shift_byte &= b4p[(shift_size - 1) / 8];
			shift_byte <<= ((shift_size - 1) % 8);

			aftrp[j / 8] |= (shift_byte >> j % 8);
		}

		fullmask[maskOut] = 0;
		for (int i = 0; i < 4; i++)
			fullmask[maskOut] |= ((uint64_t(aftrp[i])) << 8 * (7 - i));
		fullmask[maskOut] += rmask;
	}
}

double dlct3roundsConLin(double q, uint64_t rDiff, double firstProb, bool useFirstProb,
	int diffActiveSB, int linActiveSB)
{
	double state[64];
	for (int i = 0; i<64; i++)
		state[i] = 0.;
	beforedlct(state, rDiff, diffActiveSB, linActiveSB);

	double sum = 0.;
	for (int i = 0; i<64; i++)
		sum += state[i];

	double prob = dlctProb(state, linActiveSB);
	double pr = prob * firstProb + 0.5*(1. - firstProb);
	//firstProb is the probability of the first round of the.
	//In case of structures we don't consider it.
	//if(useFirstProb)
	//	return 2.*(2 * pr - 1)*q*q;
	//else
	//	return 2.*(2 * prob - 1)*q*q;
	if (useFirstProb)
		return 4.*firstProb*(prob-0.5)*q*q;
	else
		return 4.*(prob - 0.5)*q*q;
}

//static int NEx = 0;
static int NExp = 0;
static int Nss = 0;
static int Nsb = 0;
static int m1 = 0, m2 = 0, m1m2 = 0;

void specificOutTest()
{
	ofstream file;
	file.open("specificOutTest.txt");

	uint32_t lDiff = 0x00808000;
	uint32_t rDiff = 0x60000000;
	uint64_t mask = 0x2104008000008000;

	unsigned char input[8];
	unsigned char inputDiff[8];
	unsigned char output[8];
	unsigned char outputDiff[8];;

	for (int KExp = 0; KExp < 100; KExp++)
	{
		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);
		//Force the ket bits entering to S1 tobe zero, ensuring the specific in values
		uint8_t zeroKey = 0;
		keyt[0][0] = zeroKey;

		int counter = 0;
		for (int NExp = 0; NExp < TESTN; NExp++)
		{
			// Create value with specific in value to S1 = 10_x
			uint32_t rVal = (uint32_t)(rand() % 256);
			//rVal &= 0xfe;
			for (int i = 0; i < 4; i++)
				rVal |= ((uint32_t)(rand() % 256) << (8 * i));
			//uint64_t temp = (uint64_t)(rand() % 8) + 0x80;
			//rVal |= (temp << 24);
			uint32_t lVal;
			for (int i = 0; i < 4; i++)
				lVal |= ((uint32_t)(rand() % 256) << (8 * (i + 4)));
			//uint64_t plain = lVal | rVal;
			//uint64_t plainDiff = plain ^ diff;
			uint32_t rvalDiff = rVal ^ rDiff;;
			uint32_t lvalDiff = lVal ^ lDiff;

			//printf("%x\n", rVal);

			for (int posi = 0; posi < 4; posi++)
			{
				input[7 - posi] = rVal % 256;
				inputDiff[7 - posi] = rVal % 256;
				rVal /= 256;
			}

			for (int posi = 0; posi < 4; posi++)
			{
				input[3 - posi] = lVal % 256;
				inputDiff[3 - posi] = lVal % 256;
				lVal /= 256;
			}

			input[4] &= 7;
			input[4] |= 0xd8;
			input[7] &= 0xfe;
			inputDiff[4] &= 7;
			inputDiff[4] |= 0xb8;
			inputDiff[7] &= 0xfe;
			inputDiff[1] ^= 0x80;
			inputDiff[2] ^= 0x82;

			//for (int keyInd = 0; keyInd < 8; keyInd++)
			//{
			//	printf("%x", input[keyInd]);
			//}
			//cout << endl;
			//for (int keyInd = 0; keyInd < 8; keyInd++)
			//{
			//	printf("%x", inputDiff[keyInd]);
			//}
			//cout << endl;

			process_message(input, output, keyt, 7);
			process_message(inputDiff, outputDiff, keyt, 7);
			uint64_t outp = 0;
			uint64_t outpDiff = 0;
			for (int ii = 0; ii < 8; ii++)
			{
				outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
				outpDiff |= (uint64_t(outputDiff[ii]) << (8 * (7 - ii)));
			}
			bool plainParity = parity(outp & mask);
			bool plainDiffParity = parity(outpDiff&mask);
			if(plainParity==plainDiffParity)
				counter++;
		}
		file << counter << endl;
	}
	file.close();
}

bool maskParity[64][TESTN];
int keyRec9RStrctrTest()
{
	uint64_t mask = 0x21040080;
	uint64_t outDiffS6[9] = { 11, 14, 3, 6, 10, 15, 9, 13, 12 };
	uint64_t outDiffS8[10] = { 7, 12, 10, 14, 6, 9, 5, 11, 13, 3 };
	uint64_t diff[9] = {};
	diff[3] = 0x1000000000000000;
	diff[2] = 0x0000000800000000;
	diff[1] = 0x0020000000000000;
	diff[0] = 0x0000200000000000;
	diff[7] = 0x0800000000000000;
	diff[6] = 0x0000002000000000;
	diff[5] = 0x0002000000000000;
	diff[4] = 0x0000080000000000;
	diff[8] = 0x4000000000000202;

	int counter = 0;

	for (int keyInd = 0; keyInd < KEYTEST; keyInd++)
	{
		unsigned char input[8];
		unsigned char output[8];
		uint64_t rVal = 0;
		for (int i = 0; i < 4; i++)
			rVal |= ((uint64_t)(rand() % 256) << (8 * i));
		uint64_t lVal;

		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);

		for (int plntxtInd = 0; plntxtInd < TESTN; plntxtInd++)
		{
			// Init the left part of the srtucture (each str. is of 512 values) 
			if ((plntxtInd % 512) == 0)
			{
				lVal = 0;
				for (int i = 0; i < 4; i++)
					lVal |= ((uint64_t)(rand() % 256) << (8 * (i + 4)));
			}

			//Create the appropriate plaintext
			uint64_t plain = lVal | rVal;
			uint64_t temp = plntxtInd % 512;
			for (int j = 0; j < 9; j++)
			{
				if (temp % 2)
					plain ^= diff[j];
				temp /= 2;
			}

			for (int posi = 0; posi < 8; posi++)
			{
				input[7 - posi] = plain % 256;
				plain /= 256;
			}

			process_message(input, output, keyt, 9);
			uint64_t outp = 0;
			for (int ii = 0; ii < 8; ii++)
				outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
			//printf("%x\n", outpDiff);
			bool half1Parity = parity(outp & mask);
			for (uint64_t keyCandidate = 0; keyCandidate < 64; keyCandidate++)
			{
				//find the value that enter to s1
				uint64_t val = 0;
				uint64_t rellevantBit = 0x8000000;
				for (uint64_t KExp = 0; KExp < 5; KExp++)
				{
					if (outp & rellevantBit)
						val += ipow(2, KExp);
					rellevantBit <<= 1;
				}
				if (outp & 1)
					val += ipow(2, 5);
				val ^= keyCandidate;

				uint64_t Row = (val % 2) + 2 * (val / 32);
				uint64_t Col = 0;
				uint64_t temp = val / 2;
				for (uint64_t t = 0; t < 4; t++)
				{
					Col += ipow(2, t)*(temp % 2);
					temp /= 2;
				}
				uint64_t sval = sb[0][Row][Col];
				uint64_t f9out = 0;
				if (sval & 4)
					f9out = (uint64_t)0x800000000000;
				bool half2Parity = f9out ^ (outp & (uint64_t)0x800000000000);

				maskParity[keyCandidate][plntxtInd] = half1Parity ^ half2Parity;
			}
		}

		uint64_t indK;
		int bigCounter = 0;

		for (uint32_t outDiffS6Ind = 0; outDiffS6Ind < 9; outDiffS6Ind++)
		{
			for (uint32_t outDiffS8Ind = 0; outDiffS8Ind < 10; outDiffS8Ind++)
			{
				for (uint64_t keyC = 0; keyC < 64; keyC++)
				{
					
					//Run over the structures and check the parity of pairs that the output diff is outDiffS6[outDiffS6Ind] and outDiffS8[outDiffS8Ind].
					int count = 0;
					for (uint64_t i = 0; i < TESTN; i += 512)
					{
						for (uint64_t c1 = 0; c1 < 256; c1++)
						{
							uint32_t diffPosition = (((c1 % 16) ^ outDiffS6[outDiffS6Ind]) |
								(((c1 / 16) ^ outDiffS8[outDiffS8Ind]) << 4));
							if (maskParity[keyC][i + c1] == maskParity[keyC][i + 256 + diffPosition])
								count++;
						}
					}
					if (count > bigCounter)
					{
						bigCounter = count;
						indK = keyC;
					}
				}
			}
		}
		uint64_t sixBits = 0xfc;
		//uint64_t sixBits = 63;
		uint64_t thekey = (((uint64_t)keyt[8][0] & sixBits) >> 2);	//The key entering to S1 in the 8'th round, that guessed to copmute the parity of the output.
		if (indK == thekey)
			counter++;
	}
	return counter;
}

int calcTheKeyDiffPosition(uint64_t theKey, uint64_t rVal)
{
	uint64_t ks6 = theKey % 64;
	uint64_t ks8 = theKey >> 6;
	//value of s8
	uint64_t vals8 = ((rVal & 0x80000000) >> 31);
	uint64_t rellevantBit = 1;
	for (uint64_t KExp = 1; KExp < 6; KExp++)
	{
		if (rVal & rellevantBit)
			vals8 += ipow(2, KExp);
		rellevantBit <<= 1;
	}
	uint64_t vals8DDD = vals8 ^ 4;
	vals8 ^= ks8;
	uint64_t Row = (vals8 % 2) + 2 * (vals8 / 32);
	uint64_t Col = 0;
	uint64_t temp = vals8 / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		Col += ipow(2, t)*(temp % 2);
		temp /= 2;
	}
	uint64_t res8val = sb[7][Row][Col];

	//value of s6
	uint64_t vals6 = 0;
	rellevantBit = 0x80;
	for (uint64_t KExp = 0; KExp < 6; KExp++)
	{
		if (rVal & rellevantBit)
			vals6 += ipow(2, KExp);
		rellevantBit <<= 1;
	}
	uint64_t vals6DDD = vals6 ^ 4;
	vals6 ^= ks6;
	Row = (vals6 % 2) + 2 * (vals6 / 32);
	Col = 0;
	temp = vals6 / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		Col += ipow(2, t)*(temp % 2);
		temp /= 2;
	}
	uint64_t res6val = sb[5][Row][Col];

	//calculate the diff values
	uint64_t rvalDiff = rVal ^ (0x00000202);
	//diff value of s8
	uint64_t diffvals8 = ((rvalDiff & 0x80000000) >> 31);
	rellevantBit = 1;
	for (uint64_t KExp = 1; KExp < 6; KExp++)
	{
		if (rvalDiff & rellevantBit)
			diffvals8 += ipow(2, KExp);
		rellevantBit <<= 1;
	}
	diffvals8 ^= ks8;
	Row = (diffvals8 % 2) + 2 * (diffvals8 / 32);
	Col = 0;
	temp = diffvals8 / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		Col += ipow(2, t)*(temp % 2);
		temp /= 2;
	}
	uint64_t res8diffval = sb[7][Row][Col];

	//value of s6
	uint64_t diffvals6 = 0;
	rellevantBit = 0x80;
	for (uint64_t KExp = 0; KExp < 6; KExp++)
	{
		if (rvalDiff & rellevantBit)
			diffvals6 += ipow(2, KExp);
		rellevantBit <<= 1;
	}
	diffvals6 ^= ks6;
	Row = (diffvals6 % 2) + 2 * (diffvals6 / 32);
	Col = 0;
	temp = diffvals6 / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		Col += ipow(2, t)*(temp % 2);
		temp /= 2;
	}
	uint64_t res6diffval = sb[5][Row][Col];
	uint64_t resS68 = res6val | (res8val << 4);
	uint64_t resS68Diff = res6diffval | (res8diffval << 4);
	uint64_t resDifference = resS68 ^ resS68Diff;
	return resDifference;
}

void keyRec9RNeutTest()
{
	const int s[8][4][16] =
	{
		{
			{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
	{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
	{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
	{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
		},
	{
		{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
	{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
	{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
	{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
	},


	{
		{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
	{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
	{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
	{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
	},
	{
		{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
	{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
	{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
	{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
	},
	{
		{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
	{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
	{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
	{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
	},
	{
		{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
	{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
	{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
	{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
	},
	{
		{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
	{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
	{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
	{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
	},
	{
		{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
	{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
	{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
	{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
	}
	};

	ofstream file;
	file.open("NB.txt");
	uint64_t mask = 0x21040080;
	uint64_t diff[9] = {};
	diff[3] = 0x1000000000000000;
	diff[2] = 0x0000000800000000;
	diff[1] = 0x0020000000000000;
	diff[0] = 0x0000200000000000;
	diff[7] = 0x0800000000000000;
	diff[6] = 0x0000002000000000;
	diff[5] = 0x0002000000000000;
	diff[4] = 0x0000080000000000;
	diff[8] = 0x4000000000000202;

	int counter = 0;

	for (int keyInd = 0; keyInd < KEYTEST; keyInd++)
	{
		unsigned char input[8];
		unsigned char output[8];

		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);
		bool success = false;
		uint64_t indK;
		uint64_t sixBits = 0xfc;
		uint64_t thekey = (((uint64_t)keyt[8][0] & sixBits) >> 2);	//The key entering to S1 in the 8'th round, that guessed to copmute the parity of the output.
		uint64_t S6Key = (((uint64_t)keyt[0][4] & 0xf0) >> 4);
		S6Key |= (((uint64_t)keyt[0][3] & 3) << 4);
		uint64_t S8Key = ((uint64_t)keyt[0][5] & 0x3f);
		thekey |= ((S6Key << 6) | (S8Key << 12));
		uint64_t keyCounter[2] = {};

		int bigCounter = 0;
		for (int NIter = 0; NIter < 2; NIter++)
		{
			uint64_t rVal = 0;
			for (int i = 0; i < 4; i++)
				rVal |= ((uint64_t)(rand() % 256) << (8 * i));
			uint64_t lVal;
			uint64_t aneutralBits = ((uint64_t)(rand() % 32) << 59) | ((uint64_t)(rand() % 2) << 32);
			for (int plntxtInd = 0; plntxtInd < TESTN; plntxtInd++)
			{
				// Init the left part of the srtucture (each str. is of 512 values) 
				if ((plntxtInd % 512) == 0)
				{
					lVal = 0;
					lVal |= ((uint64_t)(rand()) << 44);
					lVal |= ((uint64_t)(rand() % 2048) << 33);
					lVal |= aneutralBits;
				}

				//Create the appropriate plaintext
				uint64_t plain = lVal | rVal;
				uint64_t temp = plntxtInd % 512;
				for (int j = 0; j < 9; j++)
				{
					if (temp % 2)
						plain ^= diff[j];
					temp /= 2;
				}

				for (int posi = 0; posi < 8; posi++)
				{
					input[7 - posi] = plain % 256;
					plain /= 256;
				}

				process_message(input, output, keyt, 9);
				uint64_t outp = 0;
				for (int ii = 0; ii < 8; ii++)
					outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
				bool half1Parity = parity(outp & mask);
				for (uint64_t keyCandidate = 0; keyCandidate < 64; keyCandidate++)
				{
					//find the value that enter to s1
					uint64_t val = 0;
					uint64_t rellevantBit = 0x8000000;
					for (uint64_t KExp = 0; KExp < 5; KExp++)
					{
						if (outp & rellevantBit)
							val += ipow(2, KExp);
						rellevantBit <<= 1;
					}
					if (outp & 1)
						val += ipow(2, 5);
					val ^= keyCandidate;

					uint64_t Row = (val % 2) + 2 * (val / 32);
					uint64_t Col = 0;
					uint64_t temp = val / 2;
					for (uint64_t t = 0; t < 4; t++)
					{
						Col += ipow(2, t)*(temp % 2);
						temp /= 2;
					}
					uint64_t sval = s[0][Row][Col];
					uint64_t f9out = 0;
					if (sval & 4)
						f9out = (uint64_t)0x800000000000;
					bool half2Parity = f9out ^ (outp & (uint64_t)0x800000000000);

					maskParity[keyCandidate][plntxtInd] = half1Parity ^ half2Parity;
				}
			}

			int theKeyCounter = 0;
			uint64_t resDifference0 = calcTheKeyDiffPosition((S6Key | (S8Key << 6)), rVal);
			//int subset0 = 0;// = (((resDifference & 0x8) >> 3) | ((resDifference & 0x80) >> 6));
			//if (aneutralBits & 0x1000000000000000)
			//	subset0 ^= 1;
			//if (aneutralBits & 0x800000000000000)
			//	subset0 ^= 2;
			for (int subset0 = 0; subset0 < 4; subset0++)
			{
				int c = 0;
				//Run over the structures and check the parity of appropriate pairs.
				for (uint64_t i = 0; i < TESTN; i += 512)
				{
					for (uint32_t c1 = 0; c1 < 64; c1++)
					{
						uint32_t position = (c1 & 7) | ((c1 & 56) << 1);
						position |= (((subset0 % 2) << 3) ^ ((subset0 / 2) << 7));
						uint32_t diffPosition = position ^ resDifference0;
						if (maskParity[thekey % 64][i + position] == maskParity[thekey % 64][i + 256 + diffPosition])
							c++;
					}
				}
				if (c > theKeyCounter)
					theKeyCounter = c;
			}
			//file << "KeyCounter = " << theKeyCounter << endl;

			int numMoreKey = 0;
			for (int frstR12KBits = 0; frstR12KBits < 4096; frstR12KBits++)
			{
				int ks6 = frstR12KBits % 64;
				int ks8 = frstR12KBits >> 6;
				//value of s8
				int vals8 = ((rVal & 0x80000000) >> 31);
				int rellevantBit = 1;
				for (int KExp = 1; KExp < 6; KExp++)
				{
					if (rVal & rellevantBit)
						vals8 += ipow(2, KExp);
					rellevantBit <<= 1;
				}
				uint64_t vals8DDD = vals8 ^ 4;
				vals8 ^= ks8;
				int Row = (vals8 % 2) + 2 * (vals8 / 32);
				int Col = 0;
				int temp = vals8 / 2;
				for (int t = 0; t < 4; t++)
				{
					Col += ipow(2, t)*(temp % 2);
					temp /= 2;
				}
				int res8val = s[7][Row][Col];

				//value of s6
				int vals6 = 0;
				rellevantBit = 0x80;
				for (int KExp = 0; KExp < 6; KExp++)
				{
					if (rVal & rellevantBit)
						vals6 += ipow(2, KExp);
					rellevantBit <<= 1;
				}
				uint64_t vals6DDD = vals6 ^ 4;
				vals6 ^= ks6;
				Row = (vals6 % 2) + 2 * (vals6 / 32);
				Col = 0;
				temp = vals6 / 2;
				for (int t = 0; t < 4; t++)
				{
					Col += ipow(2, t)*(temp % 2);
					temp /= 2;
				}
				int res6val = s[5][Row][Col];

				//calculate the diff values
				uint64_t rvalDiff = rVal ^ (0x00000202);
				//diff value of s8
				int diffvals8 = ((rvalDiff & 0x80000000) >> 31);
				rellevantBit = 1;
				for (int KExp = 1; KExp < 6; KExp++)
				{
					if (rvalDiff & rellevantBit)
						diffvals8 += ipow(2, KExp);
					rellevantBit <<= 1;
				}
				diffvals8 ^= ks8;
				Row = (diffvals8 % 2) + 2 * (diffvals8 / 32);
				Col = 0;
				temp = diffvals8 / 2;
				for (int t = 0; t < 4; t++)
				{
					Col += ipow(2, t)*(temp % 2);
					temp /= 2;
				}
				int res8diffval = s[7][Row][Col];

				//value of s6
				int diffvals6 = 0;
				rellevantBit = 0x80;
				for (int KExp = 0; KExp < 6; KExp++)
				{
					if (rvalDiff & rellevantBit)
						diffvals6 += ipow(2, KExp);
					rellevantBit <<= 1;
				}
				diffvals6 ^= ks6;
				Row = (diffvals6 % 2) + 2 * (diffvals6 / 32);
				Col = 0;
				temp = diffvals6 / 2;
				for (int t = 0; t < 4; t++)
				{
					Col += ipow(2, t)*(temp % 2);
					temp /= 2;
				}
				int res6diffval = s[5][Row][Col];
				int resS68 = res6val | (res8val << 4);
				int resS68Diff = res6diffval | (res8diffval << 4);
				int resDifference = resS68 ^ resS68Diff;

				for (int keyC = 0; keyC < 64; keyC++)
				{
					//int subset0 = 0;// = (((resDifference & 0x8) >> 3) | ((resDifference & 0x80) >> 6));
					//if (aneutralBits & 0x1000000000000000)
					//	subset0 ^= 1;
					//if (aneutralBits & 0x800000000000000)
					//	subset0 ^= 2;
					for (int subset = 0; subset < 4; subset++)
					{
						int count = 0;
						//Run over the structures and check the parity of appropriate pairs.
						for (uint64_t i = 0; i < TESTN; i += 512)
						{
							for (int c1 = 0; c1 < 64; c1++)
							{
								int position = (c1 & 7) | ((c1 & 56) << 1);
								position |= (((subset % 2) << 3) ^ ((subset / 2) << 7));
								int diffPosition = (((position % 16) ^ (resDifference % 16)) |
									(((position / 16) ^ (resDifference / 16)) << 4));
								int diffPosition2 = position ^ resDifference;
								if (maskParity[keyC][i + position] == maskParity[keyC][i + 256 + diffPosition])
									count++;
							}
						}
						//if (thekey == (keyC | (frstR12KBits << 6)))
						//	keyCounter[NIter] = count;
						if (count > theKeyCounter)
							numMoreKey++;
						//if (count > bigCounter)
						//{
						//	bigCounter = count;
						//	indK = keyC | (frstR12KBits << 6);
						//	file << bigCounter << "\t" << indK << endl;
						//}
					}
				}
			}
			file << numMoreKey << "\t";
		}
		file << endl;
		//if (indK == thekey)
		//	counter++;
		//else if ((keyCounter[0] == bigCounter) || (keyCounter[1] == bigCounter))
		//	counter++;
		//file << keyCounter[0] << "\t" << keyCounter[1] << "\t" << thekey << endl;
	}
	//file << counter;
	file.close();
}

void keyRec9RNeutPartTest()
{
	ofstream file;
	file.open("NB&Part.txt");
	uint64_t mask = 0x21040080;
	uint64_t outDiffS6[9] = { 11, 14, 3, 6, 10, 15, 9, 13, 12 };
	uint64_t outDiffS8[10] = { 7, 12, 10, 14, 6, 9, 5, 11, 13, 3 };
	uint64_t diff[9] = {};
	diff[3] = 0x1000000000000000;
	diff[2] = 0x0000000800000000;
	diff[1] = 0x0020000000000000;
	diff[0] = 0x0000200000000000;
	diff[7] = 0x0800000000000000;
	diff[6] = 0x0000002000000000;
	diff[5] = 0x0002000000000000;
	diff[4] = 0x0000080000000000;
	diff[8] = 0x4000000000000202;
	//diff[9] = 0x1000000000000000;

	int counter = 0;
	int Nstrctr = 0;
	int NIters[100] = {};

	for (int keyInd = 0; keyInd < KEYTEST; keyInd++)
	{
		unsigned char input[8];
		unsigned char output[8];

		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);
		bool success = false;
		uint64_t indK;

		int bigCounter = 0;
		for(int NIter = 0; NIter < 2; NIter++)
		//while (!success)
		{
			//if (success)
			//	break;
			//Nstrctr++;
			//NIters[keyInd]++;

			
			uint64_t rVal = 0;
			for (int i = 0; i < 4; i++)
				rVal |= ((uint64_t)(rand() % 256) << (8 * i));
			uint64_t lVal; //= (((uint64_t)(rand() % 256)) << 32) | (((uint64_t)(rand() % 256)) << 56);
			uint64_t aneutralBits = ((uint64_t)(rand() % 32) << 59) | ((uint64_t)(rand() % 2) << 32);
			//printf("%llx\n", aneutralBits);
			for (int plntxtInd = 0; plntxtInd < TESTN; plntxtInd++)
			{
				// Init the left part of the srtucture (each str. is of 512 values) 
				if ((plntxtInd % 512) == 0)
				{
					lVal = 0;
					lVal |= ((uint64_t)(rand()) << 44);
					lVal |= ((uint64_t)(rand() % 2048) << 33);
					//printf("%llx\n", lVal);
					lVal |= aneutralBits;
					//printf("%llx\n", lVal);
				}

				//Create the appropriate plaintext
				uint64_t plain = lVal | rVal;
				uint64_t temp = plntxtInd % 512;
				for (int j = 0; j < 9; j++)
				{
					if (temp % 2)
						plain ^= diff[j];
					temp /= 2;
				}

				for (int posi = 0; posi < 8; posi++)
				{
					input[7 - posi] = plain % 256;
					plain /= 256;
				}

				process_message(input, output, keyt, 9);
				uint64_t outp = 0;
				for (int ii = 0; ii < 8; ii++)
					outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
				//printf("%x\n", outpDiff);
				bool half1Parity = parity(outp & mask);
				for (uint64_t keyCandidate = 0; keyCandidate < 64; keyCandidate++)
				{
					//find the value that enter to s1
					uint64_t val = 0;
					uint64_t rellevantBit = 0x8000000;
					for (uint64_t KExp = 0; KExp < 5; KExp++)
					{
						if (outp & rellevantBit)
							val += ipow(2, KExp);
						rellevantBit <<= 1;
					}
					if (outp & 1)
						val += ipow(2, 5);
					val ^= keyCandidate;

					uint64_t Row = (val % 2) + 2 * (val / 32);
					uint64_t Col = 0;
					uint64_t temp = val / 2;
					for (uint64_t t = 0; t < 4; t++)
					{
						Col += ipow(2, t)*(temp % 2);
						temp /= 2;
					}
					uint64_t sval = sb[0][Row][Col];
					uint64_t f9out = 0;
					if (sval & 4)
						f9out = (uint64_t)0x800000000000;
					bool half2Parity = f9out ^ (outp & (uint64_t)0x800000000000);

					maskParity[keyCandidate][plntxtInd] = half1Parity ^ half2Parity;
				}
			}

			
			//bool con;

			for (uint32_t outDiffS6Ind = 0; outDiffS6Ind < 9; outDiffS6Ind++)
			{
				for (uint32_t outDiffS8Ind = 0; outDiffS8Ind < 10; outDiffS8Ind++)
				{
					for (uint64_t keyC = 0; keyC < 64; keyC++)
					{	
						for (int subset = 0; subset < 4; subset++)
						{
							int count = 0;
						//Run over the structures and check the parity of pairs that the output diff is outDiffS6[outDiffS6Ind] and outDiffS8[outDiffS8Ind].
							for (uint64_t i = 0; i < TESTN; i += 512)
							{
								for (uint32_t c1 = 0; c1 < 64; c1++)
								{
									uint32_t position = (c1 & 7) | ((c1 & 56) << 1);
									//position |= (((subset0 % 2) << 3) ^ (((subset0 / 2) % 2) << 7) ^ ((subset0 / 4) << 8));
									position |= (((subset % 2) << 3) ^ ((subset / 2) << 7) );
									//printf("%d\n", position);
									uint32_t diffPosition = (((position % 16) ^ outDiffS6[outDiffS6Ind]) |
										(((position / 16) ^ outDiffS8[outDiffS8Ind]) << 4));
									if (maskParity[keyC][i + position] == maskParity[keyC][i + 256 + diffPosition])
										count++;
								}
								
							}
							if (count > bigCounter)
							{
								bigCounter = count;
								indK = keyC;
								//double test = ((double)bigCounter / ((double)TESTN / 8.)) - 0.5;
								//if (bigCounter > 448)
								//	success = true;
							}
						}
					}
				}
			}
		}
		uint64_t sixBits = 0xfc;
		//uint64_t sixBits = 63;
		uint64_t thekey = (((uint64_t)keyt[8][0] & sixBits) >> 2);	//The key entering to S1 in the 8'th round, that guessed to copmute the parity of the output.
		if (indK == thekey)
		{
			counter++;
			//success = true;
		}
	}
	//for (int keyInd = 0; keyInd < 100; keyInd++)
	//	file << NIters[keyInd] << endl;
	file << counter;
	file.close();
	//return counter;
}

void smallTest()
{
	ofstream file;
	file.open("stdv.txt");
	for (int keyInd = 0; keyInd < KEYTEST; keyInd++)
	{
		unsigned char input[8];
		unsigned char output[8];
		unsigned char inputDiff[8];
		unsigned char outputDiff[8];

		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);
		int counter = 0;
		for (int pInd = 0; pInd < 768; pInd++)
		{
			uint64_t diff = 0x4000000000000000;
			uint64_t mask = 0x2104008000008000;
			uint64_t plntxt = 0;
			for (int i = 0; i < 8; i++)
				plntxt |= ((uint64_t)(rand() % 256) << 8 * i);
			uint64_t plntxtDiff = plntxt ^ diff;

			for (int i = 0; i < 8; i++)
			{
				input[7 - i] = plntxt % 256;
				inputDiff[7 - i] = plntxtDiff % 256;
				plntxt /= 256;
				plntxtDiff /=256;
			}
			process_message(input, output, keyt, 6);
			process_message(inputDiff, outputDiff, keyt, 6);

			uint64_t outp = 0;
			for (int ii = 0; ii < 8; ii++)
				outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
			uint64_t outpDiff = 0;
			for (int ii = 0; ii < 8; ii++)
				outpDiff |= (uint64_t(outputDiff[ii]) << (8 * (7 - ii)));
			if (parity(outp&mask) == parity(outpDiff&mask))
				counter++;
		}
		file << counter << endl;
	}
}

bool keyRec8RStructuresTest()
{
	//uint64_t outDiff[11] = { 6, 13, 3, 10, 14, 7, 12, 15, 5, 9, 11 };
	//uint64_t diff[5];
	//diff[3] = 0x1000000000000000;
	//diff[1] = 0x0020000000000000;
	//diff[0] = 0x0000200000000000;
	//diff[2] = 0x0000000800000000;
	//diff[4] = 0x0000000000000400;
	//uint64_t mask = 0x00808202;

	uint64_t outDiff[10] = { 14, 3, 5, 6, 9, 10, 12, 13, 11, 15 };
	uint64_t diff[5];
	diff[3] = 0x0080000000000000;
	diff[2] = 0x0000800000000000;
	diff[1] = 0x0000020000000000;
	diff[0] = 0x0000000200000000;
	diff[4] = 0x0000000060000000;
	uint64_t mask = 0x21040080;

	bool maskParity[64][TESTN];
	unsigned char input[8];
	unsigned char output[8];
	uint64_t rVal = 0;
	for (int i = 0; i < 4; i++)
		rVal |= ((uint64_t)(rand() % 256) << (8 * i));
	uint64_t lVal;

	//Generate a key
	uint64_t key = 0;
	for (int i = 0; i < 8; i++)
		key |= ((uint64_t)(rand() % 256) << 8 * i);
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	////Compute the output difference
	//uint64_t val1 = rVal, val2 = rVal ^ diff[4];
	//uint64_t val1rel = 0, val2rel = 0;
	//uint64_t relBits = 0x80;
	//for (uint64_t KExp = 0; KExp < 6; KExp++)
	//{
	//	if (val1&relBits)
	//		val1rel += ipow(2, KExp);
	//	if (val2&relBits)
	//		val2rel += ipow(2, KExp);
	//	relBits <<= 1;
	//}
	//uint64_t lsb_2 = 64 + 128;
	//uint64_t relK = ((uint64_t)keyt[0][3] & lsb_2) >> 2;
	//relK += ((uint64_t)keyt[0][4] & 0xf);
	//val1rel ^= relK;
	//val2rel ^= relK;
	//uint64_t row = (val1rel % 2) + 2 * (val1rel / 32);
	//uint64_t col = 0;
	//uint64_t tmp = val1rel / 2;
	//for (uint64_t t = 0; t < 4; t++)
	//{
	//	col += ipow(2, t)*(tmp % 2);
	//	tmp /= 2;
	//}
	//uint64_t sval1 = sb[5][row][col];
	//row = (val2rel % 2) + 2 * (val2rel / 32);
	//col = 0;
	//tmp = val2rel / 2;
	//for (uint64_t t = 0; t < 4; t++)
	//{
	//	col += ipow(2, t)*(tmp % 2);
	//	tmp /= 2;
	//}
	//uint64_t sval2 = sb[5][row][col];
	//uint64_t out6 = sval1 ^ sval2;
	//bool smallP = false;
	//if (out6 == outDiff[8] || out6 == outDiff[9] || out6 == outDiff[10])
	//	smallP = true;

	////Compute the output difference
	//uint64_t val1 = rVal, val2 = rVal ^ diff[4];
	//uint64_t val1rel = 0, val2rel = 0;
	//uint64_t relBits = 0x8000000;
	//for (uint64_t KExp = 0; KExp < 5; KExp++)
	//{
	//	if (val1&relBits)
	//		val1rel += ipow(2, KExp);
	//	if (val2&relBits)
	//		val2rel += ipow(2, KExp);
	//	relBits <<= 1;
	//}
	//if (val1 & 1)	//Since both are the same in the lsb
	//{
	//	val1rel += ipow(2, 5);
	//	val2rel += ipow(2, 5);
	//}
	//uint64_t relK = ((uint64_t)keyt[0][0] & 63);
	//val1rel ^= relK;
	//val2rel ^= relK;
	//uint64_t row = (val1rel % 2) + 2 * (val1rel / 32);
	//uint64_t col = 0;
	//uint64_t tmp = val1rel / 2;
	//for (uint64_t t = 0; t < 4; t++)
	//{
	//	col += ipow(2, t)*(tmp % 2);
	//	tmp /= 2;
	//}
	//uint64_t sval1 = sb[0][row][col];
	//row = (val2rel % 2) + 2 * (val2rel / 32);
	//col = 0;
	//tmp = val2rel / 2;
	//for (uint64_t t = 0; t < 4; t++)
	//{
	//	col += ipow(2, t)*(tmp % 2);
	//	tmp /= 2;
	//}
	//uint64_t sval2 = sb[0][row][col];
	//uint64_t out1 = sval1 ^ sval2;
	//if (out1 == 14)
	//	Nsb++;
	//bool smallP = false;
	//if (out1 == outDiff[7] || out1 == outDiff[8] || out1 == outDiff[9])
	//	Nss++;

	//uint64_t keyCount[64] = { 0 };

	for (int pTest = 0; pTest < TESTN; pTest++)
	{
		// Init the left part of the srtucture (each str. is of 32 values) 
		if (!(pTest % 32))
		{
			lVal = 0;
			for (int i = 0; i < 4; i++)
				lVal |= ((uint64_t)(rand() % 256) << (8 * (i + 4)));
		}

		//Create the appropriate plaintext
		uint64_t plain = lVal | rVal;
		uint64_t temp = pTest % 32;
		for (int j = 0; j < 5; j++)
		{
			if (temp % 2)
				plain ^= diff[j];
			temp /= 2;
		}
		//if (!(pTest % 32))
		//{
		//	cout << "new plaintext type" << endl;
		//	printf("%x\n", plain);
		//}
		
		for (int posi = 0; posi < 8; posi++)
		{
			input[7 - posi] = plain % 256;
			plain /= 256;
		}

		process_message(input, output, keyt, 8);
		uint64_t outp = 0;
		for (int ii = 0; ii < 8; ii++)
			outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
		//printf("%x\n", outpDiff);
		bool half1Parity = parity(outp & mask);
		for (uint64_t keyCandidate = 0; keyCandidate < 64; keyCandidate++)
		{
			////find the value that enter to s4
			//uint64_t val = 0;
			//uint64_t rellevantBit = 0x8000;
			//for (uint64_t outDiffS8Ind = 0; outDiffS8Ind < 6; outDiffS8Ind++)
			//{
			//	if (outpDiff&rellevantBit)
			//		val += ipow(2, outDiffS8Ind);
			//	rellevantBit <<= 1;
			//}
			//val ^= keyCandidate;

			//uint64_t Row = (val % 2) + 2 * (val / 32);
			//uint64_t Col = 0;
			//uint64_t temp = val / 2;
			//for (uint64_t t = 0; t < 4; t++)
			//{
			//	Col += ipow(2, t)*(temp % 2);
			//	temp /= 2;
			//}
			//uint64_t sval = sb[3][Row][Col];
			//uint64_t f9out;
			//if (sval % 2)
			//	f8out = 0x8000000000000000;
			//else
			//	f8out = 0;
			//bool half2Parity = f8out ^ (outpDiff & 0x8000000000000000);	// Maybe parity(f9out ^ (outpDiff & 0x8000000000000000))?

			//find the value that enter to s1
			uint64_t val = 0;
			uint64_t rellevantBit = 0x8000000;
			for (uint64_t KExp = 0; KExp < 5; KExp++)
			{
				if (outp & rellevantBit)
					val += ipow(2, KExp);
				rellevantBit <<= 1;
			}
			if (outp & 1)
				val += ipow(2, 5);
			val ^= keyCandidate;

			uint64_t Row = (val % 2) + 2 * (val / 32);
			uint64_t Col = 0;
			uint64_t temp = val / 2;
			for (uint64_t t = 0; t < 4; t++)
			{
				Col += ipow(2, t)*(temp % 2);
				temp /= 2;
			}
			uint64_t sval = sb[0][Row][Col];
			uint64_t f8out;
			if (sval & 4)
				f8out = (uint64_t)0x800000000000;
			else
				f8out = 0;
			bool half2Parity = f8out ^ (outp & 0x800000000000);

			maskParity[keyCandidate][pTest] = half1Parity ^ half2Parity;
		}
	}

	uint64_t indK;
	int bigCounter = 0;
	
	//for (uint64_t outDiffS6Ind = 0; outDiffS6Ind < 11; outDiffS6Ind++)
	for (uint64_t outDiffInd = 0; outDiffInd < 10; outDiffInd++)
	{
		bool success = false;
		for (uint64_t keyC = 0; keyC < 64; keyC++)
		{
			int count = 0;
			//Run over the structures and check the parity of pairs that the output diff is outDiff[outDiffS6Ind].
			for (uint64_t i = 0; i < TESTN; i += 32)
			{
				for (uint64_t c1 = 0; c1 < 16; c1++)
				{
					if (maskParity[keyC][i + c1] == maskParity[keyC][i + 16 + (c1^outDiff[outDiffInd])])
						count++;
				}
			}
			//if (count > keyCount[keyC])
			//	keyCount[keyC] = count;
			if (count > bigCounter)
			{
				bigCounter = count;
				indK = keyC;
			}
			//if (count > TESTN*0.285)
			//{
			//	NExp += (outDiffS6Ind + 1);
			//	success = true;
			//	indK = keyC;
			//	break;
			//}

			//if (count > TESTN*0.27)
			//	break;
		}
		//if (!success)
		//	NExp += 10;
		//if (success)
		//	break;
		//else
		//	NExp += 64;

	}





	//uint64_t keyCount2[64] = {};
	//int oof = 0;

	//for (uint64_t keyCand = 0; keyCand < 64; keyCand++)
	//{
	//	int counter = 0;
	//	for (uint64_t strctN = 0; strctN < TESTN; strctN += 32)
	//	{
	//		int maxCounter = 0;
	//		for (uint64_t outDiffIndex = 0; outDiffIndex < 11; outDiffIndex++)
	//		{
	//			int subCounter = 0;
	//			for (uint64_t c1 = 0; c1 < 16; c1++)
	//			{
	//				if (maskParity[keyCand][strctN + c1] == maskParity[keyCand][strctN + 16 + (c1^outDiff[outDiffIndex])])
	//					subCounter++;
	//			}
	//			if (subCounter > maxCounter)
	//			{
	//				maxCounter = subCounter;
	//				if (outDiffIndex > 4)
	//					oof++;
	//			}
	//		}
	//		counter += maxCounter;
	//	}
	//	keyCount2[keyCand] = counter;
	//}






	
	//int maxK = 0;
	//int maxK2 = 0;
	//uint64_t indK;
	//uint64_t indK2;
	//for (uint64_t KExp = 0; KExp < 64; KExp++)
	//{
	//	if (keyCount[KExp] > maxK)
	//	{
	//		maxK = keyCount[KExp];
	//		indK = KExp;
	//	}

	//	//if (keyCount2[KExp] > maxK2)
	//	//{
	//	//	maxK2 = keyCount2[KExp];
	//	//	indK2 = KExp;
	//	//}
	//}


	//uint64_t sixBits = 63;
	//uint64_t thekey = ((uint64_t)keyt[7][2] & sixBits);	//The key entering to S4  in the 8'th round, that guessed to copmute the parity of the output.
	//if (indK == thekey)
	//{
	//	Nss++;
	//	//if (smallP)
	//	//	Nss++;
	//	//else
	//	//	Nsb++;
	//	return true;
	//}

	uint64_t sixBits = 0xfc;
	//uint64_t sixBits = 63;
	uint64_t thekey = (((uint64_t)keyt[7][0] & sixBits) >> 2);	//The key entering to S1 in the 8'th round, that guessed to copmute the parity of the output.
	if (indK == thekey)
	{
		Nss++;
		//if (smallP)
		//	Nss++;
		//else
		//	Nsb++;
		return true;
	}

	//if (indK == thekey)
	//	m1++;
	//if (indK2 == thekey)
	//	m2++;

	return false;
}

void cipherRec7RStructuresTest()
{
	ofstream myfile1;
	myfile1.open("7RStructuresProb.txt");
	ofstream file2;
	file2.open("outdiffExp.txt");
	ofstream file3;;
	file3.open("outdiff.txt");

	//uint64_t mask = 0x0080820280000000;
	//uint64_t diff[5];
	//diff[0] = 0x1000000000000000;
	//diff[2] = 0x0020000000000000;
	//diff[3] = 0x0000200000000000;
	//diff[1] = 0x0000000800000000;
	//diff[4] = 0x0000000000000400;
	//uint64_t outDiff[11] = { 6, 13, 3, 10, 14, 7, 12, 15, 5, 9, 11 };

	uint64_t outDiff[10] = { 14, 3, 5, 6, 9, 10, 12, 13, 11, 15 };
	uint64_t diff[5];
	diff[3] = 0x0080000000000000;
	diff[2] = 0x0000800000000000;
	diff[1] = 0x0000020000000000;
	diff[0] = 0x0000000200000000;
	diff[4] = 0x0000000060000000;
	uint64_t mask = 0x2104008000008000;

	for (int keytest = 0; keytest < KEYTEST; keytest++)
	{
		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);

		bool maskParity[TESTN];
		int count = 0;
		unsigned char input[8];
		unsigned char output[8];
		uint64_t rVal = 0;
		for (int i = 0; i < 4; i++)
			rVal |= ((uint64_t)(rand() % 256) << (8 * i));

		////Compute the output difference
		//uint64_t val1 = rVal, val2 = rVal ^ diff[4];
		//uint64_t val1rel = 0, val2rel = 0;
		//uint64_t relBits = 0x1f80;
		//val1rel = (val1 & relBits) >> 7;
		//val2rel = (val2 & relBits) >> 7;

		//uint64_t relK = ((uint64_t)keyt[0][3] & 3)/* << 4*/;
		//relK += (((uint64_t)keyt[0][4] & 0xf0) >> 2);
		//val1rel ^= relK;
		//val2rel ^= relK;
		//uint64_t row = (val1rel % 2) + 2 * (val1rel / 32);
		//uint64_t col = 0;
		//uint64_t tmp = val1rel / 2;
		//for (uint64_t t = 0; t < 4; t++)
		//{
		//	col += ipow(2, t)*(tmp % 2);
		//	tmp /= 2;
		//}
		//uint64_t sval1 = sb[5][row][col];
		//row = (val2rel % 2) + 2 * (val2rel / 32);
		//col = 0;
		//tmp = val2rel / 2;
		//for (uint64_t t = 0; t < 4; t++)
		//{
		//	col += ipow(2, t)*(tmp % 2);
		//	tmp /= 2;
		//}
		//uint64_t sval2 = sb[5][row][col];
		//uint64_t out6 = sval1 ^ sval2;
		//file3 << out6 << endl;

		////Compute the output difference
		//uint64_t val1 = rVal, val2 = rVal ^ diff[4];
		//uint64_t val1rel = 0, val2rel = 0;
		//uint64_t relBits = 0x8000000;
		//for (uint64_t KExp = 0; KExp < 5; KExp++)
		//{
		//	if (val1&relBits)
		//		val1rel += ipow(2, KExp);
		//	if (val2&relBits)
		//		val2rel += ipow(2, KExp);
		//	relBits <<= 1;
		//}
		//if (val1 & 1)	//Since both are the same in the lsb
		//{
		//	val1rel += ipow(2, 5);
		//	val2rel += ipow(2, 5);
		//}
		//uint64_t relK = ((uint64_t)keyt[0][0] & 0xfc) >> 2;
		//val1rel ^= relK;
		//val2rel ^= relK;

		////file3 << val1rel << "\n" << val2rel << endl;

		////uint64_t in1 = val1rel ^ val2rel;
		////file3 << in1 << endl;

		//uint64_t row = (val1rel % 2) + 2 * (val1rel / 32);
		//uint64_t col = 0;
		//uint64_t tmp = val1rel / 2;
		//for (uint64_t t = 0; t < 4; t++)
		//{
		//	col += ipow(2, t)*(tmp % 2);
		//	tmp /= 2;
		//}
		//uint64_t sval1 = sb[0][row][col];
		//row = (val2rel % 2) + 2 * (val2rel / 32);
		//col = 0;
		//tmp = val2rel / 2;
		//for (uint64_t t = 0; t < 4; t++)
		//{
		//	col += ipow(2, t)*(tmp % 2);
		//	tmp /= 2;
		//}
		//uint64_t sval2 = sb[0][row][col];
		//uint64_t out1 = sval1 ^ sval2;
		////if (out1 == 14)
		////	p = true;
		////file3 << out1 << endl;
		
		uint64_t lVal;

		for (uint64_t testN = 0; testN < TESTN; testN++)
		{
			// Init the left part of the srtucture (each str. is of 32 values) 
			if (!(testN % 32))
			{
				lVal = 0;
				for (int i = 0; i < 4; i++)
					lVal |= ((uint64_t)(rand() % 256) << (8 * (i + 4)));
			}

			//Create the appropriate plaintext
			uint64_t plain = lVal | rVal;
			uint64_t temp = testN % 32;
			for (int j = 0; j < 5; j++)
			{
				if (temp % 2)
					plain ^= diff[j];
				temp /= 2;
			}
			temp = plain;
			for (int posi = 0; posi < 8; posi++)
			{
				input[7 - posi] = temp % 256;
				temp /= 256;
			}

			process_message(input, output, keyt, 7);
			uint64_t outp = 0;
			for (int ii = 0; ii < 8; ii++)
				outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
			maskParity[testN] = parity(outp & mask);
		}

		double prob = 0;
		
		int outdiffsuc = 0;
		for (uint64_t outDiffInd = 0; outDiffInd < 10; outDiffInd++)
		//for (uint64_t outDiffS6Ind = 0; outDiffS6Ind < 11; outDiffS6Ind++)
		{
			count = 0;
			//Run over the structures and check the parity of pairs that the output diff is outDiff[outDiffS6Ind].
			for (uint64_t i = 0; i < TESTN; i += 32)
			{
				for (uint64_t c1 = 0; c1 < 16; c1++)
				{
						if (maskParity[i + c1] == maskParity[i + 16 + (c1^outDiff[outDiffInd])])
							count++;
				}
			}
			double res = double(count) / (0.5*((double)(TESTN))) - 0.5;
			if (res > prob)
			{
				prob = res;
				outdiffsuc = outDiffInd;
				if (res > 0.06)
					break;
			}
		}
		//if (prob < 0.06)
		//{

		//}
		myfile1 << prob << endl;
		file2 << outDiff[outdiffsuc] << endl;
	}
	myfile1.close();
	file2.close();
	file3.close();
}

void speciphicDiffExp()
{
	ofstream myfile1;
	myfile1.open("smallExp.txt");
	uint64_t diff;
	uint64_t mask;

	for (int test = 0; test < 3; test++)
	{
		switch (test)
		{
		case 0:
			diff = 0x0020000800000400;
			mask = 0x0080820280000000;
			break;
		case 1:
			diff = 0x0080820060000000;
			mask = 0x2104008000008000;
			break;
		case 2:
			diff = 0x0000020240000000;
			mask = 0x2104008000008000;
			break;
		default:
			break;
		}

		//Print the values
		myfile1 << "diff = ";
		char diffHex[16];
		prtInt64AsHex(diff, diffHex);
		for (int i = 0; i < 16; i++)
			myfile1 << diffHex[i];
		
		myfile1 << ", mask = ";
		char maskHex[16];
		prtInt64AsHex(mask, maskHex);
		for (int i = 0; i < 16; i++)
			myfile1 << maskHex[i];
		myfile1 << endl;

		//Put the diff into an array of char vars.
		unsigned char diffa[8];
		uint64_t difft = diff;
		for (int i = 0; i < 8; i++)
		{
			diffa[7 - i] = difft % 256;
			difft /= 256;
		}

		for (int keytest = 0; keytest < KEYTEST; keytest++)
		{
			//Generate a key
			uint64_t key = 0;
			for (int i = 0; i < 8; i++)
				key |= ((uint64_t)(rand() % 256) << 8 * i);
			string kp;
			int2BinS(key, 64, kp);
			unsigned char keyt[16][6];
			keySchedule(keyt, kp);

			double res = 0.;
			bool stest1, stest2;
			int count = 0;
			unsigned char input[8];
			for (int testN = 0; testN < TESTN; testN++)
			{
				unsigned char output[8];
				if (testN % 2)
				{
					for (int ii = 0; ii < 8; ii++)
						input[ii] ^= diffa[ii];
					process_message(input, output, keyt, 7);
					uint64_t outp = 0;
					for (int ii = 0; ii < 8; ii++)
						outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
					stest2 = parity(outp & mask);
					if (stest1 == stest2)
						count++;
				}
				else
				{
					for (int posi = 0; posi < 8; posi++)
						input[posi] = rand() % 256;
					process_message(input, output, keyt, 7);
					uint64_t outp = 0;
					for (int ii = 0; ii < 8; ii++)
						outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
					stest1 = parity(outp & mask);
				}
			}
			res = double(count) / (0.5*((double)(TESTN)));
			myfile1 << res - 0.5 << endl;
		}
		myfile1 << endl;
	}
	myfile1.close();
}

void sbox1startEndExp()
{
	double bestres = 0;
	uint64_t bestdiff;
	uint64_t bestmask;
	ofstream myfile1;
	myfile1.open("bigExp.txt");

	//Generate a key
	uint64_t rnd;
	uint64_t key = rand();
	for (int i = 0; i < 4; i++)
	{
		key <<= 15;
		rnd = rand();
		key |= rnd;
	}
	key <<= 4;
	rnd = rand() % 16;
	key |= rnd;
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	for (uint64_t difftest = 0; difftest < 24; difftest++)
	{
		uint64_t diff[16];
		cmptFullDiff(2 * (difftest % 3) + 2, diff, 8 - (difftest / 3));

		for (int fd = 0; fd < 16; fd++)
		{
			uint64_t difft;
			if (diff[fd] != 0xffffffffffffffff)	//This diff means that this case is not relevant.
				difft = diff[fd];
			else
				continue;
			unsigned char diffa[8];
			for (int i = 0; i < 8; i++)
			{
				diffa[7 - i] = difft % 256;
				difft /= 256;
			}
			if (diff[fd] == 0xffffffffffffffff)
				continue;
			
			for (uint64_t masktest = 0; masktest < 64 * 8; masktest++)
			{
				uint64_t mask[16];
				cmptFullMask(masktest % 64, masktest / 64, mask);
				for (int fm = 0; fm < 16; fm++)
				{
					if (mask[fm] == 0xffffffffffffffff)
						continue;
					double res = 0.;
					for (int keytest = 0; keytest < KEYTEST; keytest++)
					{
						bool stest1, stest2;
						int count = 0;
						unsigned char input[8];
						//uint64_t plaint;
						//string plain;
						for (int testN = 0; testN < TESTN; testN++)
						{
							unsigned char output[8];
							
							
							if (testN % 2)
							{
								for (int ii = 0; ii < 8; ii++)
									input[ii] ^= diffa[ii];
								process_message(input, output, keyt, 7);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								//int2BinS(plaint^diff[fd], 64, plain);
								//uint64_t outpDiff = strToInt(des(kp, plain, 7), 64);
								stest2 = parity(outp & mask[fm]);
								if (stest1 == stest2)
									count++;
							}
							else
							{
								for (int posi = 0; posi < 8; posi++)
									input[posi] = rand() % 256;
								process_message(input, output, keyt, 7);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								
								//plaint = rand();
								//for (int KExp = 0; KExp < 4; KExp++)
								//{
								//	plaint <<= 15;
								//	rnd = rand();
								//	plaint |= rnd;
								//}
								//plaint <<= 4;
								//rnd = rand() % 16;
								//plaint |= rnd;
								//int2BinS(plaint, 64, plain);
								//uint64_t outpDiff = strToInt(des(kp, plain, 7), 64);
								stest1 = parity(outp & mask[fm]);
							}
						}
						res += double(count) / (0.5*((double)(TESTN)));
					}
					//if(diff[fd]==0x0080820060000000 && mask[fm]==0x2104008000008000)
					//{
					//	myfile1 << "diff = ";
					//	char diffHex[16];
					//	prtInt64AsHex(diff[fd], diffHex);
					//	for (int KExp = 0; KExp < 16; KExp++)
					//		myfile1 << diffHex[KExp];
					//	myfile1 << ",  mask = ";
					//	char maskHex[16];
					//	prtInt64AsHex(mask[fm], maskHex);
					//	for (int KExp = 0; KExp < 16; KExp++)
					//		myfile1 << maskHex[KExp];
					//	myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					//}
					if (res < 0.9 && abs(res - 0.5) > 0.015)	//Print only the interesting results.
					{
						myfile1 << "diff = ";
						char diffHex[16];
						prtInt64AsHex(diff[fd], diffHex);
						for (int i = 0; i < 16; i++)
							myfile1 << diffHex[i];
						myfile1 << ",  mask = ";
						char maskHex[16];
						prtInt64AsHex(mask[fm], maskHex);
						for (int i = 0; i < 16; i++)
							myfile1 << maskHex[i];
						myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					}
					if (res < 0.9 && abs(res - 0.5) > bestres)
					{
						bestres = abs(res - 0.5);
						bestdiff = diff[fd];
						bestmask = mask[fm];
					}
				}
			}
		}
	}
	myfile1 << "\n" << "bestres = " << bestres << ", bestdiff = " 
		<< bestdiff << ", bestmask = " << bestmask << endl;
	
	myfile1.close();
}

void LangfordHellmanLikeExp()
{
	double bestres = 0;
	uint64_t bestdiff;
	uint64_t bestmask;
	ofstream myfile1;
	myfile1.open("LHExpS.txt");

	//Generate a key
	uint64_t rnd;
	uint64_t key = rand();
	for (int i = 0; i < 4; i++)
	{
		key <<= 15;
		rnd = rand();
		key |= rnd;
	}
	key <<= 4;
	rnd = rand() % 16;
	key |= rnd;
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	for (uint64_t difftest = 0; difftest < 24; difftest++)
	{
		uint64_t diff = (2 * (difftest % 3) + 2) << (4 * (difftest / 3) + 32);

		uint64_t difft = diff;
			unsigned char diffa[8];
			for (int i = 0; i < 8; i++)
			{
				diffa[7 - i] = difft % 256;
				difft /= 256;
			}

			for (uint64_t masktest = 0; masktest < 4/*64 * 8*/; masktest++)
			{
				uint64_t maska;
				switch (masktest)
				{
				case 0:
					maska = 0x2104008000008000;
					break;
				case 1:
					maska = 0x0080820280000000;
					break;
				case 2:
					maska = 0x8040104000140000;
					break;
				case 3:
					maska = 0x8040104000110000;
					break;
				default:
					maska = 0;
					break;
				}
				//uint64_t mask[16];
				//cmptFullMask(masktest % 64, masktest / 64, mask);
				//for (int fm = 0; fm < 16; fm++)
				//{
					//if (mask[fm] == 0xffffffffffffffff)
					//	continue;
					double res = 0.;
					for (int keytest = 0; keytest < KEYTEST; keytest++)
					{
						bool stest1, stest2;
						int count = 0;
						unsigned char input[8];
						//uint64_t plaint;
						//string plain;
						for (int testN = 0; testN < TESTN; testN++)
						{
							unsigned char output[8];


							if (testN % 2)
							{
								for (int ii = 0; ii < 8; ii++)
									input[ii] ^= diffa[ii];
								process_message(input, output, keyt, 6);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								//int2BinS(plaint^diff[fd], 64, plain);
								//uint64_t outpDiff = strToInt(des(kp, plain, 7), 64);

								//stest2 = parity(outpDiff & mask[fm]);
								stest2 = parity(outp & maska);
								if (stest1 == stest2)
									count++;
							}
							else
							{
								for (int posi = 0; posi < 8; posi++)
									input[posi] = rand() % 256;
								process_message(input, output, keyt, 6);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));

								//plaint = rand();
								//for (int KExp = 0; KExp < 4; KExp++)
								//{
								//	plaint <<= 15;
								//	rnd = rand();
								//	plaint |= rnd;
								//}
								//plaint <<= 4;
								//rnd = rand() % 16;
								//plaint |= rnd;
								//int2BinS(plaint, 64, plain);
								//uint64_t outpDiff = strToInt(des(kp, plain, 7), 64);

								//stest1 = parity(outpDiff & mask[fm]);
								stest1 = parity(outp & maska);
							}
						}
						res += double(count) / (0.5*((double)(TESTN)));
					}
					//if(diff[fd]==0x0080820060000000 && mask[fm]==0x2104008000008000)
					//{
					//	myfile1 << "diff = ";
					//	char diffHex[16];
					//	prtInt64AsHex(diff[fd], diffHex);
					//	for (int KExp = 0; KExp < 16; KExp++)
					//		myfile1 << diffHex[KExp];
					//	myfile1 << ",  mask = ";
					//	char maskHex[16];
					//	prtInt64AsHex(mask[fm], maskHex);
					//	for (int KExp = 0; KExp < 16; KExp++)
					//		myfile1 << maskHex[KExp];
					//	myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					//}
					if (res < 0.9 && abs(res - 0.5) > 0.05)	//Print only the interesting results.
					{
						myfile1 << "diff = ";
						char diffHex[16];
						prtInt64AsHex(diff, diffHex);
						for (int i = 0; i < 16; i++)
							myfile1 << diffHex[i];
						myfile1 << ",  mask = ";
						char maskHex[16];
						//prtInt64AsHex(mask[fm], maskHex);
						prtInt64AsHex(maska, maskHex);
						for (int i = 0; i < 16; i++)
							myfile1 << maskHex[i];
						myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					}
					if (res < 0.9 && abs(res - 0.5) > bestres)
					{
						bestres = abs(res - 0.5);
						bestdiff = diff;
						//bestmask = mask[fm];
						bestmask = maska;
					}
				//}
			}
	}
	myfile1 << "\n" << "bestres = " << bestres << ", bestdiff = "
		<< bestdiff << ", bestmask = " << bestmask << endl;

	myfile1.close();
}

double Phi(double z)
{
	//if (z > 0)
	//	return 1. - exp(-z * z / 2.) / (z + 0.5);
	//else
	//	return exp(-z * z / 2.) / (-z + 0.5);

	return 0.5*(1. + erf(z / sqrt(2.)));
}

double PErr(double t, double p, double N)
{
	double err1 = Phi(t * 2.*sqrt(N));
	double err2 = Phi((t + 0.5 - p) / sqrt(p*(1 - p) / N));
	return pow(err1,10.) * err2;

	//double err1 = 1. - Phi((t + 0.5 - p) / sqrt(p*(1 - p) / N));
	//double err2 = Phi(t * 2.*sqrt(N));
	//return err1 * err2;
}

int main()
{
	//serpentDDTS2Init();
	//int r = serpentDDTS2[5][0xa];
	time_t ttt;
	srand((unsigned)time(&ttt));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0., 1.);
	//smallTest();
	//keyRec9RNeutPartTest();
	keyRec9RNeutTest();

	//ddtInit();
	//latInit();
	//dlctInit();
	//double ppp = dlct3roundsConLin(20. / 64., 0x40000000, 12. / 64., 1, 0, 4);
	////int successRate = keyRec9RStrctrTest();
	////specificOutTest();

	////cipherRec7RStructuresTest();

	////for (int subset0 = 0; subset0 < KEYTEST; subset0++)
	////	keyRec8RStructuresTest();

	////double avg = (double)Nss / (double)KEYTEST;

	////sbox1startEndExp();
	//LangfordHellmanLikeExp();

	//double Pr = PErr(0.06, 0.5820316, 256);

	//speciphicDiffExp();
	//
	//
	//
	//double outs8_4[10] = { 12., 10., 8., 8., 6., 6., 4., 4., 4., 2. };
	//double outs6_4[9] = { 10., 10., 8., 8., 8., 8., 6., 4., 2. };
	//double outs6_8[11] = { 16., 12., 6., 6., 6., 4., 4., 4., 2., 2., 2. };
	//double mean84 = 0.;
	//double mean64 = 0.;
	//double mean68 = 0.;
	//for (int subset0 = 0; subset0 < 10; subset0++)
	//	mean84 += (subset0 + 1)*outs8_4[subset0] / 64.;
	//for (int subset0 = 0; subset0 < 9; subset0++)
	//	mean64 += (subset0 + 1)*outs6_4[subset0] / 64.;
	//for (int subset0 = 0; subset0 < 11; subset0++)
	//	mean68 += (subset0 + 1)*outs6_8[subset0] / 64.;

	////double Ps = 0.5*(1. + erf(2.*sqrt(7.) - sqrt(2.)*erfinv(2.*(1. - pow(2., -6.)) - 1.)));



	////double res = dlct3roundsConLin(20. / 64., 0x60000000);

	////speciphicDiffExp();
	return 0;
}

//dlctInit();
//ofstream f;
//f.open ("dlct.txt");
//for (int sbox = 0; sbox < 8; sbox++)
//{
//	f<<"sbox = "<<sbox+1<<"\n"<<endl;
//	for(int r=0; r < 64; r++)
//	{
//		f<<r<<":  ";
//		if(r<10)
//			f<<" ";
//		for(int c=0;c<16;c++)
//		{
//			if (r == 0 || c== 0)
//				f<<99<<" ";
//			else
//				f<<dlct[sbox][r][c]-32<<" ";
//		}
//		f<<endl;
//	}
//	f<<"\n"<<endl;
//}