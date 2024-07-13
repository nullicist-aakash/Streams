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
	for (auto i : int_stream(1, 15).limit(8))
	{
		sum += i;
	}
	return sum;
}
