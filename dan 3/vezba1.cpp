#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <numeric>

void printVector(const std::vector<int>& vec) {
	std::for_each(vec.begin(), vec.end(), 
		[] (const int elem) -> void {
			std::cout << elem << " ";
		});	
	std::cout << std::endl;
}

int main() {
	std::vector<int> prviVektor(20, 0);
	int start = 5;
	std::for_each(prviVektor.begin(), prviVektor.end(), 
			[&start = start] (int& elem) -> void {
				elem = start;
				start += 5;
			});

	printVector(prviVektor);

	std::for_each(prviVektor.begin(), prviVektor.end(), 
			[] (int& elem) -> void {
				elem = std::pow(elem, 2);
			});

	printVector(prviVektor);

	std::vector<int> drugiVektor;
	std::copy_if(prviVektor.begin(), prviVektor.end(), std::back_inserter(drugiVektor),
			[](const int elem) -> bool {
				return elem % 7 != 0;
			});

	printVector(drugiVektor);

	double reciprocnaSuma;
	reciprocnaSuma = std::accumulate(drugiVektor.begin(), drugiVektor.end(), 0.0, 
			[](const double a, const double b) -> double {
				return a + 1.0 / b;
			});

	std::cout << reciprocnaSuma << std::endl;

	drugiVektor.erase(std::remove_if(drugiVektor.begin(), drugiVektor.end(), 
			[](const int elem) -> bool {
				return elem % 9 == 0;
			}), drugiVektor.end());

	printVector(drugiVektor);

	auto it = std::find_if(drugiVektor.begin(), drugiVektor.end(), 
			[](const int elem) -> bool {
				return elem % 13 == 0;
			});
	
	int index = it == drugiVektor.end()? -1 : std::distance(drugiVektor.begin(), it);
	std::cout << index << std::endl;

	return 0;
}
