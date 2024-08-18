#include <iostream>
#include <vector>

using namespace std;

int lengthOfLastWord(string s) {
    int length = 0;
    bool hitWord = false;
    for (int index = s.length() - 1; index >= 0; --index)
    {
    	cout << s[index] << endl;
        if (s[index] != ' ')
        {
            hitWord = true;
            ++length;                
        } else if (hitWord == true)
        {
            break;
        }
    }
    return length;
}	

int main()
{
	string s = "a";

	// lengthOfLastWord(s);
	cout << lengthOfLastWord(s) << endl;

}


