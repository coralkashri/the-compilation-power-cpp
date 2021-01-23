#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <ranges>
#include <type_traits>

#define CONSTEXPR_AND_CONSTEVAL

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
#elif defined(TEMPLATE_ARGUMENT_RESTRICTIONS)
/// @brief Convert a container of a specific type to the same container of another type.
template <typename DstCont, typename SrcCont>
DstCont cast_all(SrcCont &src_collection) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    DstCont dst(src_collection.begin(), src_collection.end());
    return dst;
}

/// @brief Convert a container of a specific type to the same container of another type.
template <typename OutContType, template <typename...> typename Cont, typename InContType>
Cont<OutContType> cast_all_v2(Cont<InContType> &src_collection) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    Cont<OutContType> dst(src_collection.begin(), src_collection.end());
    return dst;
}

template <typename T>
concept IsPrintable = requires(T t) {
    { std::cout << t };
};

template <IsPrintable ...Args>
void print_me(Args&& ...args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

template <typename ...Args, typename = std::enable_if_t<(std::is_arithmetic_v<Args> && ...)>>
auto sum(Args&& ...args) {
    return (args + ...);
}
#elif defined(CONSTEXPR_AND_CONSTEVAL)
template <typename T> concept Number = std::is_arithmetic_v<T>;
template <typename Op> concept Operator = requires(Op op) { { op(1, 2) } -> std::same_as<int>; };

template <Number T>
class my_number_class {
public:
    constexpr my_number_class(T val) : my_val(val) {}

    template <Operator BinaryOperation>
    [[nodiscard]] constexpr my_number_class op(BinaryOperation op, Number auto num) const {
        return my_number_class(op(my_val, num));
    }

    [[nodiscard]] constexpr T get_val() const { return my_val; }

private:
    T my_val;
};
#elif defined(CONCEPTS_EXTENSION)
class interface {
public:
    explicit interface(std::string &&name) : my_name(name) {}
    virtual void print_me() const = 0;
    [[nodiscard]] std::string get_my_name() const { return my_name; }

private:
    std::string my_name;
};

class derived_a : public interface {
public:
    explicit derived_a(std::string &&name) : interface(std::move(name)) {}
    void print_me() const override { std::cout << "Derived A: " << get_my_name() << "\n"; }
};

class derived_b : public interface {
public:
    explicit derived_b(std::string &&name) : interface(std::move(name)) {}
    void print_me() const override { std::cout << "Derived B: " << get_my_name() << "\n"; }
};

template <typename T>
concept InterfaceDerived = std::is_base_of_v<interface, T>;

template <InterfaceDerived Base>
class my_special_class : public Base {
public:
    explicit my_special_class(std::string &&name) : Base(std::move(name)) {}
    void print_me() const override { std::cout << "My special class: " << this->get_my_name() << "\n"; }
};

template <typename T>
concept Interface = requires(T t) {
    { T(std::move(std::string())) };
    { t.print_me() };
    { t.get_my_name() } -> std::same_as<std::string>;
};

template <Interface Base>
class my_special_class_2 : public Base {
public:
    explicit my_special_class_2(std::string &&name) : Base(std::move(name)) {}
    void print_me() const { std::cout << "My special class: " << this->get_my_name() << "\n"; }
};
#endif

int main() {
#ifdef TEMPLATES_BRIEF
    std::array arr1 = {1, 2, 3};
    std::array arr2 = {4, 5};
    std::array arr3 = {0.5, 3.5};
    my_func(arr1);
    my_func(arr2);
    my_func(arr3);
#elif defined(TEMPLATE_ARGUMENT_RESTRICTIONS)
    std::vector<int> i_vec = {1, 2, 3, 4, 5};
    auto d_vec = cast_all<std::vector<double>>(i_vec); // Compiles - Legal usage
    auto d_list = cast_all<std::list<double>>(i_vec); // Compiles - Illegal usage

    auto d_vec2 = cast_all_v2<double>(i_vec);

    print_me("34", 52.6, 3.2f);

    std::cout << sum(1, 50, 2.5, 3, 1.2f) << "\n";
#elif defined(CONSTEXPR_AND_CONSTEVAL)
    constexpr my_number_class compiletime_number(3);
    constexpr my_number_class number_inc = compiletime_number.op(std::plus<>{}, 3);
    constexpr my_number_class number_dec = compiletime_number.op(std::minus<>{}, 3);
    constexpr my_number_class number_mul = compiletime_number.op(std::multiplies<>{}, 3);
    constexpr my_number_class number_div = compiletime_number.op(std::divides<>{}, 3);
    constexpr my_number_class number_custom = compiletime_number.op([](auto n1, auto n2) {
        return (n1 + n2) * n2 * n1;
    }, 3);
    std::cout << "Inc: " << number_inc.get_val() << "\n";
    std::cout << "Minus: " << number_dec.get_val() << "\n";
    std::cout << "Mul: " << number_mul.get_val() << "\n";
    std::cout << "Div: " << number_div.get_val() << "\n";
    std::cout << "Custom: " << number_custom.get_val() << "\n";
    int runtime_num = 50;
    my_number_class runtime_number(runtime_num);                                                     // Won't compile - Consteval must be evaluated on compile time.
#elif defined(CONCEPTS_EXTENSION)
    struct no_derived {
        no_derived(std::string &&str) {}

        void print_me() { std::cout << "NO\n"; }

        std::string get_my_name() const { return ""; }
    };
    struct test {};
    // Interface auto p = test(); // Won't compile
    Interface auto p = no_derived("");
    p.print_me();
    my_special_class<derived_a> msc("MyName");
    my_special_class_2<no_derived> msc1("MyName");
#endif
    return EXIT_SUCCESS;
}