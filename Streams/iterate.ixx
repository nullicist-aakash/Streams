export module stream:iterate;
import :common;
import std;

template <typename T, typename op_type> requires std::invocable<op_type, T>
class iterate_iterator
{
    T seed;
    const op_type op;
    static_assert(std::same_as<std::remove_cvref_t<decltype(std::invoke(op, seed))>, T>, "Operator does not return same type as input!");

public:
    constexpr iterate_iterator(T seed, op_type op) : seed{ seed }, op{ op }
    {

    }

    constexpr auto operator*() const { return seed; }

    constexpr auto& operator++()
    {
        seed = std::invoke(op, seed);
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    friend constexpr auto operator== (const iterate_iterator& a, sentinel end) { return false; };
    friend constexpr auto operator!= (const iterate_iterator& a, sentinel end) { return true; };
};

template <typename T, typename op_type> requires std::invocable<op_type, T>
struct iterate_container
{
    using input_type = std::remove_cvref_t<T>;

    const T&& seed;
    const std::remove_cvref_t<op_type> op;
    static_assert(std::same_as<std::remove_cvref_t<decltype(std::invoke(op, seed))>, input_type>, "Operator does not return same type as input!");

    constexpr iterate_container(T&& seed, op_type&& op) : seed{ std::forward<T>(seed) }, op{ std::forward<op_type>(op) } {}
    constexpr auto begin() const { return iterate_iterator(seed, op); }
    constexpr auto end() const { return sentinel{}; }
};
