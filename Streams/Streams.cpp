import std;
import stream;

using namespace std;

int main()
{
	vector<int> v = {1, 2, 3, 4, 5};
	stream(v).for_each([](auto& x) { x *= 2; });
	stream(v).for_each([&](auto x) { cout << x << " "; });
}
