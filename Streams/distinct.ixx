export module stream:distinct;

import :common;

template <typename begin_type, typename end_type> requires valid_iterator<begin_type, end_type>
class distinct_iterator
{
    begin_type m_iterator;
    end_type m_end;
    std::set<std::remove_cvref_t<decltype(*m_iterator)>> m_set;

public:
    constexpr distinct_iterator(const begin_type& iterator, const end_type& end) : m_iterator{ iterator }, m_end{ end }
    {

    }

    constexpr auto operator*() const { return *m_iterator; }

    constexpr auto& operator++()
    {
        m_set.insert(*m_iterator);
        while (++m_iterator != m_end && m_set.contains(*m_iterator));
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const distinct_iterator& a, const end_type& end) { return a.m_iterator == end; };
    static friend constexpr auto operator!= (const distinct_iterator& a, const end_type& end) { return a.m_iterator != end; };
};

template <typename begin_type, typename end_type> requires valid_iterator<begin_type, end_type>
struct distinct_container
{
    const begin_type m_begin;
    const end_type m_end;

    constexpr auto begin() const { return distinct_iterator(m_begin, m_end); }
    constexpr auto end() const { return m_end; }
};
