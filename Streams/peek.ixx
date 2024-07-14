export module stream:peek;
import :common;

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>&& std::invocable<transform_type, decltype(*std::declval<begin_type>())>
class peek_iterator
{
    using mapped_type = std::invoke_result_t<transform_type, decltype(*std::declval<begin_type>())>;

    begin_type m_iterator;
    end_type m_end;
    transform_type peeker;

public:
    constexpr peek_iterator(begin_type iterator, const end_type& end, const transform_type& peeker) : m_iterator{ iterator }, m_end{ end }, peeker{ peeker } { }

    constexpr auto operator*() const 
    {
        auto ret = *m_iterator;
        std::invoke(peeker, ret); 
        return ret;
    }

    constexpr auto& operator++()
    {
        ++m_iterator;
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const peek_iterator& a, const end_type& end) { return a.m_iterator == end; };
    static friend constexpr auto operator!= (const peek_iterator& a, const end_type& end) { return a.m_iterator != end; };
};

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>&& std::invocable<transform_type, decltype(*std::declval<begin_type>())>
struct peek_container
{
    const begin_type m_begin;
    const end_type m_end;
    const transform_type peeker;

    constexpr auto begin() const { return peek_iterator(m_begin, m_end, peeker); }
    constexpr auto end() const { return m_end; }
};
