export module stream:range;
import std;

template <std::integral basic_type>
struct range_iterator
{
private:
    basic_type val;
    const std::uint64_t steps;
public:
    constexpr range_iterator(basic_type value, std::uint64_t steps) : val(value), steps(steps) {}
    constexpr auto operator*() const { return val; }

    constexpr auto& operator++() { val += steps; return *this; }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const range_iterator& a, basic_type end) { return a.val >= end; };
    static friend constexpr auto operator!= (const range_iterator& a, basic_type end) { return a.val < end; };
};

export template <std::integral basic_type>
struct range
{
    const basic_type first;
    const basic_type last;
    const std::uint64_t steps = 1ull;

    constexpr auto begin() const { return range_iterator{ first, steps }; }
    constexpr auto end() const { return last; }
};
