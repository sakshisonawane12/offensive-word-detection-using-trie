#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream> // Include for file handling
using namespace std;

// Trie Node class
class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
    }
};

// Trie class for building and searching the offensive words
class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Function to insert a word into the trie
    void insert(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }
            node = node->children[ch];
        }
        node->isEndOfWord = true;
    }

    // Function to search a word in the trie
    bool search(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            if (node->children.find(ch) == node->children.end()) {
                return false;
            }
            node = node->children[ch];
        }
        return node->isEndOfWord;
    }

    // Function to check if any offensive word exists in the comment
    bool containsOffensiveWord(string comment) {
        string word = "";
        for (char ch : comment) {
            if (isalpha(ch)) {
                word += tolower(ch); // Convert to lowercase for case insensitivity
            } else {
                if (!word.empty() && search(word)) {
                    return true; // Found an offensive word
                }
                word = ""; // Reset word for the next word in the comment
            }
        }
        // Check the last word in the comment
        if (!word.empty() && search(word)) {
            return true;
        }
        return false;
    }
};

// Function to moderate the user's comment
void moderateComment(Trie& trie, string comment) {
    if (trie.containsOffensiveWord(comment)) {
        cout << "This comment contains offensive words." << endl;
    } else {
        cout << "Comment posted: " << comment << endl;
    }
}

// Function to read offensive words from a text file
vector<string> readOffensiveWordsFromFile(const string& filename) {
    vector<string> offensiveWords;
    ifstream file(filename); // Open the file

    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return offensiveWords; // Return an empty vector on error
    }

    string word;
    while (file >> word) { // Read each word from the file
        offensiveWords.push_back(word);
    }

    file.close(); // Close the file
    return offensiveWords;
}

int main() {
    // Read offensive words from the text file
    vector<string> offensiveWords = readOffensiveWordsFromFile("offensive_words.txt");

    // Initialize the Trie and insert offensive words
    Trie trie;
    for (const string& word : offensiveWords) {
        trie.insert(word);
    }

    // Example user input comment
    string userComment;
    cout << "Enter your comment: ";
    getline(cin, userComment);

    // Moderate the comment
    moderateComment(trie, userComment);

    return 0;
}
