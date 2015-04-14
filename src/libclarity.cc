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

#include <algorithm>
#include <cstdint>
#include <set>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <regex>
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
using std::regex;
using std::regex_match;

namespace {
    /** A regex used to detect all-lowercase strings. */
    regex lowercase_rx { "^[a-z]+$" };

    /** Our set of known-good words, loaded at runtime from a
      * dictionary */
    set<string> good_words;

    /** @fn set<string> make_words_from(vvstring board, uint32_t x, uint32_t y, string sofar="")
      *
      * @brief Find all words that can be generated from a given
      * starting point on a board
      *
      * @param board A matrix of strings representing the Boggle board
      * @param x The x-coordinate of the matrix to start searching from
      * @param y The y-coordinate of the matrix to start searching from
      * @param sofar The string that has been found so far in the search
      *
      * @exception std::out_of_range Only if there's a logic error somewhere
      * in the code */
    set<string> make_words_from(vvstring board,
                                uint32_t x,
                                uint32_t y,
                                string sofar = "")
    {
        set<string> wordset;
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
            return wordset;

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
            return wordset;

        /* Finally, if the insertion point matches the word
         * we've constructed, then we've got a good word.
         * Add it to our list and see what more words we can
         * make from here. */

        if (*lookup == sofar)
            wordset.insert(sofar);

        /* End the AI techniques. */

        board.at(y).at(x) = " ";

        for (auto row = min_y ; row <= max_y ; ++row)
            for (auto col = min_x ; col <= max_x ; ++col)
                if (board.at(row).at(col) != " ") {
                    auto found_words = make_words_from(board,
                                                       col,
                                                       row,
                                                       sofar);
                    wordset.insert(found_words.begin(),
                                   found_words.end());
                }
        return wordset;
    }
}

/** Returns the string 'no dictionary found' */
const char* NoDictionaryFound::what() const noexcept
{
    return "no dictionary found";
}

/** Returns the string 'bad board size' */
const char* BadBoard::what() const noexcept
{
    return "bad board";
}

lstring solve(const vvstring& board)
{
    /* Always, always, always, always, validate user input before
     * you begin processing it.  For Clarity there are three conditions
     * we need to test. */

    /* One: if we can't load the word dictionary, throw an exception
     * alerting the programmer to the problem. */

    if (good_words.size() == 0) {
        ifstream infile(PKGDATADIR "/wordlist.txt");
        if (not infile) throw NoDictionaryFound();
        good_words = set<string>(iiter(infile), iiter());
    }

    /* Two: if we're given an empty board, throw an exception and don't
     * process it further. */

    if (board.size() == 0)
        throw BadBoard();

    /* Three: if the rows are of different sizes, or if the contents
     * aren't strictly lowercase, throw an exception and don't process
     * it further. */

    const auto first_row_size = board.at(0).size();
    for (const auto& row: board) {
        if (first_row_size != row.size())
            throw BadBoard();
        for (const auto& str: row)
            if (not regex_match(str, lowercase_rx))
                throw BadBoard();
        }

    /* Our input is sane and we have our word dictionary.  It should be
     * all smooth sailing from here, right?
     *
     * No, it's not.  Resist the temptation to think "but errors can't
     * happen here!"  Whenever a programmer says that, he or she is almost
     * certainly wrong. */


    set<string> wordset;

    for (uint32_t row = 0 ; row < board.size() ; ++row)
        for (uint32_t col = 0 ; col < board.at(row).size() ; ++col) {

            /* Why do we have this try block?  Because make_words_from
             * accesses the board, and it's theoretically possible that
             * a bug might result in the function trying to access
             * outside the board's boundaries.  If that happens a
             * std::out_of_range exception will be raised.  Since it's
             * possible that a bug in our code could result in that
             * exception being raised, it's just prudent programming
             * to catch it. */

            try
            {
                auto found_words = make_words_from(board, col, row);
                wordset.insert(found_words.begin(), found_words.end());
            }
            catch (const std::out_of_range& oor)
            {
#ifdef DEBUG
                std::cerr << "Caught an out-of-range exception\n";
#else

                /* There's nothing the user can do, so just silently
                 * do nothing. */

#endif
            }
        }
    lstring rv(wordset.begin(), wordset.end());
    rv.sort();
    return rv;
}
