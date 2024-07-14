export module stream;
import :filter;
import :map;
import :limit;
import :iterate;
import std;

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

    template <typename T, typename op_type>
    friend constexpr auto seed_stream(T&& seed, op_type&& op);

    constexpr stream(Container&& container) : mm_container{ std::move(container) }, m_begin{ mm_container->begin() }, m_end{ mm_container->end() } {}
public:
    constexpr stream(Container& container) : mm_container{ std::nullopt }, m_begin{ container.begin() }, m_end{ container.end() } {}
    constexpr auto begin() const { return m_begin; }
    constexpr auto end() const { return m_end; }

    template <typename T> requires std::invocable<T, value_type>
    constexpr auto filter(const T&& predicate) const
    {
        auto container = filter_container{ m_begin, m_end, predicate };
        return stream<decltype(container)>{ std::move(container) };
    }

    template <typename T> requires std::invocable<T, value_type>
    constexpr auto map(T&& transform) const
    {
        auto container = map_container{ m_begin, m_end, transform };
        return stream<decltype(container)>{ std::move(container) };
    }

    constexpr auto limit(int limit) const
	{
		auto container = limit_container{ m_begin, m_end, limit };
		return stream<decltype(container)>{ std::move(container) };
	}
};


template <typename T, typename op_type>
constexpr auto seed_stream(T&& seed, op_type&& op)
{
    return stream { iterate_container{ std::forward<T>(seed), std::forward<op_type>(op) } };
}

export template <std::integral start_type, std::integral step_type>
constexpr auto int_stream(start_type&& start, step_type&& steps = 1)
{
    return seed_stream(std::forward<start_type>(start), [steps](auto x) { return x + steps; });
}