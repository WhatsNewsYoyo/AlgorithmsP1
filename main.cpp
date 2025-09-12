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
        cout << "Can't open file : " << filename << endl;
        return "";
    }
    stringstream text;
    text << in.rdbuf(); 
    return text.str();
}

// Function to read the malicious codes, and store their size, keeping in mind they are always palindromes.
void mcodeReader(vector<string> &mcodesContent, vector<int> &mcodesLength, vector<string> &mcodesName){
    vector<string> mcodes = {"mcode1.txt", "mcode2.txt", "mcode3.txt", "mcode5.txt"};

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

//Functions for part 1.

//Function to look up for proper prefixes wich are also a suffix
vector<int> ArrayLPS (string &pat){
    //Size of the pattern to look - basically length of the mcode.
    int n = pat.size();
    vector<int>LPS(n, 0);

    //Iterators to check 
    int len = 0;
    for(int i=1; i<n ; i++){
        if(pat[i] == pat[len]){
            len++;
            LPS[i] = len;
        } else {
            if(len>0){
                len = LPS[len - 1];
            } else {
                LPS[i] = 0;
            }
        }
    }
    return LPS;
}

vector<int> search(string &pat, string &text) {
    vector<int> lps = ArrayLPS(pat);
    vector<int> res;
    int j = 0;

    for (int i = 0; i < text.size(); i++) {
        while (j > 0 && text[i] != pat[j])
            j = lps[j - 1];
        if (text[i] == pat[j])
            j++;
        if (j == pat.size()) {
            res.push_back(i - j + 1);
            j = lps[j - 1];
        }
    }
    return res;
}

void KMP(){
    //Files to read (where it will look for the Patterns of the Mcodes)
    vector<string> files = {"transmission1.txt", "transmission2.txt"};
    vector<string> mcodes = {"mcode1.txt", "mcode3.txt", "mcode2.txt", "mcode4.txt", "mcode5.txt"}; 
    
    cout << "\nPart 3 : " << endl;
    //Look at both files to check if any of them has a mcode.
    for (const string &TransmissionFilename : files) {    
        string TransmissionText = readFile(TransmissionFilename);
        if (!TransmissionText.empty()){
            for (const string &mcodeFileName : mcodes) {    
                string mcodeText = readFile(mcodeFileName);
                if (!mcodeText.empty()){
                  
                    vector<int>res = search(mcodeText, TransmissionText);
                    if (!res.empty()){
                        cout << "(true) the file: " << TransmissionFilename << "contains the code: " << mcodeText << "contained in the file: " << mcodeFileName << "\n";
                    } else {
                        cout << "(false) the file: " << TransmissionFilename << "doesn't contain the code: " << mcodeText << "contained in the file: " << mcodeFileName << "\n"; 
                    }
                    
                } else { cout << "Empty mcode : "<< mcodeFileName << endl;}
            }

        } else { cout << "Empty transmission file : " << TransmissionFilename << endl; }
    }
}


// Functions utilized for Part 2 : First to modify the string and later to use manacher
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
        // If it's within the current palindrome. If it's beyond or touches R it will ignore this
        if (i < R)
            P[i] = min(R - i, P[mirror]);

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

    vector<vector<int>> dp(2, vector<int>(n+1, 0)); // matrix 2 x (n+1)

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
        currRow = 1 - currRow;
    }
    startPos = min(startPosS1, startPosS2);

    return {lengthCommonSubstring, startPos};
}

//Aply the KMP, the Manacher Algorithm & the dynamic programming of the LCS so we can complete part 1, 2 and 3 of the Integer Activity.
void apply (){
    
    //Apply KMP
    KMP();

    //Files to read
    vector<string> files = {"transmission1.txt", "transmission2.txt"}; 
    cout << "\nPart 2 : " << endl;   
    for (const string &filename : files) {    
        string text = readFile(filename);
        if (!text.empty()){

            //Variables for Manacher's Algorithm
            int count = 0;
            int maxLength = 0;
            int start = 0;
            int end;

            manacher(text, count, maxLength, start, end);
            // Print part 2
            cout << "\nFile : " << filename<<endl;
            cout << "Start position: " << start << " End Position: " << end << endl;
            cout << "Length of the Longest Palindrome: " << maxLength << endl;
        }
    }

    string s1 = readFile(files[0]);
    string s2 = readFile(files[1]);
    pair<int,int> transmissions = lcs(s1, s2);
    cout << "\nPart 3 : \n";
    cout << "Common substring of length : " << transmissions.first << " - " << "Start Position : " << transmissions.second << "  End position : " << transmissions.first + transmissions.second - 1<< endl;
}


int main(){
    apply();  
}