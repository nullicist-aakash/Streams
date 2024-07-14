import std;
import stream;

using namespace std;

int main()
{
	cout << stream(std::vector{ 1, 2, 3, 4, 5 })
		.peek([](auto x) {std::print("Taking Integer: {}\n", x); })
		.filter([](auto x) { return x % 2 == 1; })
		.peek([](auto x) {std::print("Filtered Integer: {}\n", x); })
		.map([](auto x) { return x * x; })
		.peek([](auto x) {std::print("Mapped Integer: {}\n", x); })
		.reduce(0, [](auto x, auto y) { return x + y; });
}
