#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>



int main()
{
	std::cout << "Misha and Ani will receive 100 for this project\n";

	unsigned long long sum = 0;
	
	for (std::size_t i = 0; i < 5; ++i)
		sum += i;

	std::cout << sum;
}