import std;
import stream;

using namespace std;

int main()
{
	vector s{ 1,2,4,4,3,2,6 };
	auto str = stream(s).distinct().limit(2);
	for (const auto x : str)
		cout << x << " ";
}
