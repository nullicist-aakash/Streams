export module stream:filter;
import :common;

template <typename begin_type, typename end_type, typename predicate_type> 
    requires valid_iterator<begin_type, end_type>
class filter_iterator
{
    begin_type m_iterator;
    const end_type& m_end;
    const predicate_type& predicate;
    static_assert(std::invocable<predicate_type, decltype(*m_iterator)>, "Predicate is not invocable with the value type of the container!");
    static_assert(std::same_as<std::remove_cvref_t<decltype(std::invoke(predicate, *m_iterator))>, bool>, "Predicate does not return a boolean value!");

public:
    constexpr filter_iterator(const begin_type& iterator, const end_type& end, const predicate_type& predicate) : m_iterator{ iterator }, m_end{ end }, predicate{ predicate }
    {
        while (m_iterator != m_end && !std::invoke(predicate, *m_iterator))
            ++m_iterator;
    }

    constexpr auto operator*() const { return *m_iterator; }

    constexpr auto& operator++()
    {
        while (++m_iterator != m_end && !std::invoke(predicate, *m_iterator));
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const filter_iterator& a, sentinel end) { return a.m_iterator == a.m_end; };
    static friend constexpr auto operator!= (const filter_iterator& a, sentinel end) { return a.m_iterator != a.m_end; };
};

template <typename begin_type, typename end_type, typename predicate_type> 
    requires valid_iterator<begin_type, end_type>
struct filter_container
{
    const begin_type m_begin;
    const end_type m_end;
    const predicate_type predicate;
    static_assert(std::invocable<predicate_type, decltype(*m_begin)>, "Predicate is not invocable with the value type of the container!");
    static_assert(std::same_as<std::remove_cvref_t<decltype(std::invoke(predicate, *m_begin))>, bool>, "Predicate does not return a boolean value!");

    constexpr auto begin() const { return filter_iterator(m_begin, m_end, predicate); }
    constexpr auto end() const { return sentinel{}; }
};
