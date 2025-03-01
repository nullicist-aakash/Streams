export module stream:map;
import :common;

template <typename begin_type, typename end_type, typename transform_type>
    requires is_valid_iterator<begin_type, end_type>
class map_iterator
{
    using mapped_type = std::invoke_result_t<transform_type, decltype(*std::declval<begin_type>())>;
    
    begin_type m_iterator;
    end_type m_end;
    transform_type transform;

public:
    constexpr map_iterator(begin_type iterator, const end_type& end, const transform_type& transform) : m_iterator{ iterator }, m_end{ end }, transform{ transform } { }

    constexpr auto operator*() const { return std::invoke(transform, *m_iterator); }

    constexpr auto& operator++()
    {
        ++m_iterator;
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const map_iterator& a, sentinel end) { return a.m_iterator == a.m_end; };
    static friend constexpr auto operator!= (const map_iterator& a, sentinel end) { return a.m_iterator != a.m_end; };
};

template <typename begin_type, typename end_type, typename transform_type>
    requires is_valid_iterator<begin_type, end_type>
struct map_container
{
    const begin_type m_begin;
    const end_type m_end;
    const transform_type transform;

    constexpr auto begin() const { return map_iterator(m_begin, m_end, transform); }
    constexpr auto end() const { return sentinel{}; }
};
