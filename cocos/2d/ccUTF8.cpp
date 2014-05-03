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

#include "ccUTF8.h"
#include "2d/platform/CCCommon.h"
#include "base/CCConsole.h"
//#include <codecvt>
//#include <locale>
#include "ConvertUTF.h"

NS_CC_BEGIN

namespace StringUtils {

/*
 * @str:    the string to search through.
 * @c:        the character to not look for.
 *
 * Return value: the index of the last character that is not c.
 * */
unsigned int getIndexOfLastNotChar16(const std::vector<char16_t>& str, char16_t c)
{
    int len = static_cast<int>(str.size());

    int i = len - 1;
    for (; i >= 0; --i)
        if (str[i] != c) return i;

    return i;
}

/*
 * @str:    the string to trim
 * @index:    the index to start trimming from.
 *
 * Trims str st str=[0, index) after the operation.
 *
 * Return value: the trimmed string.
 * */
static void trimUTF16VectorFromIndex(std::vector<char16_t>& str, int index)
{
    int size = static_cast<int>(str.size());
    if (index >= size || index < 0)
        return;

    str.erase(str.begin() + index, str.begin() + size);
}

/*
 * @ch is the unicode character whitespace?
 *
 * Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 * Return value: weather the character is a whitespace character.
 * */
bool isUnicodeSpace(char16_t ch)
{
    return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
    || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
    ||  ch == 0x205F || ch == 0x3000;
}

bool isCJKUnicode(char16_t ch)
{
    return (ch >= 0x4E00 && ch <= 0x9FBF)   // CJK Unified Ideographs
        || (ch >= 0x2E80 && ch <= 0x2FDF)   // CJK Radicals Supplement & Kangxi Radicals
        || (ch >= 0x2FF0 && ch <= 0x30FF)   // Ideographic Description Characters, CJK Symbols and Punctuation & Japanese
        || (ch >= 0x3100 && ch <= 0x31BF)   // Korean
        || (ch >= 0xAC00 && ch <= 0xD7AF)   // Hangul Syllables
        || (ch >= 0xF900 && ch <= 0xFAFF)   // CJK Compatibility Ideographs
        || (ch >= 0xFE30 && ch <= 0xFE4F)   // CJK Compatibility Forms
        || (ch >= 0x31C0 && ch <= 0x4DFF);  // Other exiensions
}

void trimUTF16Vector(std::vector<char16_t>& str)
{
    int len = static_cast<int>(str.size());

    if ( len <= 0 )
        return;

    int last_index = len - 1;

    // Only start trimming if the last character is whitespace..
    if (isUnicodeSpace(str[last_index]))
    {
        for (int i = last_index - 1; i >= 0; --i)
        {
            if (isUnicodeSpace(str[i]))
                last_index = i;
            else
                break;
        }

        trimUTF16VectorFromIndex(str, last_index);
    }
}

bool UTF8ToUTF16(const std::string& utf8, std::u16string& outUtf16)
{
    if (utf8.empty())
    {
        outUtf16.clear();
        return true;
    }

    bool ret = false;
    const size_t utf16Bytes = (utf8.length()+1) << 1;
    char16_t* utf16 = (char16_t*)malloc(utf16Bytes);
    memset(utf16, 0, utf16Bytes);

    UTF16* utf16Start = (UTF16*)utf16;
    UTF16* utf16End = ((UTF16*)utf16) + (utf8.length());

    const UTF8* utf8Start = (const UTF8*)utf8.data();
    const UTF8* utf8End = ((const UTF8*)utf8.data()) + utf8.length();

    if (conversionOK == ConvertUTF8toUTF16((const UTF8 **) &utf8Start, utf8End, &utf16Start, utf16End, strictConversion))
    {
        outUtf16 = utf16;
        ret = true;
    }

    free(utf16);

    return ret;
}

bool UTF16ToUTF8(const std::u16string& utf16, std::string& outUtf8)
{
    if (utf16.empty())
    {
        outUtf8.clear();
        return true;
    }

    bool ret = false;
    const size_t utf8Bytes = (utf16.length() << 2) + 1;
    char* utf8 = (char*)malloc(utf8Bytes);
    memset(utf8, 0, utf8Bytes);

    UTF8 *utf8Start = (UTF8*)utf8;
    UTF8 *utf8End = ((UTF8*)utf8) + (utf8Bytes -1);

    const UTF16* utf16Start = (const UTF16*)utf16.data();
    const UTF16* utf16End = ((const UTF16*)utf16.data()) + utf16.length();

    if (conversionOK == ConvertUTF16toUTF8(&utf16Start, utf16End, &utf8Start, utf8End, strictConversion))
    {
        outUtf8 = utf8;
        ret = true;
    }

    free(utf8);

    return ret;
}

std::vector<char16_t> getUTF16VectorFromUTF16String(const std::u16string& str)
{
    std::vector<char16_t> str_new;

    size_t len = str.length();
    for (size_t i = 0; i < len; ++i)
    {
        str_new.push_back(str[i]);
    }
    return str_new;
}


long getCharacterCountInUTF8String(const std::string& utf8)
{
    return getUTF8StringLength((const UTF8*)utf8.c_str());
}

} //namespace StringUtils {

NS_CC_END
