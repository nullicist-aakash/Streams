export module stream:generate;
import :common;
import std;

template <typename op_type>
class generate_iterator
{
    const op_type op;
    decltype(std::invoke(op)) val;

public:
    constexpr generate_iterator(op_type op) : op{ op }, val{std::invoke(this->op)}
    {

    }

    constexpr auto operator*() const { return val; }

    constexpr auto& operator++()
    {
        val = std::invoke(op);
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    friend constexpr auto operator== (const generate_iterator& a, sentinel end) { return false; };
    friend constexpr auto operator!= (const generate_iterator& a, sentinel end) { return true; };
};

template <typename op_type>
struct generate_container
{
    const op_type op;
    
    constexpr auto begin() const { return generate_iterator(op); }
    constexpr auto end() const { return sentinel{}; }
};
