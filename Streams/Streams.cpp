import std;
import stream;

using namespace std;

struct P
{
	int x;
	bool valid;
};

int main()
{
	vector<P> v{ {1, true}, {2, false} };
	int sum = 0;
	for (auto i : stream(v).filter(&P::valid).map(&P::x).map([](auto i) { return i + 5; }))
	{
		sum += i;
	}
	return sum;
}
