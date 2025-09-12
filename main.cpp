#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;



//Function to open and read files in general.
string readFile(const string &filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Error: No se pudo abrir " << filename << endl;
        return "";
    }
    stringstream text;
    text << in.rdbuf(); 
    return text.str();
}

// Function to read the malicious codes, and store their size, keeping in mind they are always palindromes.
void mcodeReader(vector<string> &mcodesContent, vector<int> &mcodesLength, vector<string> &mcodesName){
    vector<string> mcodes = {"mcode4.txt", "mcode2.txt", "mcode3.txt"};

    for (const string &filename : mcodes) {    
        string text = readFile(filename);
        if (!text.empty()){
            //Save the file content and size.
            mcodesContent.push_back(text);
            mcodesLength.push_back(text.size());
            mcodesName.push_back(filename); 
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
void manacher(const string &s, int &count, int &maxLength, int &start, int&end, vector<int>&P) {
    // Modify the string of the test file.
    string T = modifyString(s);
    int n = T.size();
    // Vector for P[i]
    P.assign(n, 0);

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


// Dynamic programming for part 3
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


//Aply the Manacher Algorithm & the dynamic programming of the LCS so we can complete part 1, 2  and 3 of the Integer Activity.
void apply (){

    struct Part1Info {
        string filename;
        string pattern;
        string mcodeName;
        bool found;
    };

    struct Part2Info {
        string filename;
        int start;
        int end;
        int maxLength;
    };
    
    //Variables
    vector<string> mcodesContent;
    vector<string> mcodesName;
    vector<Part2Info> Output2;
    vector<Part1Info>Output1;
    vector<int> mcodesLength;
    vector<int>P;


    //Read thte content of the malicious codes
    mcodeReader(mcodesContent, mcodesLength, mcodesName);

    
    //Files to read
    vector<string> files = {"transmission1.txt", "transmission2.txt"};    
    
    for (const string &filename : files) {    
        string text = readFile(filename);
        if (!text.empty()){

            //Variables for Manacher's Algorithm
            int count = 0;
            int maxLength = 0;
            int start = 0;
            int end;

            manacher(text, count, maxLength, start, end, P);


            //Part 1 of the activity
            for (size_t j = 0; j < mcodesContent.size(); ++j) {
                string pattern = mcodesContent[j];
                if (pattern.empty()) continue;
                int L = (int)pattern.size();
                
                bool found = false;
                for (size_t i = 0; i < P.size(); ++i) {
                    if (P[i] < L) continue;
                    int sp = (int)(((int)i - L) / 2);
                    if (sp < 0) continue;
                    if (sp + L > (int)text.size()) continue;
        
                    if (text.compare(sp, L, pattern) == 0) {
                        found = true;
                        break;
                    }
                }
                Output1.push_back({filename, pattern, mcodesName[j], found});
            }

            //Store part 2
            Output2.push_back({filename, start, end, maxLength});
        }
    }

    // Print part 1
    cout << "Part 1 :  \n" << endl;
    for (const auto &info : Output1) {
        cout << "(" << (info.found ? "true" : "false") << ") the file " << info.filename << " contains the code " << info.pattern << " in the file " << info.mcodeName << endl;
    }
    
    // Print part 2
    cout << "\nPart 2 : " << endl;
    for (const auto &info : Output2) {
        cout << "\nFile : " << info.filename<<endl;
        cout << "Start position: " << info.start << " End Position: " << info.end << endl;
        cout << "Length of the Longest Palindrome: " << info.maxLength << endl;
    }


    string s1 = readFile(files[0]);
    string s2 = readFile(files[1]);
    pair<int,int> transmissions = lcs(s1, s2);
    cout << "\nPart 3 : \n";
    cout << "Common substring of length : " << transmissions.first << " - " << "Start Position : " << transmissions.second << "  End position : " << transmissions.first + transmissions.second << endl;
}


int main(){
    apply();  
}