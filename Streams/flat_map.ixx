export module stream:flat_map;
import :common;
import std;

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>
    && std::invocable<transform_type, decltype(*std::declval<begin_type>())>
class flat_map_iterator
{
    using mapped_type = std::invoke_result_t<transform_type, decltype(*std::declval<begin_type>())>;
    using mapped_itr_type = decltype(std::declval<mapped_type>().begin());

    begin_type m_iterator;
    end_type m_end;
    transform_type transform;
    std::shared_ptr<mapped_type> container{ nullptr };
    std::shared_ptr<mapped_itr_type> container_iterator{ nullptr };
    
    static_assert(is_container<mapped_type>, "Result type is not a iterable thing!");
public:
    constexpr flat_map_iterator(begin_type iterator, const end_type& end, const transform_type& transform) : m_iterator{ iterator }, m_end{ end }, transform{ transform } 
    {
        while (m_iterator != m_end)
        {
            container = std::make_shared<mapped_type>(std::invoke(transform, *m_iterator));
            if (container->begin() == container->end())
			{
                container = nullptr;
				++m_iterator;
				continue;
			}
            container_iterator = std::make_shared<mapped_itr_type>(container->begin());
            break;
        }
    }

    constexpr auto operator*() const { return *(*container_iterator); }

    constexpr auto& operator++()
    {
        if (*container_iterator != container->end())
        {
            ++(*container_iterator);
            if (*container_iterator != container->end())
				return *this;
        }

        while (++m_iterator != m_end)
		{
			container = std::make_shared<mapped_type>(std::invoke(transform, *m_iterator));
			if (container->begin() == container->end())
				continue;
            container_iterator = std::make_shared<mapped_itr_type>(container->begin());
			break;
		}
        return *this;
    }
    constexpr auto operator++(int) { auto tmp = *this; ++(*this); return tmp; }

    static friend constexpr auto operator== (const flat_map_iterator& a, const end_type& end) { return a.m_iterator == end; };
    static friend constexpr auto operator!= (const flat_map_iterator& a, const end_type& end) { return a.m_iterator != end; };
};

template <typename begin_type, typename end_type, typename transform_type>
    requires valid_iterator<begin_type, end_type>
    && std::invocable<transform_type, decltype(*std::declval<begin_type>())>
struct flat_map_container
{
    using mapped_type = std::invoke_result_t<transform_type, decltype(*std::declval<begin_type>())>;

    const begin_type m_begin;
    const end_type m_end;
    const transform_type transform;

    static_assert(is_container<mapped_type>, "Result type is not a iterable thing!");

    constexpr auto begin() const { return flat_map_iterator(m_begin, m_end, transform); }
    constexpr auto end() const { return m_end; }
};
