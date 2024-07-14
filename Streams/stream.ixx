export module stream;
import :distinct;
import :filter;
import :flat_map;
import :map;
import :limit;
import :generate;
import :peek;
import std;

export template <is_container Container>
class stream
{
    using value_type = std::remove_cvref_t<decltype(*std::declval<Container>().begin())>;
    using begin_type = decltype(std::declval<Container>().begin());
    using end_type = decltype(std::declval<Container>().end());

    std::shared_ptr<Container> mm_container;
    begin_type m_begin;
    end_type m_end;

    template <is_container OtherContainer> friend class stream;
public:
    constexpr stream(Container&& container) : mm_container{ std::make_shared<Container>(std::move(container)) }, m_begin{ mm_container->begin() }, m_end{ mm_container->end() } {}
    constexpr stream(Container& container) : mm_container{ nullptr }, m_begin{ container.begin() }, m_end{ container.end() } {}
    constexpr auto& begin() const { return m_begin; }
    constexpr auto& end() const { return m_end; }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto all_match(op_type predicate) const
	{
        for (const auto& entry: *this)
            if (!std::invoke(predicate, entry))
				return false;
        return true;
	}

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto any_match(op_type predicate) const
    {
        for (const auto& entry: *this)
			if (std::invoke(predicate, entry))
                return true;
		return false;
    }

    constexpr auto distinct() const
	{
		auto container = distinct_container{ m_begin, m_end };
		return stream<decltype(container)>(std::move(container));
	}

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto filter(op_type predicate) const
    {
        auto container = filter_container{ m_begin, m_end, predicate };
        return stream<decltype(container)>{ std::move(container) };
    }

    constexpr auto find_any() const
    {
        std::optional<std::remove_cvref_t<decltype(*this->begin())>> op{ std::nullopt };
        if (m_begin != m_end)
            op = *m_begin;
        return op;
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto find_first(op_type predicate) const
    {
        std::optional<std::remove_cvref_t<decltype(*this->begin())>> op{ std::nullopt };
        for (const auto& entry : *this)
            if (std::invoke(predicate, entry))
                return std::optional{ entry };
        return op;
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto flat_map(op_type transform) const
    {
        auto container = flat_map_container{ m_begin, m_end, transform };
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
    constexpr auto none_match(op_type predicate) const
    {
        return !this->any_match(predicate);
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename op_type> requires std::invocable<op_type, value_type>
    constexpr auto peek(op_type peeker) const
    {
        auto container = peek_container{ m_begin, m_end, peeker };
        return stream<decltype(container)>{ std::move(container) };
    }

    // Using long method name to keep intellisense happy. Otherwise, we can inline this concept in function declaration
    template <typename T, typename op_type> requires std::invocable<op_type, T, value_type>
    constexpr T reduce(const T& identity, op_type transform) const
    {
        T result = identity;
        for (const auto& entry : *this)
			result = std::invoke(transform, result, entry);
        return result;
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
        return container;
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