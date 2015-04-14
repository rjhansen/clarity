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

/** @file */

#ifndef _CLARITY_H
#define _CLARITY_H

#include <vector>
#include <string>
#include <list>
#include <exception>

using std::list;
using std::string;
using std::vector;

/** @fn list<string> solve(const vector<vector<string>>& board)
  *
  * @brief Solves a Boggle board.
  *
  * The board must be rectangular (i.e., non-jagged), non-empty
  * (having at least one square), and composed entirely of
  * lowercase strings.  If any of these conditions fail to hold
  * a BadBoard exception will be raised.
  *
  * @param board A const reference to a matrix of strings
  * @return A sorted list of words found in the board
  * @exception BadBoard board is jagged, empty, or holds invalid data
  * @exception NoDictionaryFound word dictionary could not be found */
list<string> solve(const vector<vector<string>>& board);

/** Thrown in the event the dictionary of good words could not be found. */
class NoDictionaryFound : public std::exception
{
public:
    virtual const char* what() const noexcept;
};

/** Thrown in the event an invalid board is passed to solve(). */
class BadBoard : public std::exception
{
public:
    virtual const char* what() const noexcept;
};

#endif
