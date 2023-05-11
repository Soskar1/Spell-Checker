#include <iostream>
#include <string>
#include <vector>

#include "SpellChecker.h"

//Sentence generation: https://randomwordgenerator.com/sentence.php
//Error generation: http://www.hakank.org/reading_scrambled_words/g_spell.cgi

int main()
{
    using namespace Spelling;

    Dictionary dictionary({ 
        "he", "it", "we", "me", "or", "at", "do", "up", "as", "oh", "an",
        "she", "him", "but", "mud", "tax", "see", "few", "boy",
        "long", "read", "trap", "unit", "date", "just", "slot",
        "wrote", "model", "trade", "cheat", "brain", "handy",
        "letter", "didn\'t", "extend", "likely", "broken", "trance", "flavor"});

    std::string originalText = "she wrote him a long letter but he didn\'t read it";
    std::string mangledText = "hse wrte ihm a longf lette bdt hz dadn\'t retd ti";
    
    SpellChecker spellChecker(dictionary);

    std::vector<std::vector<std::string>> similarWordsVectors;
    similarWordsVectors = spellChecker.CheckTextSpelling(mangledText);

    std::cout << "original: " << originalText << std::endl;
    std::cout << "mangled: " << mangledText << std::endl;
    std::cout << "\nMistakes:\n";

    std::vector<std::string> words = SplitWords(mangledText);

    for (int i = 0; i < words.size(); ++i) {
        if (similarWordsVectors[i].size() == 0)
            continue;

        if (words[i] == similarWordsVectors[i][0])
            continue;

        std::cout << words[i] << ": ";

        for (auto word : similarWordsVectors[i]) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Not found in dictionary: \n";
    for (int i = 0; i < words.size(); ++i) {
        if (similarWordsVectors[i].size() == 0) {
            std::cout << words[i] << " ";
        }
    }

    return 0;
}