/* Copyright (c) 2015, Robert J. Hansen <rjh@sixdemonbag.org>
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <set>
#include <fstream>
#include <iterator>
#include "clarity.h"
#include "config.h"

using vvstring = std::vector<std::vector<std::string>>;
using lstring = std::list<std::string>;
using oiter = std::ostream_iterator<std::string>;
using iiter = std::istream_iterator<std::string>;
using std::ifstream;
using std::set;
using std::copy;
using std::remove_if;
using std::mismatch;

namespace {

    set<string> good_words;

    lstring make_words_from(vvstring board,
                            uint32_t x,
                            uint32_t y,
                            string sofar = "")
    {
        lstring wordlist;
        uint32_t min_x, min_y, max_x, max_y;

        min_x = x > 0 ? x - 1 : x;
        min_y = y > 0 ? y - 1 : y;
        max_x = x < board.at(y).size() - 1 ? x + 1 : x;
        max_y = y < board.size() - 1 ? y + 1 : y;

        sofar += board.at(y).at(x);

        /* Begin the AI techniques. */

        /* First: check our current word (sofar) against
         * the dictionary.  If we were to put sofar into
         * the dictionary, where would it go?  If it would
         * go after the final word, we not only know it's
         * no good, but that nothing built from this word
         * can be good, either.  Bail out and return an
         * empty word list. */

        auto lookup = good_words.lower_bound(sofar);
        if (lookup == good_words.cend())
            return wordlist;

        /* Now that we've found the insertion point into the
         * dictionary, does our word match the beginning of
         * that?  For instance, if we have 'qan', we would
         * insert it before 'qanat'.  Since 'qan' matches
         * the beginning of 'qanat', then even though 'qan'
         * isn't a word we still might be able to make one
         * from here.
         *
         * If we had 'qam', though... our insertion point
         * would still say 'qanat', and we'd know that we
         * couldn't make any valid word from here.  Therefore,
         * if we fail the match, we know we can bail out. */

        auto prefix = mismatch(sofar.begin(), sofar.end(),
            lookup->begin(), lookup->end()).first;
        if (prefix != sofar.end())
            return wordlist;

        /* Finally, if the insertion point matches the word
         * we've constructed, then we've got a good word.
         * Add it to our list and see what more words we can
         * make from here. */

        if (*lookup == sofar)
            wordlist.emplace_back(sofar);

        /* End the AI techniques. */

        board.at(y).at(x) = " ";

        for (auto row = min_y ; row <= max_y ; ++row)
            for (auto col = min_x ; col <= max_x ; ++col)
                if (board.at(row).at(col) != " ")
                    for (auto word: make_words_from(board, col, row, sofar))
                        wordlist.emplace_back(word);

        return wordlist;
    }
}

const char* NoDictionaryFound::what() noexcept
{
    return "no dictionary found";
}

list<string> solve(const vvstring& board)
{
    if (good_words.size() == 0) {
        ifstream infile(PKGDATADIR "/wordlist.txt");
        if (not infile) throw NoDictionaryFound();
        good_words = set<string>(iiter(infile), iiter());
    }

    list<string> wordlist;
    for (uint32_t row = 0 ; row < board.size() ; ++row)
        for (uint32_t col = 0 ; col < board.at(row).size() ; ++col)
            for (auto word: make_words_from(board, col, row))
                wordlist.emplace_back(word);
    return wordlist;
}
