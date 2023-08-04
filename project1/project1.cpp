#include <iostream>
#include<stdlib.h>
using namespace std;

//定义碰撞长度
//int Collisionlen=0;
int cmphash(unsigned char* H1, unsigned char* H2, int Len)
{   //暂时不处理超过32bit的比较
	if (Len <= 32) { //取int比较
		uint a = *(int*)H1;
		uint b = *(int*)H2;
		uint mask = (int)pow(2, Collisionlen) - 1;
		if ((a & mask) == (b & mask))
			return 0;
	}
	return 1;
}

int Pollard_Rho(uint image, unsigned char* H, uint c, uint* preimage) //H=SM3(image)
{
	uint m1 = rand();
	uint m2 = m1;
	while (true)
	{
		m1 = F(m1, c);
		m2 = F(F(m2, c), c);
		if (m2 == m1)
			return 1;
		uint tmp = m2 - m1;
		string input = to_string(tmp).c_str();
		unsigned char output[SM3_OUTLEN];
		SM3(input, output);

		if (!cmphash(H, output, Collisionlen) && tmp != image) //如果碰撞了，就返回0，同时打印了一下sm3值
		{
			*preimage = tmp;
			cout << "SM3(" << input << "):";
			print_Hashvalue(output, SM3_OUTLEN);
			return 0;
		}
	}
}

void PreimageAttack(uint image)
{
	uint preimage;
	string image_input = to_string(image);
	unsigned char image_output[SM3_OUTLEN];

	uint c = rand();
	while (Pollard_Rho(image, image_output, c, &preimage))
	{
		c = rand();
	}
}

int main()
{
	srand(time(NULL));//初始随机数会直接影响到找到环路的时间，好的时候只需要几秒，不好的时候要几分钟
	clock_t start, end;//定义clock_t变量
	start = clock();//开始时间
	unsigned int image = rand();
	PreimageAttack(image);
	end = clock();//结束时间
	cout << "花费时间：time=" << double(end - start) / CLOCKS_PER_SEC << "s" << endl;//输出时间（单位：s）
	return 0;
}