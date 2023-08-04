#include <iostream>
#include<stdlib.h>
using namespace std;

//������ײ����
//int Collisionlen=0;
int cmphash(unsigned char* H1, unsigned char* H2, int Len)
{   //��ʱ��������32bit�ıȽ�
	if (Len <= 32) { //ȡint�Ƚ�
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

		if (!cmphash(H, output, Collisionlen) && tmp != image) //�����ײ�ˣ��ͷ���0��ͬʱ��ӡ��һ��sm3ֵ
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
	srand(time(NULL));//��ʼ�������ֱ��Ӱ�쵽�ҵ���·��ʱ�䣬�õ�ʱ��ֻ��Ҫ���룬���õ�ʱ��Ҫ������
	clock_t start, end;//����clock_t����
	start = clock();//��ʼʱ��
	unsigned int image = rand();
	PreimageAttack(image);
	end = clock();//����ʱ��
	cout << "����ʱ�䣺time=" << double(end - start) / CLOCKS_PER_SEC << "s" << endl;//���ʱ�䣨��λ��s��
	return 0;
}