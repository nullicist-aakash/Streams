export module stream:common;
import std;

struct sentinel {};

template <typename begin_type, typename end_type>
concept valid_iterator = requires(begin_type begin, end_type end) { { begin != end } -> std::same_as<bool>; *begin; ++begin; };

template <typename Container>
concept is_container = requires(Container c) { c.begin(); c.end(); { c.begin() != c.end() } -> std::same_as<bool>; *c.begin(); ++c.begin(); };

template <typename T>
concept set_type = requires(T t) { std::set<T>(); };