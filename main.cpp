#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;


string readFile(const string &filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Error: No se pudo abrir " << filename << endl;
        return "";
    }
    string text;
    char c;
    while (in.get(c)) {
            text.push_back(c);
    }
    return text;
}

// Function to read the malicious codes, and store their size, keeping in mind they are always palindromes.
void mcodeReader(vector<string> mcodesContent, vector<int> mcodesLength){
    vector<string> mcodes = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    for (const string &filename : mcodes) {    
        string text = readFile(filename);
        if (!text.empty()){
            //Save the file content and size.
            mcodesContent.push_back(text);
            mcodesLength.push_back(text.size());
        }
    }
}

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
void manacher(const string &s, int &count, int &maxLength, int &start, int&end) {
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
    start = (center - maxLength) / 2 + 1;
    end = (center + maxLength) / 2;
};

//Aply the Manacher Algorithm so we can complete part 1 and 2 of the Integer Activity.
void apply (){
    vector<string> files = {"transmission1.txt", "transmission2.txt"};
    
    for (const string &filename : files) {    
        string text = readFile(filename);
        if (!text.empty()){
            
            int count = 0;
            int maxLength = 0;
            int start = 0;
            int end;

            manacher(text, count, maxLength, start, end);
            cout << "File: " << filename << ":\n";
            cout << "Start position: " << start << " End Position: " << end <<endl;
            cout << "maxLength" << maxLength << endl;
        }
    }
}


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

string readFileLCS(const string &filename) {
    ifstream in(filename);

    stringstream buffer;

    buffer << in.rdbuf(); // leer todo de una vez

    return buffer.str();
}

int main(){
    vector<string> files = {"transmission1.txt", "transmission2.txt"};
    
    for (string &fname : files) {    
        ifstream in(fname);
        if (!in) {
            cout << "Error: No se pudo abrir " << fname << endl;
            continue;
        }
        string s;
        char c;
        while (in.get(c)) {
            s.push_back(c);
        }        
        int count = 0;
        int maxLength = 0;
        int start = 0;
        int end;

        manacher(s, count, maxLength, start, end);
        cout << "File: " << fname << ":\n";
        cout << "Start position: " << start << " End Position: " << end <<endl;

    }

    string s1 = readFileLCS(files[0]);
    string s2 = readFileLCS(files[1]);

    pair<int,int> transmissions = lcs(s1, s2);

    cout << "A substring of length " << transmissions.first
        << " was found at start position " << transmissions.second << " and end position "
        << transmissions.first + transmissions.second << endl;
}