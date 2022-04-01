#pragma once
#include <iostream>
# define RED      "\033[1;31m"
# define GREEN    "\033[0;32m"
# define RESET    "\033[0;0m"

void put( bool x) {
	(x && (std::cout << GREEN "OK!" RESET << std::endl)) || (std::cout << RED "FAIL!" RESET << std::endl);
}

template <bool condition, typename Type>
struct EnableIf;

char begin(...);

template <typename T>
struct IsForReady {
	static constexpr bool value
			= sizeof(begin(std::declval<T>())) != sizeof(char);
};

template <typename Type>
struct EnableIf<true, Type> {
	using type = Type;
};

template <typename T>
typename EnableIf<IsForReady<T>::value, void>::type p(T container) {
	std::cout << "Values:{ ";
	for(auto value : container)
		std::cout << value << " ";
	std::cout << "}\n";
}

template <typename T>
typename EnableIf<!IsForReady<T>::value, void>::type p(T value) {
	std::cout << "";
	std::cout << value << "\n";
}

//template <class T>
//void p(const T & t) { std::cout << t << std::endl; }

template <class T, class T2> 
void p(const T & t, const T2 & t2) { std::cout << t  << t2 << std::endl; }

template <class T, class T2, class T3> 
void p(const T & t, const T2 & t2, const T3 & t3) { std::cout << t << ' ' << t2  << ' ' << t3 << std::endl; }

// interesting way - nested template
///template <typename T, typename Alloc, template <typename, typename> class V>
///void print_container(V<T, Alloc> & a) {
///	for (typename V<T, Alloc>::iterator i = a.begin(); i != a.end(); i++)
///		std::cout << *i << " ";
///	std::cout << std::endl;;
///}

template <typename T>
void print_container(T & a) {
	std::cout << "values: ";
	for (typename T::iterator i = a.begin(); i != a.end(); i++)
		std::cout << *i << " ";
	std::cout << std::endl;;
}
