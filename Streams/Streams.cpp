import std;
import stream;

using namespace std;

int main()
{
	int sum = 0;
	for (auto x : int_stream(1, 3).limit(5))
		sum += x;
	return sum;
}