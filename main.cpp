#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;


// Add the character "#" between characters of the string so the algorithm works for both even and odd-length palindromes. 
string modifyString(const string &s) {
    
    if (s.empty()) {
        return "^$";
    }
    string t = "^";

    for (char c : s) {
        t += "#";
        t += c;
    }
    t += "#$";
    return t;
}

// Function for Manacher Algorithm.
void manacher(const string &s, int &count, int &maxLength, int &start) {
    // Modify the string of the test file.
    string T = modifyString(s);
    int n = T.size();
    // Vector for P[i]
    vector<int> P(n, 0);

    //Center and Right Boundary.
    int C = 0, R = 0;

    // For every character in the modified string.
    for (int i = 1; i < n - 1; i++) {
        int mirror = 2 * C - i;
        // If it's within the current palindrome
        if (i < R)
            P[i] = min(R - i, P[mirror]);

        // If it's beyond or touches R limit
        while (T[i + (1 + P[i])] == T[i - (1 + P[i])])
            //Expand at i
            P[i]++;

        //Update center and R if it palindrome was expanded beyond R
        if (i + P[i] > R) {
            C = i;
            R = i + P[i];
        }
    }
    
    maxLength = 0;
    int center = 0;

    // Count the palindromes
    for (int i = 1; i < n - 1; i++) {
        count += (P[i] + 1) / 2 + 1;

        // Update the longest palindrome info
        if (P[i] > maxLength) {
            maxLength = P[i];
            center = i;
        }
    }
    // Start position of the longest palindrome
    start = (center - maxLength) / 2;
};

void hash(){
    
}


int main(){
    vector<string> files = {"mcode1.txt", "mcode2.txt", "mcode3.txt", "mcode4.txt", "mcode5.txt"};

    for (string &fname : files) {
        ifstream in(fname);

        string s;
        in >> s;

        cout << s << endl;
    }
}