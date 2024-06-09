#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include <string.h>
#include<malloc.h>
#include<math.h>
#include <windows.h>
#define Num 1024

int* randomarray(int n)
{
	int a[Num];
	for (int i = 0;i < Num;i++)
	{
		a[i] = rand() % (n + 1);
	}
	for (int i1 = 0;i1 < Num;i1++)
		printf("%d ", a[i1]);
	printf("\n");//测试正常输出
	return a;
}

int* linealarray(int n)
{
	int a[Num];
	int A = 10051, B = 255267;
	int M = 998244353;
	a[0] = 7;//便于测试，可换为由random产生的随机数
	for (int i = 0;i < Num-1;i++)
	{
		a[i + 1] = (a[i] * A + B) % M;
		if (a[i + 1] < 0)
		{
			a[i + 1] = a[i + 1] + M;
		}
	}
	for (int i2 = 0;i2 < Num ;i2++)
	{
		a[i2] = a[i2] % (n+1);
	}
	for (int i1 = 0;i1 < Num;i1++)
		printf("%d ", a[i1]);
	printf("\n");//测试正常输出
	return a;
}
//线性同余方法（LCG）是一种产生伪随机数的方法。
//它是根据递归公式：RandSeed = (A * RandSeed + B) % M
//线性同余法最重要的是定义了三个整数，乘数 A、增量 B和模数 M，其中A, B, M是产生器设定的常数

int* mullerarray(int n)
{
	int M = 5231;//M越大（对1024来说），x1，x2分布越均匀;但考虑到rand函数理论上有一定输出分布，取到大数概率低太大会使x1，x2取0.5以下概率大于0.5以上
	double pi = 3.1415926535897932;
	double a[Num];
	int b[Num];
	double x1, x2,y;
	int p;
	for (int i = 0;i < Num;i++)
	{
		p = rand() % M;
		if(p==0)
		{
			x1 = 1;
		}
		else 
		{
			x1 = double(rand() % M) / double(M);
		}//x1为0则y无意义，故用x1=1代替
		x2 = double(rand() % (M)/double(M));
		y = sqrt(-2 * log(x1)) * cos(2 * pi * x2);
		//y = sqrt(-2 * log(x2))*sin(2 * pi*x1);  另一种产生正态分布随机数公式
		if (y < 0) 
		{
			a[i] = -y;
		}
		else
		{
			a[i] = y;
		}
		//方便起见，将正态分布改为单边的正态分布;否则删去此步骤，a[i] 恒= y即可
		b[i] = (int(a[i] + 0.5)) % (n + 1);
		//printf("%f ", x1);
		//printf("%f ", x2);
		//printf("%f ", y);
		//printf("%d\n", b[i]);
	}
	for (int i1 = 0;i1 < Num;i1++)
		printf("%d ", b[i1]);
	printf("\n");//测试正常输出
	return b;
}
// 设 X{1},X{2} 是在[0,1)上遵从均匀分布的随机数（生成[0,1)上遵从均匀分布的随机数可以使用rand），令
//Y{1} = sqrt{-2lnX{1}}cos(2piX{2}),Y{2}=sqrt{-2lnX{2}}cos(2piX{1})
//则Y{1},Y{2}必然遵守标准正态分布，y~N（0,1），即利用两个独立的遵从均匀分布的随机数得到两个独立的正态分布的随机数
// 其实有点问题，这是生成正态分布随机数之后再进行归一化，不太满足在对应区间进行正态分布输出的情况
//p(b=0)=-0.5到0.5上标准正态分布的积分约为0.383
//p(b=1)=0.5到1.5上标准正态分布的积分的两倍，约为0.483
//p(b=2)=1.5到2.5上标准正态分布的积分的两倍，约为0.121
//p(b=3)=2.5到3.5上标准正态分布的积分的两倍，约为0.012，其余概率太小，可以忽略


void numprobability(int n,int b[Num])
{
	int num;
	for (int i = 0;i <= n;i++)
	{
		num = 0;
		for (int j = 0;j < Num;j++)
		{
			if (b[j] == i)
				num++;
		}
		float p = float(num) / Num;
		int* arr = (int*)malloc(sizeof(int) * num);
		int e = 0, f = 0, g = 0,sum=0;
		float exp = 0;
		for (int k= 0;k< Num-1;k++)
		{
			if (b[k] == i)
			{
				e = k;
				arr[f] = e - g;
				f++;
				g = k;
			}
		}
		printf("%d出现次数：%d   ", i, num);
		printf("%d出现概率：%lf\n", i, p);
		if (num != 0) 
		{
			printf("间隔依次为：");
			for (int h = 1;h < num;h++)
			{
				printf("%d ", arr[h]);
				sum = sum + arr[h];
			}
			printf("\n");
			exp = float(sum) / float(num - 1);
			printf("期望间隔：%f\n", exp);
		}
	}
}

long double f1(long double x)
{
	if (x == 0) 
		return 0;//由洛必达，t->0,f(t)->0;
	else
	    return exp(-1/pow(x,2))/pow(x,2);//对反常积分，作变换f（t）=f（1/x）
}

