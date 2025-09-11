#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class manacher {
public:
    // p[i] stores the radius of the palindrome 
    // centered at position i in ms
    vector<int> p;  
    
    // transformed string with sentinels
    // and separators
    string ms;      

    manacher(string &s) {
        
        // left sentinel to avoid bounds check
        ms = "@";  
        for (char c : s) {
            
            // insert '#' between every character
            ms += "#" + string(1, c);  
        }
        
        // right sentinel
        ms += "#$";  
        runManacher();
    }

    void runManacher() {
        int n = ms.size();
        p.assign(n, 0);
        int l = 0, r = 0;

        for (int i = 1; i < n - 1; ++i) {
            // initialize p[i] based on its mirror 
            // and current [l, r] range
            if(r + l - i >= 0 && r + l - i < n)
                p[i] = max(0, min(r - i, p[r + l - i]));

            // try expanding around center i
            while (ms[i + 1 + p[i]] == ms[i - 1 - p[i]]) {
                ++p[i];
            }

            // update [l, r] if the new palindrome goes 
            // beyond current right boundary
            if (i + p[i] > r) {
                l = i - p[i];
                r = i + p[i];
            }
        }
    }

    // return the radius of the longest palindrome
    // centered at original index 'cen'
    int getLongest(int cen, int odd) {
        int pos = 2 * cen + 2 + !odd;
        return p[pos];
    }

    // checks whether the substring 
    // s[l..r] is a palindrome
    bool check(int l, int r) {
        int res = getLongest((r + l) / 2, (r - l + 1) % 2);
        return (r - l + 1) <= res;
    }
};

// finds and returns the longest palindromic substring in s
string getLongestPal(string &s) {
    int n = s.size(), maxLen = 1, start = 0;
    manacher M(s);

    for (int i = 0; i < n; ++i) {
        int oddLen = M.getLongest(i, 1);
        if (oddLen > maxLen) {
            // update start for odd-length palindrome
            start = i - (oddLen - 1) / 2;  
        }

        int evenLen = M.getLongest(i, 0);
        if (evenLen > maxLen) {
            // update start for even-length palindrome
            start = i - (evenLen - 1) / 2;  
        }

        maxLen = max(maxLen, max(oddLen, evenLen));
    }

    return s.substr(start, maxLen);
}

int main() {
    vector<string> files = {"transmission1.txt", "transmission2.txt"};
    
    for (string &fname : files) {    
        ifstream in(fname, ios::binary);
        if (!in) {
            cout << "Error: No se pudo abrir " << fname << endl;
            continue;
        }
        string s;
        char c;
        while (in.get(c)) {
            s.push_back(c);
        }
        cout << getLongestPal(s) << endl;
    }
    

    return 0;
}