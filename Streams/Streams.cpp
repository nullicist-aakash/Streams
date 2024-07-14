import std;
import stream;

using namespace std;

int main()
{
	vector s{ 1,2,4,4,3,2,6 };
	auto opt = stream(s).reduce(0, std::plus{});
	cout << opt << endl;
}