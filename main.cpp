#include <iostream>
#include <array>
#include <type_traits>

#define TEMPLATES_BRIEF

#ifdef TEMPLATES_BRIEF
template <typename T, size_t N>
void my_func(std::array<T, N> &arr) {
    for (auto &elem : arr) { std::cout << elem << " "; }
    std::cout << std::endl;
}

template <>
void my_func<int, 3>(std::array<int, 3> &arr) {
    for (auto &elem : arr) { std::cout << elem << " "; }
    std::cout << std::endl;
}

template <>
void my_func<int, 2>(std::array<int, 2> &arr) {
    for (auto &elem : arr) { std::cout << elem << " "; }
    std::cout << std::endl;
}

template <>
void my_func<double, 2>(std::array<double, 2> &arr) {
    for (auto &elem : arr) { std::cout << elem << " "; }
    std::cout << std::endl;
}
#endif

int main() {
#ifdef TEMPLATES_BRIEF
    std::array arr1 = {1, 2, 3};
    std::array arr2 = {4, 5};
    std::array arr3 = {0.5, 3.5};
    my_func(arr1);
    my_func(arr2);
    my_func(arr3);
#endif
    return EXIT_SUCCESS;
}