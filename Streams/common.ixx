export module stream:common;
import std;

struct sentinel {};

template <typename begin_type, typename end_type>
concept valid_iterator = requires(begin_type begin, end_type end) { { begin != end } -> std::same_as<bool>; *begin; ++begin; };

template <typename transform_type, typename return_type, typename... Args>
concept invocable = std::invocable<transform_type, Args...> && std::same_as<std::invoke_result_t<transform_type, Args...>, return_type>;
