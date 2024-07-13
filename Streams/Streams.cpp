import std;
import stream;

using namespace std;

bool isPrime(int i) 
{
	if (i <= 1)
		return false;

	if (i == 2)
		return true;

	if (i % 2 == 0)
		return false;

	for (int j = 3; j <= std::sqrt(i); j += 2)
	{
		if (i % j == 0)
			return false;
	}

	return true;
}

int main()
{
	for (auto i : int_stream(1, 1000).filter(&isPrime).limit(5))
	{
		cout << i << endl;
	}
}
