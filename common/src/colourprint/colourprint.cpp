#include "colourprint.h"

#include <iostream>

Word& Word::withPostfix(std::optional<std::string> postfix) {
    if (postfix) {
        post = SizedText<MaxWordSize>::fromString(postfix.value());
        uint8_t updated = static_cast<uint8_t>(this->metadata);
        updated |= 0b00000001;
        this->metadata = static_cast<WordFixNotation>(updated);
    }
    return *this;
}

Word& Word::withPrefix(std::optional<std::string> prefix) {
    if (prefix) {
        pre = SizedText<MaxWordSize>::fromString(prefix.value());
        uint8_t updated = static_cast<uint8_t>(this->metadata);
        updated |= 0b00000100;
        this->metadata = static_cast<WordFixNotation>(updated);
    }
    return *this;
}

std::string Word::get() {
    std::string output;
    if (pre.getSize()) {
        output += std::string{pre.c_str()};
    }

    if (word.getSize()) {
        output += std::string{word.c_str()};
    }

    if (post.getSize()) {
        output += std::string{post.c_str()};
    }

    return output;
}

bool Word::operator==(const Word& other) const { return this == &other; }

void SentenceGenerator::add(Word&& word) { wordSequence_.push_back(word); }

std::string SentenceGenerator::get() {
    bool isFirst = true;

    for (auto cword_itr = wordSequence_.cbegin();
         cword_itr != wordSequence_.cend(); cword_itr++) {
        if (isFirst) {
            oss_ << wordPattern.pre.c_str();
            isFirst = false;
        }

        if (cword_itr == --(wordSequence_.cend())) {
            oss_ << cword_itr->word.c_str() << wordPattern.post.c_str();
        } else {
            oss_ << cword_itr->word.c_str() << wordPattern.word.c_str();
        }
    }
    return oss_.str();
}

std::optional<std::string> SentenceGenerator::getNext(size_t N) {
    oss_.str("");

    if (firstGetNext_) {
        itr_ = wordSequence_.begin();
        firstGetNext_ = false;
    }

    size_t minSize = std::min(N, wordSequence_.size());
    LOG_V() << "Minimum size is " << minSize;

    bool isFirst = true;
    for (uint32_t i = 0; i < minSize; i++) {
        if ((itr_ == --(wordSequence_.end())) || i == (minSize - 1)) {
            oss_ << itr_->word.c_str() << wordPattern.post.c_str();
            itr_++;
            break;
        } else if (itr_ >= wordSequence_.end()) {
            break;
        } else {
            if (isFirst) {
                oss_ << wordPattern.pre.c_str();
                isFirst = false;
            }
            oss_ << itr_->word.c_str() << wordPattern.word.c_str();
            itr_++;
        }
    }

    std::string result = oss_.str();
    if (result.empty()) {
        return {};
    }
    return result;
}

#ifdef UNIT_TEST
#include <gtest/gtest.h>

TEST(Word, defaultWord) {
    auto w = Word{"hello"};
    ASSERT_EQ(w.metadata, WordFixNotation::OnlyWord);
}

TEST(Word, withPostfix) {
    auto w = Word{"this"}.withPostfix("!");
    ASSERT_EQ(w.metadata, WordFixNotation::PostfixWord);
    ASSERT_EQ(w.get(), "this!");
}

TEST(Word, withPrefix) {
    auto w = Word{"this"}.withPrefix("-");
    ASSERT_EQ(w.metadata, WordFixNotation::PrefixWord);
    ASSERT_EQ(w.get(), "-this");
}

TEST(Word, all) {
    auto w = Word{"this"}.withPrefix("a prefix ").withPostfix(" Some post fix");
    ASSERT_EQ(w.metadata, WordFixNotation::All);
    ASSERT_EQ(w.get(), "a prefix this Some post fix");
}

TEST(SentenceGenerator, sentenceWithWordFix) {
    auto sentence_generator =
        SentenceGenerator{Word{"\t"}.withPrefix("<S>").withPostfix("<E>")};
    sentence_generator.add(Word{"text"});
    sentence_generator.add(Word{"is"});
    sentence_generator.add(Word{"between"});
    sentence_generator.add(Word{"S"});
    sentence_generator.add(Word{"and"});
    sentence_generator.add(Word{"E"});
    sentence_generator.add(Word{"and"});
    sentence_generator.add(Word{"tab"});
    sentence_generator.add(Word{"separated"});
    ASSERT_EQ(sentence_generator.get(),
              "<S>text\tis\tbetween\tS\tand\tE\tand\ttab\tseparated<E>");
}

TEST(SentenceGenerator, nextN) {
    auto sentence = SentenceGenerator{Word{" "}};
    sentence.add(Word{"h"});
    sentence.add(Word{"e"});
    sentence.add(Word{"l"});
    sentence.add(Word{"l"});
    sentence.add(Word{"o"});

    auto result = sentence.getNext(8);
    ASSERT_EQ(result.value(), "h e l l o");

    result = sentence.getNext(8);
    ASSERT_FALSE(result.has_value());
}

#endif
