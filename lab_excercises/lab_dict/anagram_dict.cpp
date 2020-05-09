/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <iostream>
#include <string>
#include "anagram_dict.h"
#include <algorithm>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    ifstream words(filename);
    string word;
    if (words.is_open()) {
        while (getline(words, word)) {
            add_word_to_dict(word);
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    for (const string& word : words) {
        add_word_to_dict(word);
    }
}

string hash(const string& word)
{
  int primes[] = {2, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103};
  int key = 1;
  for (int i = 0; i < (int) word.size(); i++) {
    key *= primes[((int) word[i]) - 96];
  }
  return std::to_string(key);
}

/**
 * @param word The word to add to the dictionary
 * Adds the word as a (key, value) pair to the dictionary.
 * NOTE: word is the value. You will need to decide an appropriate key
 * for the word.
 */
void AnagramDict::add_word_to_dict(const std::string& word)
{
    /* Your code goes here! */
    string key = hash(word);
    auto lookup = dict.find(key);
    if (lookup != dict.end()) {
      lookup->second.push_back(word);
    } else {
      vector<string> anagrams;
      anagrams.push_back(word);
      dict.insert(std::pair<string, vector<string>>(key, anagrams));
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    /* Your code goes here! */
    string key = hash(word);
    auto lookup = dict.find(key);
    if (lookup != dict.end()) {
      vector<string> anagrams = lookup->second;
      return anagrams;
    } else {
      return vector<string>();
    }
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    /* Your code goes here! */
    vector<vector<string>> ret;
    std::map<std::string, std::vector<std::string>>::const_iterator it;
    for (it = dict.begin(); it != dict.end(); ++it) {
      ret.push_back(it->second);
    }
    return ret;
}
