import std;
import stream;

using namespace std;

int main()
{
	vector s{ 1,2,4,4,3,2,6 };
	auto opt = stream(s).sorted().find_any([](auto x) {return x > 2; });
	if (opt)
		cout << *opt << endl;
	stream(s).for_each([](auto x) {cout << x << " "; });
}