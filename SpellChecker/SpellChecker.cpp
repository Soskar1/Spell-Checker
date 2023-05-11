#include "SpellChecker.h"

#include <algorithm>

namespace Spelling {
	size_t Dictionary::Hash(const std::string& value) const
	{
		return value.length() % m_Capacity;
	}

	Dictionary::Dictionary(const size_t& initialCapacity)
	{
		m_Table = new std::list<std::string>[initialCapacity];
		m_Capacity = initialCapacity;
	}

	Dictionary::Dictionary(const std::vector<std::string>& words)
	{
		auto it = std::max_element(words.begin(), words.end(),
			[](const auto& a, const auto& b) {
				return a.size() < b.size();
			});

		m_Table = new std::list<std::string>[it->size()];
		m_Capacity = it->size();

		for (auto word : words) {
			Insert(word);
		}
	}

	Dictionary::~Dictionary()
	{
		delete[] m_Table;
		m_Table = nullptr;
	}

	void Dictionary::Create(const std::vector<std::string>& words)
	{
		for (auto word : words) {
			Insert(word);
		}
	}

	void Dictionary::Insert(const std::string& value)
	{
		size_t hashIndex = Hash(value);
		m_Table[hashIndex].push_back(value);
	}

	void Dictionary::Remove(const std::string& value)
	{
		size_t hashIndex = Hash(value);

		for (auto it = m_Table[hashIndex].begin(); it != m_Table[hashIndex].end(); ++it) {
			if (*it == value) {
				m_Table[hashIndex].erase(it);
				break;
			}
		}
	}

	bool Dictionary::Contains(const std::string& value)
	{
		size_t hashIndex = Hash(value);
		auto list = m_Table[hashIndex];

		for (auto it = list.begin(); it != list.end(); ++it) {
			if (*it == value) {
				return true;
			}
		}

		return false;
	}

	std::list<std::string> Dictionary::GetWords(const size_t& length) const
	{
		return m_Table[length % m_Capacity];
	}

	size_t SpellChecker::LevensteinDamerauDistance(const std::string& firstWord, const std::string& secondWord) const
	{
		std::vector<std::vector<size_t>> buffer(firstWord.size() + 1);

		for (int i = 0; i < firstWord.size() + 1; ++i) {
			buffer[i] = std::vector<size_t>(secondWord.size() + 1);

			for (int j = 0; j < secondWord.size() + 1; ++j) {
				if (i == 0 && j == 0) {
					buffer[0][0] = 0;
					continue;
				}

				if (i == 0) {
					buffer[0][j] = buffer[0][j - 1] + 1;
				}
				else if (j == 0) {
					buffer[i][0] = buffer[i - 1][0] + 1;
				}
				else if (firstWord[i - 1] == secondWord[j - 1]) {
					buffer[i][j] = buffer[i - 1][j - 1];
				}
				else {
					buffer[i][j] = std::min({buffer[i - 1][j - 1], buffer[i - 1][j], buffer[i][j-1]}) + 1;
				}

				if (i <= 1 || j <= 1)
					continue;

				if (firstWord[i - 1] == secondWord[j - 2] && firstWord[i - 2] == secondWord[j - 1]) {
					buffer[i][j] = std::min(buffer[i - 2][j - 2], buffer[i][j]) + 1;
				}
			}
		}

		return buffer[firstWord.size()][secondWord.size()];
	}

	SpellChecker::SpellChecker(const Dictionary& dictionary)
	{
		m_Dictionary = const_cast<Dictionary*>(&dictionary);
	}

	std::vector<std::string> SpellChecker::CheckWordSpelling(const std::string& word) const
	{
		if (m_Dictionary->Contains(word)) {
			return std::vector<std::string>({ word });
		}

		std::vector<std::string> similarWords;
		size_t minDistance = INT_MAX;
		size_t distance = 0;

		std::vector<std::string> dictionaryWords;
		auto list = m_Dictionary->GetWords(word.length());
		dictionaryWords.insert(dictionaryWords.begin(), list.begin(), list.end());

		if (word.length() > 1) {
			list = m_Dictionary->GetWords(word.length() - 1);
			dictionaryWords.insert(dictionaryWords.begin(), list.begin(), list.end());
		}

		if (word.length() < m_Dictionary->m_Capacity) {
			list = m_Dictionary->GetWords(word.length() + 1);
			dictionaryWords.insert(dictionaryWords.begin(), list.begin(), list.end());
		}

		for (auto dictionaryWord : dictionaryWords) {
			distance = LevensteinDamerauDistance(word, dictionaryWord);

			if (minDistance > distance) {
				minDistance = distance;
				similarWords.clear();
			}

			if (minDistance == distance) {
				similarWords.push_back(dictionaryWord);
			}
		}

		return similarWords;
	}

	std::vector<std::vector<std::string>> SpellChecker::CheckTextSpelling(const std::string& text) const
	{
		std::vector<std::vector<std::string>> similarWords;
		std::vector<std::string> words = SplitWords(text);

		for (auto word : words) {
			similarWords.push_back(CheckWordSpelling(word));
		}

		return similarWords;
	}

	std::vector<std::string> SplitWords(const std::string& text)
	{
		std::vector<std::string> words;

		int offset = 0;
		int spaceIndex = 0;
		while (spaceIndex != std::string::npos) {
			spaceIndex = text.find(' ', offset);
			std::string word = text.substr(offset, spaceIndex - offset);
			words.push_back(word);

			offset += spaceIndex - offset + 1;
		}

		return words;
	}
}