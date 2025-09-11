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


#include <bits/stdc++.h>

pair<int,int> lcs(const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();

    int lengthCommonSubstring = 0;
    int startPosS1 = -1;
    int startPosS2 = -1;
    int startPos = -1;

    vector<vector<int>> dp(2, vector<int>(n+1, 0)); // matriz 2 x (n+1)

    int currRow = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[currRow][j] = dp[1-currRow][j-1] + 1;

                if (dp[currRow][j] > lengthCommonSubstring) {
                    lengthCommonSubstring = dp[currRow][j];
                    startPosS1 = i - lengthCommonSubstring + 1;
                    startPosS2 = j - lengthCommonSubstring + 1;
                }
            } else {
                dp[currRow][j] = 0;
            }
        }
        currRow = 1 - currRow; // alternar filas
    }

    startPos = min(startPosS1, startPosS2);

    return {lengthCommonSubstring, startPos};
}

int obtainLCS(string &s1, string &s2) {
    pair<int,int> lcsTransmissions = lcs(s1, s2);
    cout << "Longitud: " << lcsTransmissions.first << endl;
    cout << "Inicio: " << lcsTransmissions.second << endl;
    cout << "Final: " << lcsTransmissions.first + lcsTransmissions.second - 1 << endl;

    return 0;
}

int main(){
    vector<string> files = {"mcode1.txt", "mcode2.txt", "mcode3.txt", "mcode4.txt", "mcode5.txt"};
    
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
        cout << "Contenido de " << fname << ":\n";
        cout << s << "\n";
    }
}