long double f2(long double x,int n)
{
	if (x == 0)
		return 0;//由洛必达，t->0,f(t)->0;
	else
		return exp(-1 / pow(x, 2)) / pow(x, double(n/2)+1);//对反常积分，作变换f（t）=f（1/x）
}

long double gamma_half_n(int n)
{
	double pi = 3.1415926535897932;
	if(n%2==1)
	{
		if (n == 1)
			return pow(pi, 0.5);
		else
			return (double(n - 2) / 2) * gamma_half_n(n - 2);
	}
	if(n%2==0)
	{
		if (n == 2)
			return 1;
		else
			return (n/2-1)* gamma_half_n(n - 2);
	}
}

void bitcheck(int b[Num])
{
	int sum = 0;
	double v, z, xt, x0;
	double pi = 3.1415926535897932;
	for (int i = 0;i < Num;i++)
	{
		sum = sum + 2 * b[i] - 1;
	}
	v = double(abs(sum)) / pow(Num, 0.5);
	//printf("%f", v);
	z = v / pow(2, 0.5);
	xt = 1 / z;
	x0 = 0;
	int n = 100000;
	long double dx = (xt - x0) / n;
	long double sum1 = 0;
	for (long long k = 0; k < n; k++)
	{
		sum1 += ((f1(x0 + k * dx) + f1(x0 + (k + 1) * dx)) / 2) * dx;//由此计算数据较为粗糙
	}
	sum1 = 2 * sum1 / pow(pi, 0.5);
	printf("%lf\n", sum1);//检验计算值为0.70，实际值为0.59
	if (sum1 >= 0.01)//按我国规定，a=0.01
		printf("通过检测\n");
	else
		printf("未通过检测\n");
}

void blockcheck(int b[Num])
{
	int m = 100;
	int N;
	N = int(Num / m);
	double pi = 3.1415926535897932;
	int* arr = (int*)malloc(sizeof(int) * N);
	double v = 0;
	double sum2 = 0;
	double xt, x0;
	for (int i = 0;i < N;i++)
	{
		int sum1 = 0;
		int a1,a2,j;
		a1 = i * m;
		a2 = (i + 1) * m - 1;
		for (j = a1;j <= a2 ;j++)
		{
			sum1 += b[j];
		}
		if (arr)
		{
			arr[i] = sum1;
		}
		//printf("%d ", arr[i]);
		sum2 += double(4 * pow(double(sum1 -(double) m/2), 2)/m);
		v =(double)1 / sum2;
	}
	printf("%f\n", v);
	xt = v;
	x0 = 0;
	int n = 100000;
	long double dx = (xt - x0) / n;
	long double sum = 0;
	for (long long k = 0; k < n; k++)
	{
		sum += ((f2(x0 + k * dx,N) + f2(x0 + (k + 1) * dx,N)) / 2) * dx;//由此计算数据较为粗糙
	}
	sum = sum / (gamma_half_n(N)*pow(2,double(N/2)));
	printf("%e\n", sum);//检验计算值为2.1095*10^-9，实际值为2.1093*10^-9,非常准确
	if (sum >= 0.01)//按我国规定，a=0.01
		printf("通过检测\n");
	else
		printf("未通过检测\n");
}

void cusumcheck(int a[Num])
{
	int e=0;
	int limit = 5;
	double u;
	double sum=0;
	double value = 0;
	for(int i=0;i<Num;i++)
	{
		e += a[i];
	}
	u = double(e) / Num;
	//printf("%f\n", u);
	double up[Num],down[Num];
	up[0] = a[0] - u;
	down[0] = a[0] - u;
	for (int j = 1;j < Num;j++)
	{
		up[j] = up[j - 1] + a[j] - u;
		if (up[j] < 0)
			up[j] = 0;//上界小于零时则置零
	}
	for (int k = 1;k < Num;k++)
	{
		down[k] = down[k - 1] + a[k] - u;
		if (down[k] > 0)
			down[k] = 0;//下界大于零时也置零
	}
	for(int l=0;l<Num;l++)
	{
		if (up[l] > limit)
		{
			printf("%d ", l);
			//printf("%f ", up[l]);
			if (up[l + 1] <= limit)
				printf("\n");
		}
	}
	printf("这些地方上界可能有问题，1太多\n");
	for (int m = 0;m < Num;m++)
	{
		if (down[m] < -limit)
		{
			printf("%d ", m);
			//printf("%f ", down[m]);
			if (down[m + 1] >= -limit)
				printf("\n");
		}
	}
	printf("这些地方下界可能有问题，0太多\n");
}

int* youcheng(int b[Num])
{
	int a[Num];
	int j = 0;
	int l = 1;
	for(int m=0;m<Num;m++)
	{
		a[m] = 0;
	}
	for(int j=0;j<Num;j++)
	{
		if(b[j]==b[j+1])
		{
			l = l + 1;
		}
		else
		{
			a[l - 1] = a[l - 1] + 1;
			l = 1;
		}
	}
	for(int k=0;k<Num;k++)
	{
		if (a[k] != 0)
			printf("%d的游程有%d个\n", k + 1, a[k]);
	}
	return a;
}

