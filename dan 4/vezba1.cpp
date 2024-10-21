#include <iostream>
#include <sstream>
#include <type_traits>
#include <cassert>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

// koristio za nacin kao na prezentaciji
/*template <typename T>
auto sum(T t) {
	return t;
}
template <typename T, typename... Ts> 
auto sum(T t, Ts... args) {
	return t + sum(args...);
}
*/

enum class Types {STRING, INTEGER, DOUBLE};
std::unordered_map<char, Types> type_map {
	{'d', Types::INTEGER},
	{'f', Types::DOUBLE},
	{'s', Types::STRING}
};

// should expand to (a + (b + (c + d))?
template <typename... Ts>
auto sum(Ts... args) {
	return (args + ...);
}

template<typename T>
auto min(T t) {
	return t;
}

// nisam uspeo sa fold expression..., malo je komplikovano sa pozivom funkcije (za min)
template<typename T, typename... Ts>
auto min(T t, Ts... args) {
	auto others = min(args...);
	// odmah kastuje, bez stdmin implementacija
	return t < others? t : others;
}

int my_printf(const std::string format, std::stringstream& result) {
	int pos = format.find('%');
	if(pos != std::string::npos) {
		std::cout << "Invalid format\n";
		return 1;
	}

	result << format;
	
	std::cout << result.str();

	return 0;
}

template <typename T, typename... Ts>
int my_printf(const std::string format, std::stringstream& result, const T& t, const Ts&... args) {
	int pos = format.find('%');
	if(pos == std::string::npos || pos == (format.length() - 1)) {
		std::cout << "Invalid format\n";
		return 1;
	}

	result << format.substr(0, pos);

	pos++;
	if(!type_map.count(format[pos])) {
		std::cout << "Invalid format\n";
		return 1;
	}

	bool error = false;


	switch(type_map.at(format[pos])) {
	case Types::STRING: 
		if (!std::is_constructible<std::string, std::remove_reference_t<T>>::value) 
			error = true;
		break;
	case Types::INTEGER:
		if (!std::is_integral<std::remove_reference_t<T>>::value) 
			error = true;
		break;
	case Types::DOUBLE:	
		if (!std::is_floating_point<std::remove_reference_t<T>>::value) 
			error = true;
		break;
	}

	if(error) {
		std::cout << "Invalid format\n";
		return 1;
	}

	result << t;

	return my_printf(format.substr(pos + 1, format.length() - pos), result, args...);
}

template <typename... Ts>
int my_printf(const std::string format, const Ts&... args) {
	std::stringstream s;
	return my_printf(format, s, args...);
}

int my_printf(const std::string format) {
	std::stringstream s;
	return my_printf(format, s);
}

// typechecking mora u runtime, za slucaj da korisnik prosledi format  
// takodje, zbog std::string.
template<typename... Ts>
int myprintf_iter(const std::string format, const Ts&... args) {
	std::vector<Types> types;
	// svakako cu morati da ih ubacim u string, mogu odmah da konvertujem u string
	std::vector<std::string> arg_strings;

	enum class States {FORWARD, INSERT};
	States state = States::FORWARD;
	std::string result = "";
	// u dva prolaza, prvo "semanticka analiza" pa onda "generisanje koda"
	
	for(char c : format) {
		switch(state) {
		case States::FORWARD:
			if(c == '%') {
				state = States::INSERT;
			}  

			break;
		case States::INSERT:
			types.push_back(type_map[c]);
			state = States::FORWARD;
			break;
		}
	}

	// prvi prolaz
	assert((sizeof...(Ts) == types.size()));


	int i = 0;
	([&arg = args, &i = i, &types = types, &arg_strings = arg_strings]() {
	 	switch(types[i]) {
		case Types::INTEGER:
			assert(std::is_integral<std::remove_reference_t<decltype(arg)>>::value); 
			if constexpr (std::is_integral<std::remove_reference_t<decltype(arg)>>::value) arg_strings.push_back(std::to_string(arg));
			break;	
		case Types::DOUBLE:
			assert(std::is_floating_point<std::remove_reference_t<decltype(arg)>>::value); 
			if constexpr (std::is_floating_point<std::remove_reference_t<decltype(arg)>>::value) arg_strings.push_back(std::to_string(arg));
			break;
		case Types::STRING:
			// ne radi savrseno, ali alternativa je typeid
			assert((std::is_constructible<std::string, std::remove_reference_t<decltype(arg)>>::value));
			if constexpr (std::is_constructible<std::string, std::remove_reference_t<decltype(arg)>>::value) arg_strings.push_back(arg);
			break;
		}	
		i++;
	 }(),...);

	i = 0;
	state = States::FORWARD;
	for(char c : format) {
		switch(state) {
		case States::FORWARD:
			if(c == '%') {
				state = States::INSERT;
			} else {
				result += c;
			}
			break;
		case States::INSERT:
			result += arg_strings[i++];
			state = States::FORWARD;
		}
	}

	std::cout << result;

	return 0;
}


int main() {
	// sum
	std::cout << sum(2, 1.8, 3.8, 4, 5.8) << std::endl;
	std::cout << sum(1.8, 2, 3.8, 4, 5.8) << std::endl;

	// min
	std::cout << min(2.2, 1.1, 4, 4, 58) << std::endl;
	std::cout << min(2.2, 1.1, 4, 4, -58) << std::endl;

	// iter myprintf
	myprintf_iter("Simple check\nNew line\nAgain new line\n");
	// baca assert
	// myprintf_iter(" text %s, integral value %d than something %d\n", "SUCCESSFULLY", 85);

	// baca assert
	// myprintf_iter("Some text %s, some floating point number %f again something %k\n", "45", 89.7, 56);
	myprintf_iter("Some text %s, some floating point number %f again something %d\n", "45", 89.7, 56);
	
	// normalna implementacija
	std::cout << std::endl << std::endl;

	my_printf("Simple check\nNew line\nAgain new line\n");
	my_printf(" text %s, integral value %d than something %d\n", "SUCCESSFULLY", 85);

	my_printf("Some text %s, some floating point number %f again something %k\n", "45", 89.7, 56);
	my_printf("Some text %s, some floating point number %f again something %d\n", "45", 89.7, 56);


	return 0;
}
