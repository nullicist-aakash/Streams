export module stream;
import :range;
import :filter;
import :map;

export template <typename Container> requires requires(Container cont) { cont.begin(); cont.end(); }
class stream
{
    using value_type = decltype(*std::declval<Container>().begin());
    using begin_type = decltype(std::declval<Container>().begin());
    using end_type = decltype(std::declval<Container>().end());

    std::optional<Container> mm_container;
    begin_type m_begin;
    end_type m_end; 

    template <typename OtherContainer> requires requires(OtherContainer cont) { cont.begin(); cont.end(); }
    friend class stream;

    template <std::integral basic_type>
    friend stream<range<basic_type>> int_stream(basic_type start, basic_type end, std::uint64_t steps);
    
    constexpr stream(Container&& container) : mm_container{ std::move(container) }, m_begin{ mm_container->begin() }, m_end{ mm_container->end() } {}
public:
    constexpr stream(Container& container) : mm_container{ std::nullopt }, m_begin{ container.begin() }, m_end{ container.end() } {}
    constexpr auto begin() const { return m_begin; }
    constexpr auto end() const { return m_end; }

    constexpr auto filter(const auto& predicate) const
    {
        auto container = filter_container{ m_begin, m_end, predicate };
        return stream<decltype(container)>{ std::move(container) };
    }

    constexpr auto map(const auto& transform) const
	{
        auto container = map_container{ m_begin, m_end, transform };
        return stream<decltype(container)>{ std::move(container) };
	}
};

export template <std::integral basic_type>
stream<range<basic_type>> int_stream(basic_type start, basic_type end, std::uint64_t steps = 1) { return stream{ range<basic_type>{start, end, steps} }; }