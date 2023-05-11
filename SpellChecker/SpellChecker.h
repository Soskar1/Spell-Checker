#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <string>
#include <vector>
#include <list>

namespace Spelling {
	class Dictionary {
	private:
		std::list<std::string> *m_Table;
		size_t m_Capacity;

		size_t Hash(const std::string& value) const;
	public:
		Dictionary(const size_t& initialCapacity);
		Dictionary(const std::vector<std::string>& words);
		~Dictionary();

		void Create(const std::vector<std::string>& words);

		void Insert(const std::string& value);
		void Remove(const std::string& value);
		bool Contains(const std::string& value);

		std::list<std::string> GetWords(const size_t& length) const;

		friend class SpellChecker;
	};

	class SpellChecker {
	private:
		Dictionary* m_Dictionary;

		size_t LevensteinDamerauDistance(const std::string& firstWord, const std::string& secondWord) const;
	public:
		SpellChecker(const Dictionary& dictionary);

		std::vector<std::string> CheckWordSpelling(const std::string& word) const;
		std::vector<std::vector<std::string>> CheckTextSpelling(const std::string& text) const;
	};

	std::vector<std::string> SplitWords(const std::string& text);
}
#endif