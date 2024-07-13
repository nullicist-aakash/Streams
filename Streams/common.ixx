export module stream:common;
import std;

struct sentinel {};

template <typename begin_type, typename end_type>
concept valid_iterator = requires(begin_type begin, end_type end) { { begin != end } -> std::same_as<bool>; *begin; ++begin; };