void yccheck(int b[Num])
{
	int a[Num];
	memcpy(a, youcheng(b), sizeof(a));
	int r = 0;
	int n1 = 0, n0 = 0;
	double ex, var, v, p, p1, p2, p3;
	for (int k = 0;k < Num;k++)
	{
		if (a[k] != 0)
			r = r + a[k];
	}
	for (int i = 0;i < Num;i++)
	{
		if (b[i] == 1)
			n1++;
		else
			n0++;
	}
	ex = (double)(2 * n0 * n1 + Num) / Num;
	//printf("%lf\n", ex);
	p1 = (double)2 * n0 * n1;
	p2 = (double)(Num * Num * (Num - 1));
	p3 = p1 - Num;
	var = p1 * p3 / p2;
	//printf("%lf\n", var);
	v = pow(var, 0.5);
	p = double(r - ex) / v;//p遵循（0,1）的正态分布
	printf("%lf\n", p);
	if (p >= -1.96 && p <= 1.96)//误差在0.05时，标准正态分布取值范围是-1.96~1.96
		printf("通过检测");
	else
		printf("未通过检测");
}

int g(int x,int y)
{
	if (x == y)
		return 0;
	else
		return 1;
}
void zixiangguan(int b[Num],int i)
{
	int sum = 0;
	double v,v1,v2,v3;
	double z, x0, xt;
	double pi = 3.1415926535897932;
	for (int j = 0;j < Num - i;j++)
	{
		sum += g(b[j], b[j + i]);
	}
	v1 = (double)(Num - i) / 2;
	v2 = (double)2 * (sum - v1);
	v3 = pow(Num - i, 0.5);
	v = v2 / v3;
	z = v / pow(2, 0.5);
	xt = 1 / z;
	x0 = 0;
	int n = 100000;
	long double dx = (xt - x0) / n;
	long double sum1 = 0;
	for (long long k = 0; k < n; k++)
	{
		sum1 += ((f1(x0 + k * dx) + f1(x0 + (k + 1) * dx)) / 2) * dx;//由此计算数据较为粗糙
	}
	sum1 = 2 * sum1 / pow(pi, 0.5);
	printf("%lf\n", sum1);//检验计算值为0.70，实际值为0.59
	if (sum1 >= 0.01)//按我国规定，a=0.01
		printf("通过检测\n");
	else
		printf("未通过检测\n");
}

int main()
{
	//srand((int)time(0));
	srand(1);//为方便调试固定rand种子，保证输出不变。正式输出可用以上代码替换
	long double start1 = clock();  //开始时间
	int n, b[Num],d1,d2;
	printf("请输入随机数范围");
	scanf_s("%d", &n);
	printf("随机数生成方式：1rand函数  2线性同余法  3box-muller法");
	scanf_s("%d", &d1);
	if (d1 == 1) 
	{
		memcpy(b, randomarray(n), sizeof(b));//将函数生成随机数组拷贝到主函数b[Num]中
	}
	if (d1 == 2)
	{
		memcpy(b, linealarray(n), sizeof(b));
	}
	if (d1 == 3)
	{
		memcpy(b, mullerarray(n), sizeof(b));
	}
	long double finish1 = clock();   //结束时间
	long double t = finish1 - start1;
	printf("The run time is:%lf ms!\n",t); //输出时间
	long double start2 = clock();  //开始时间
	printf("测试方式：1频率与间隔  2单比特频数检测  3块内频率测试  4累积和检测  5游程检测  6自相关检测\n");
	printf("后5种方式仅适用于01分布的随机数\n");
	scanf_s("%d", &d2);
	if (d2 == 1) 
	{
		numprobability(n, b);
	}
	if (d2 == 2)
	{
		if (n != 1)
		{
			printf("请确认随机数取值只有0和1\n");
		}
		else
		{
			bitcheck(b);
		}
	}
	if (d2 == 3)
	{
		if (n != 1)
		{
			printf("请确认随机数取值只有0和1\n");
		}
		else
		{
			blockcheck(b);
		}
	}
	if (d2 == 4)
	{
		if (n != 1)
		{
			printf("请确认随机数取值只有0和1\n");
		}
		else
		{
			cusumcheck(b);
		}
	}
	if (d2 == 5)
	{
		if (n != 1)
		{
			printf("请确认随机数取值只有0和1\n");
		}
		else
		{
			yccheck(b);
		}
	}
	if (d2 == 6)
	{
		if (n != 1)
		{
			printf("请确认随机数取值只有0和1\n");
		}
		else
		{
			int k;
			printf("右移多少位");
			scanf_s("%d", &k);
			zixiangguan(b,k);
		}
	}
	long double finish2 = clock();   //结束时间
	long double t1 = finish2 - start2;
	printf("The run time is:%lf ms!\n",t1); //输出时间
	return 0;
}