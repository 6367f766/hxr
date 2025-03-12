#ifndef COLOURPRINT_H
#define COLOURPRINT_H
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "common.h"

static constexpr uint32_t MaxWordSize = 15;

/** @enum WordFixNotation
 *
 *  @brief The smallest unit is a word. A word comes with a "prefix" and a
 *  "postfix" and a byte (flag) that signals which to apply:
 *  - 0000 (not initialised)
 *  - 0010 (word)
 *  - 0110 (prefix & word)
 *  - 0011 (word & postfix)
 *  - 0111 (prefix, word, postfix)
 */
enum class WordFixNotation : uint8_t {
    Uninitialised = 0,
    OnlyWord = 2,
    PostfixWord = 3,
    PrefixWord = 6,
    All = 7
};

/**
 * @class Word
 * @brief A word is, well, a std::string with no spaces (hopefully), but relaxed
 * to be whatever the user considers to be a "Word". We don't all speak English.
 * A great intellect once said:
 *
 * "You speak English because it's the only language you know. I speak English
 * because it's the only language you know. We are not the same."
 *
 */
struct Word {
    SizedText<MaxWordSize> pre;
    SizedText<MaxWordSize> word;
    SizedText<MaxWordSize> post;
    WordFixNotation metadata{WordFixNotation::OnlyWord};

    explicit Word(std::string word)
        : word{SizedText<MaxWordSize>::fromString(word)} {};

    /**
     * @brief add postfix to current Word. Will change metadata accordingly
     *
     * For example if current word is "hello", a postfix could be:
     * " world" and the result is "hello world"
     *
     * @param postfix
     * @return this
     */
    Word& withPostfix(std::optional<std::string> postfix);

    /**
     * @brief add prefix to current Word, changing metadata accordingly
     *
     * For example if current word is "hello", a prefix can be something like:
     * "--->" and the result is "--->hello"
     *
     * @param prefix
     * @return this
     */
    Word& withPrefix(std::optional<std::string> prefix);
    std::string get();

    bool operator==(const Word& other) const;
};

/**
 * @class SentenceGenerator
 * @brief A sentence is, well, a sequence of Words
 *
 */
class SentenceGenerator {
    std::vector<Word> wordSequence_{};
    std::ostringstream oss_{};

    Word wordPattern;

   public:
    /**
     * @brief Initialise with a `wordPattern` - a data member that describes
     * what do pre-sentence pattern, the intra-word pattern and post sentence
     * pattern.
     *
     * That is, for a normal sentence Word{" "}.withPostfix("\n")
     *
     * This would add spaces between words in `wordSequence_` and a new line
     * character at the end. NOTE: there is no pre-fix in the example above.
     *
     * @param wordPattern
     */
    SentenceGenerator(Word wordPattern) : wordPattern{wordPattern} {}

    void add(Word&& word);

    std::string get();
};

#endif
