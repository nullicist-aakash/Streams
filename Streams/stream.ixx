export module stream;
import :filter;
import :map;
import :limit;
import :generate;
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

    friend constexpr auto generate_stream(std::invocable auto op);

    template <typename T>
    friend constexpr auto seed_stream(const T& seed, std::invocable<T> auto op);

    constexpr stream(Container&& container) : mm_container{ std::move(container) }, m_begin{ mm_container->begin() }, m_end{ mm_container->end() } {}
public:
    constexpr stream(Container& container) : mm_container{ std::nullopt }, m_begin{ container.begin() }, m_end{ container.end() } {}
    constexpr auto begin() const { return m_begin; }
    constexpr auto end() const { return m_end; }

    constexpr auto filter(std::invocable<value_type> auto predicate) const
    {
        auto container = filter_container{ m_begin, m_end, predicate };
        return stream<decltype(container)>{ std::move(container) };
    }

    constexpr auto map(std::invocable<value_type> auto transform) const
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

export constexpr auto generate_stream(std::invocable auto op)
{
    return stream{ generate_container{ op } };
}

export template <typename T>
constexpr auto seed_stream(const T& seed, std::invocable<T> auto op)
{
    struct _local
    {
        mutable std::shared_ptr<T> _seed;
        decltype(op) _op;
        mutable bool init = false;

        constexpr T operator()() const {
            if (!init) {
                init = true;
                return *_seed;
            }

            return *(_seed = std::make_shared<T>(std::invoke(_op, *_seed)));
        }
    };

    return stream{ generate_container{ _local{ std::make_shared<T>(seed), op } } };
}

export constexpr auto int_stream(const std::integral auto& start, const std::integral auto& steps)
{
    return seed_stream(start, [steps](const auto& x) { return x + steps; });
}