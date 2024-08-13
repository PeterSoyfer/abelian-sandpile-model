#include <iostream>
#include <cmath>
//#include <tgmath.h> //for long double tgammal function
#include <algorithm>
#include <vector>
//#include <string>


int factorial(int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}


int combs(int n, int k)
{
	return factorial(n) / (factorial(k) * factorial(n - k));
}


int comb_formula(int n, int b, int a)
{
	int answer = 0;
	
	for (int k = 0; k <= n; ++k)
	{
		if (b - k*a < 0)
			return -1;
		
		answer += combs(n, k) * ((int)pow(-1, k)) * combs(n + b - k*a - 1, b - k*a);
	}
	
	return answer;
}


int combs2(int, int);

void print_combs(int, int);


void print_vector(std::vector<int>& v)
{
	for (int number : v)
		std::cout << number << ' ';
}


int digits_sum(std::vector<int>& digits)
{
	int res = 0;
	
	for (int digit : digits)
		res += digit;
	
	return res;
}


std::vector<int> expansion(int number, int base)
{
	std::vector<int> ans;
	int tmp = number;

	if (tmp >= base)
	{
		int digit = 0;
		
		while (tmp / base != 0)
		{
			digit = tmp % base;
			ans.push_back(digit);
			tmp = (tmp - digit) / base;
		}
	}
	
	ans.push_back(tmp);
	std::reverse(ans.begin(), ans.end());
	
	return ans;
}


std::vector<std::vector<int>> gennums(int len, int base)
{
	long long int total_amount = pow(base, len);
	std::vector<std::vector<int>> numbers(total_amount, std::vector<int>(len, 0));
	std::vector<int> tmp;
	
	for (int i = 0; i < total_amount; ++i)
	{
		tmp = expansion(i, base);
		auto start_pos = numbers[i].end() - tmp.size();
		std::copy(tmp.begin(), tmp.end(), start_pos);
	}
	
	return numbers;
}


int siftout(std::vector<std::vector<int>> numbers, int sum)
{
	int count = 0;
	
	std::cout << '\n';
	
	for (int i = 0; i < numbers.size(); ++i)
	{
		if (digits_sum(numbers[i]) == sum)
		{
			++count;
			//print_vector(numbers[i]);
			//std::cout << '\n';
		}
	}
	
	return count;
}


int main(void)
{
	int N, max_height, files_sum, result;
	
	std::cout << "\nEnter the length N: ";
	std::cin >> N;
	
	std::cout << "\nEnter the maximal height: ";
	std::cin >> max_height;
	
	files_sum = 2*N - 1;
		
	result = siftout(gennums(N, max_height + 1), files_sum);
	
	std::cout << "\nThe number of sifted configurations is: ";
	std::cout << result;
	std::cout << '\n';
	
	/*
	result2 = comb_formula(N, files_sum, max_height + 1);
	
	if (result2 == -1)
		std::cerr << "\nImpossible combinatorics!!!\n";
		
	else
	{
		std::cout << "\nThe combinatorical result is: ";
		std::cout << result2;
		std::cout << '\n';
	}
	*/
	
	return 0;
}


int combs2(int n, int k)
{
	return ((int) tgamma(n + 1)) / ( ((int) tgamma(k + 1)) * ((int) tgamma(n - k + 1)) );
}


void print_combs(int n, int k)
{
    std::vector<bool> v(n);
    std::fill(v.end() - k, v.end(), true);

    do
    {
		for (int i = 0; i < n; ++i)
			if (v[i])
				std::cout << (i + 1) << " ";
        std::cout << "\n";
	}
	while (std::next_permutation(v.begin(), v.end()));
}

