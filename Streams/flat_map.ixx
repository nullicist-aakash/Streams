export module stream:flat_map;
import :common;
import std;

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>
    && std::invocable<transform_type, decltype(*std::declval<begin_type>())>
class flat_map_iterator
{
    using mapped_type = std::invoke_result_t<transform_type, decltype(*std::declval<begin_type>())>;

    begin_type m_iterator;
    const end_type& m_end;
    const transform_type& transform;
    
    static_assert(is_container<std::remove_cvref_t<decltype(std::invoke(transform, *m_iterator))>>, "Result type is not a iterable thing!");
public:
    constexpr flat_map_iterator(begin_type iterator, const end_type& end, const transform_type& transform) : m_iterator{ iterator }, m_end{ end }, transform{ transform } {
        
    }

    constexpr auto operator*() const { return std::invoke(transform, *m_iterator); }

    constexpr auto& operator++()
    {
        ++m_iterator;
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const flat_map_iterator& a, sentinel end) { return a.m_iterator == a.m_end; };
    static friend constexpr auto operator!= (const flat_map_iterator& a, sentinel end) { return a.m_iterator != a.m_end; };
};

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>
    && std::invocable<transform_type, decltype(*std::declval<begin_type>())>
struct flat_map_container
{
    const begin_type m_begin;
    const end_type m_end;
    const transform_type transform;

    static_assert(is_container<std::remove_cvref_t<decltype(std::invoke(transform, *m_begin))>>, "Result type is not a iterable thing!");

    constexpr auto begin() const { return flat_map_iterator(m_begin, m_end, transform); }
    constexpr auto end() const { return sentinel{}; }
};
