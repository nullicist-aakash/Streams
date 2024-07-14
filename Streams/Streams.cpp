import std;
import stream;

using namespace std;

int main()
{
	string s = "5 apples and 25 oranges";
	stream(s).for_each([](char c) { cout << c; });
}