/****************************************************************************
 Copyright (c) 2014      cocos2d-x.org
 Copyright (c) 2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __cocos2dx__ccUTF8__
#define __cocos2dx__ccUTF8__

#include "base/CCPlatformMacros.h"
#include <vector>
#include <string>

NS_CC_BEGIN


CC_DLL void cc_utf8_trim_ws(std::vector<char16_t>* str);

/**
 * Whether the character is a whitespace character.
 *
 * @param ch    the unicode character
 * @returns     whether the character is a white space character.
 *
 * @see http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 * */
CC_DLL bool isspace_unicode(char16_t ch);

/**
 * Whether the character is a Chinese/Japanese/Korean character.
 *
 * @param ch    the unicode character
 * @returns     whether the character is a Chinese character.
 *
 * @see http://www.searchtb.com/2012/04/chinese_encode.html
 * @see http://tieba.baidu.com/p/748765987
 * */
CC_DLL bool iscjk_unicode(char16_t ch);

/**
 * Returns the length of the string in characters.
 *
 * @param p     pointer to the start of a UTF-8 encoded string.
 * @param max   the maximum number of bytes to examine. If \p max is less than
 *              0, then the string is assumed to be null-terminated. If \p max
 *              is 0, \p p will not be examined and my be %nullptr.
 *
 * @returns the length of the string in characters
 **/
CC_DLL long
cc_utf8_strlen (const char * p, int max);

/**
 * Find the last character that is not equal to the character given.
 *
 * @param str   the string to be searched.
 * @param c     the character to be searched for.
 *
 * @returns the index of the last character that is not \p c.
 * */
CC_DLL unsigned int cc_utf8_find_last_not_char(std::vector<char16_t> str, char16_t c);

CC_DLL std::vector<char16_t> cc_utf16_vec_from_utf16_str(const std::u16string& utf16);


CC_DLL std::u16string cc_utf8_to_utf16(const std::string& utf8);
CC_DLL std::string cc_utf16_to_utf8 (const std::u16string& utf16);

NS_CC_END

#endif /* defined(__cocos2dx__ccUTF8__) */
