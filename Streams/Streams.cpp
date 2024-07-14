import std;
import stream;

using namespace std;

int main()
{
	for (const auto x : int_stream(1, 1).filter([](int x) {return x > 6;}).limit(5ul))
		cout << x << endl;
}