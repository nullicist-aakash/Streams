export module stream;
import :distinct;
import :filter;
import :map;
import :limit;
import :generate;
import std;

export template <typename Container> requires requires(Container cont) { cont.begin(); cont.end(); }
class stream
{
    using value_type = std::remove_cvref_t<decltype(*std::declval<Container>().begin())>;
    using begin_type = decltype(std::declval<Container>().begin());
    using end_type = decltype(std::declval<Container>().end());

    std::optional<Container> mm_container;
    begin_type m_begin;
    end_type m_end; 

    template <typename OtherContainer> requires requires(OtherContainer cont) { cont.begin(); cont.end(); }
    friend class stream;
public:
    constexpr stream(Container&& container) : mm_container{ std::move(container) }, m_begin{ mm_container->begin() }, m_end{ mm_container->end() } {}
    constexpr stream(Container& container) : mm_container{ std::nullopt }, m_begin{ container.begin() }, m_end{ container.end() } {}
    constexpr auto begin() const { return m_begin; }
    constexpr auto end() const { return m_end; }

    constexpr auto distinct() const
	{
		auto container = distinct_container{ m_begin, m_end };
		return stream<decltype(container)>{ std::move(container) };
	}

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto filter(op_type predicate) const
    {
        auto container = filter_container{ m_begin, m_end, predicate };
        return stream<decltype(container)>{ std::move(container) };
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr void for_each(op_type op) const
    {
        for (const auto& value : *this)
            std::invoke(op, value);
    }

    constexpr auto limit(int limit) const
    {
        auto container = limit_container{ m_begin, m_end, limit };
        return stream<decltype(container)>{ std::move(container) };
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto map(op_type transform) const
    {
        auto container = map_container{ m_begin, m_end, transform };
        return stream<decltype(container)>{ std::move(container) };
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto peek(op_type op) const
    {
        if (this->begin() != this->end())
            std::invoke(op, *this->begin());
        return *this;
    }

    constexpr auto skip(int skip) const
    {
        auto that = *this;
        while (that.m_begin != that.m_end && skip-- > 0)
			++that.m_begin;
        return that;
    }

    constexpr auto sorted() const
    {
        std::vector<value_type> container{ m_begin, m_end };
        std::sort(container.begin(), container.end());
        return stream<decltype(container)>{ std::move(container) };
    }
};

export constexpr auto generate_stream(std::invocable auto op)
{
    return stream{ generate_container{ op } };
}

export template <typename T>
constexpr auto iterate_stream(const T& seed, std::invocable<T> auto op)
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
    return iterate_stream(start, [steps](const auto& x) { return x + steps; });
}