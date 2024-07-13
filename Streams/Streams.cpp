import std;
import stream;

using namespace std;

int main()
{
	int sum = 0;
	for (auto i : int_stream(0, 10).filter([](auto i) { return i % 2 == 0; }).map([](auto i) { return i * 2; }))
	{
		sum += i;
	}
	return sum;
}
