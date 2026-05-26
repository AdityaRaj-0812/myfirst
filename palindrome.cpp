// Simple palindrome checker
#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(const string &s) {
	string t;
	for (unsigned char ch : s) {
		if (isalnum(ch)) t.push_back(tolower(ch));
	}
	int i = 0, j = (int)t.size() - 1;
	while (i < j) {
		if (t[i++] != t[j--]) return false;
	}
	return true;
}

int main(int argc, char **argv) {
	string s;
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
			if (i > 1) s.push_back(' ');
			s += argv[i];
		}
	} else {
		if (!getline(cin, s)) return 0;
	}

	cout << (isPalindrome(s) ? "Palindrome" : "Not a palindrome") << '\n';
	return 0;
}
