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

#include "clarity.h"
#include <iostream>

int main()
{
    try
    {
        vector<vector<string>> board {
            { "z", "w", "p", "u", "m", "o", "s" },
            { "p", "w", "w", "n", "z", "r", "w" },
            { "c", "d", "h", "q", "d", "p", "e" },
            { "w", "c", "u", "x", "d", "n", "q" },
            { "r", "c", "s", "d", "k", "w", "q" },
            { "i", "c", "m", "p", "r", "x", "x" },
            { "o", "y", "g", "u", "i", "x", "m" },
        };

        for (auto word: solve(board))
            std::cout << word << "\n";
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return -1;
    }
}
