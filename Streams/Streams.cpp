import std;
import stream;

using namespace std;

int main()
{
	vector s{ 1,2,4,4,3,2,6 };
	stream(s).sorted().for_each([](auto i) { cout << i << " "; });
}