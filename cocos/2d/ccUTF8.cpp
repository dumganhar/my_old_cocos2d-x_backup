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

//static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> __conversion;

/*
 * @str:    the string to search through.
 * @c:        the character to not look for.
 *
 * Return value: the index of the last character that is not c.
 * */
unsigned int cc_utf8_find_last_not_char(std::vector<char16_t> str, char16_t c)
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
static void cc_utf8_trim_from(std::vector<char16_t>* str, int index)
{
    int size = static_cast<int>(str->size());
    if (index >= size || index < 0)
        return;
    
    str->erase(str->begin() + index, str->begin() + size);
}

/*
 * @ch is the unicode character whitespace?
 *
 * Reference: http://en.wikipedia.org/wiki/Whitespace_character#Unicode
 *
 * Return value: weather the character is a whitespace character.
 * */
bool isspace_unicode(char16_t ch)
{
    return  (ch >= 0x0009 && ch <= 0x000D) || ch == 0x0020 || ch == 0x0085 || ch == 0x00A0 || ch == 0x1680
    || (ch >= 0x2000 && ch <= 0x200A) || ch == 0x2028 || ch == 0x2029 || ch == 0x202F
    ||  ch == 0x205F || ch == 0x3000;
}

bool iscjk_unicode(char16_t ch)
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

void cc_utf8_trim_ws(std::vector<char16_t>* str)
{
    int len = static_cast<int>(str->size());
    
    if ( len <= 0 )
        return;
    
    int last_index = len - 1;
    
    // Only start trimming if the last character is whitespace..
    if (isspace_unicode((*str)[last_index]))
    {
        for (int i = last_index - 1; i >= 0; --i)
        {
            if (isspace_unicode((*str)[i]))
                last_index = i;
            else
                break;
        }
        
        cc_utf8_trim_from(str, last_index);
    }
}

std::u16string cc_utf8_to_utf16(const std::string& utf8)
{
    std::u16string ret;
    
    ret.resize(utf8.length()+1);
    
    UTF16* utf16Start = (UTF16*)ret.data();
    UTF16* utf16End = ((UTF16*)ret.data()) + utf8.length();
    
    UTF8* utf8Start = (UTF8*)utf8.data();
    UTF8* utf8End = ((UTF8*)utf8.data()) + utf8.length();
    
    ConvertUTF8toUTF16((const UTF8 **) &utf8Start, utf8End, &utf16Start, utf16End, strictConversion);
    
    return ret;
}

std::vector<char16_t> cc_utf16_vec_from_utf16_str(const std::u16string& str)
{
    std::vector<char16_t> str_new;
    
    size_t len = str.length();
    for (size_t i = 0; i < len; ++i)
    {
        str_new.push_back(str[i]);
    }
    return str_new;
}

std::string cc_utf16_to_utf8 (const std::u16string& utf16)
{
    std::string ret;
    const size_t utf8Bytes = (utf16.length() * 4) + 1;
    ret.resize(utf8Bytes);
    
    UTF8 *utf8Start = (UTF8*)ret.data();
    UTF8 *utf8End = ((UTF8*)ret.data()) + utf8Bytes;
    
    UTF16* utf16Start = (UTF16*)utf16.data();
    UTF16* utf16End = ((UTF16*)utf16.data()) + utf16.length();
    
    ConvertUTF16toUTF8((const UTF16 **) &utf16Start, utf16End, &utf8Start, utf8End, strictConversion);
    
    return ret;
}

NS_CC_END
