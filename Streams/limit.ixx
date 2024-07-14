export module stream:limit;
import :common;
import std;

template <typename begin_type, typename end_type>
    requires valid_iterator<begin_type, end_type>
class limit_iterator
{
    begin_type m_iterator;
    end_type m_end;
    int remain;

public:
    constexpr limit_iterator(const begin_type& iterator, const end_type& end, const int& limit) : m_iterator{ iterator }, m_end{ end }, remain{ std::max(0, limit) } { }

    constexpr auto operator*() const { return *m_iterator; }

    constexpr auto& operator++()
    {
        if (m_iterator != m_end && remain-- > 0)
            ++m_iterator;
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    friend constexpr auto operator== (const limit_iterator& a, end_type end) { return a.remain == 0 || a.m_iterator == end; };
    friend constexpr auto operator!= (const limit_iterator& a, end_type end) { return !(a == end); };
};

template <typename begin_type, typename end_type>
    requires valid_iterator<begin_type, end_type>
struct limit_container
{
    const begin_type m_begin;
    const end_type m_end;
    const int limit;

    constexpr auto begin() const { return limit_iterator(m_begin, m_end, limit); }
    constexpr auto end() const { return m_end; }
};
