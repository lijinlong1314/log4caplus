// Module:  Log4CPLUS
// File:    property.cxx
// Created: 2/2002
// Author:  Tad E. Smith
//
//
// Copyright 2002-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <log4cplus/config.hxx>

#include <cstring>
#if defined (UNICODE)
#  include <cwctype>
#else
#  include <cctype>
#endif
#if defined (LOG4CPLUS_HAVE_CODECVT_UTF8_FACET) \
    || defined (LOG4CPLUS_HAVE_CODECVT_UTF16_FACET) \
    || defined (LOG4CPLUS_HAVE_CODECVT_UTF32_FACET)
#  include <codecvt>
#endif
#include <locale>
#include <log4cplus/helpers/property.h>
#include <log4cplus/fstreams.h>


namespace log4cplus
{


const tchar helpers::Properties::PROPERTIES_COMMENT_CHAR = LOG4CPLUS_TEXT('#');


namespace
{


static
int
is_space (tchar ch)
{
#if defined (UNICODE)
    return std::iswspace (ch);
#else
    return std::isspace (static_cast<unsigned char>(ch));
#endif
}


static
void
trim_leading_ws (tstring & str)
{
    tstring::iterator it = str.begin ();
    for (; it != str.end (); ++it)
    {
        if (! is_space (*it))
            break;
    }
    str.erase (str.begin (), it);
}


static
void
trim_trailing_ws (tstring & str)
{
    tstring::reverse_iterator rit = str.rbegin ();
    for (; rit != str.rend (); ++rit)
    {
        if (! is_space (*rit))
            break;
    }
    str.erase (rit.base (), str.end ());
}


static
void
trim_ws (tstring & str)
{
    trim_trailing_ws (str);
    trim_leading_ws (str);
}


} // namespace



namespace helpers
{

///////////////////////////////////////////////////////////////////////////////
// Properties ctors and dtor
///////////////////////////////////////////////////////////////////////////////

Properties::Properties() 
{
}



Properties::Properties(tistream& input)
{
    init(input);
}



Properties::Properties(const tstring& inputFile, unsigned flags)
{
    if (inputFile.empty ())
        return;

    tifstream file;

    switch (flags & fEncodingMask)
    {
#if defined (LOG4CPLUS_HAVE_CODECVT_UTF8_FACET) && defined (UNICODE)
    case fUTF8:
        file.imbue (
            std::locale (file.getloc (),
                new std::codecvt_utf8<tchar, 0x10FFFF,
                    static_cast<std::codecvt_mode>(std::consume_header | std::little_endian)>));
        break;
#endif

#if defined (LOG4CPLUS_HAVE_CODECVT_UTF16_FACET) && defined (UNICODE)
    case fUTF16:
        file.imbue (
            std::locale (file.getloc (),
                new std::codecvt_utf16<tchar, 0x10FFFF,
                    static_cast<std::codecvt_mode>(std::consume_header | std::little_endian)>));
        break;

#elif defined (UNICODE) && defined (WIN32)
    case fUTF16:
        file.imbue (
            std::locale (file.getloc (),
                new std::codecvt<wchar_t, wchar_t, std::mbstate_t>));
    break;

#endif

#if defined (LOG4CPLUS_HAVE_CODECVT_UTF32_FACET) && defined (UNICODE)
    case fUTF32:
        file.imbue (
            std::locale (file.getloc (),
                new std::codecvt_utf32<tchar, 0x10FFFF,
                    static_cast<std::codecvt_mode>(std::consume_header | std::little_endian)>));
        break;
#endif

    case fUnspecEncoding:;
    default:
        // Do nothing.
        ;
    }

    file.open(LOG4CPLUS_FSTREAM_PREFERED_FILE_NAME(inputFile).c_str(),
        std::ios::binary);
    init(file);
}



void 
Properties::init(tistream& input) 
{
    if (! input)
        return;

    tstring buffer;
    while (std::getline (input, buffer))
    {
        trim_leading_ws (buffer);

        tstring::size_type const buffLen = buffer.size ();
        if (buffLen == 0 || buffer[0] == PROPERTIES_COMMENT_CHAR)
            continue;
        
        // Check if we have a trailing \r because we are 
        // reading a properties file produced on Windows.
        if (buffer[buffLen-1] == LOG4CPLUS_TEXT('\r'))
            // Remove trailing 'Windows' \r.
            buffer.resize (buffLen - 1);

        tstring::size_type const idx = buffer.find('=');
        if (idx != tstring::npos)
        {
            tstring key = buffer.substr(0, idx);
            tstring value = buffer.substr(idx + 1);
            trim_trailing_ws (key);
            trim_ws (value);
            setProperty(key, value);
        }
    }
}



Properties::~Properties() 
{
}



///////////////////////////////////////////////////////////////////////////////
// Properties public methods
///////////////////////////////////////////////////////////////////////////////

tstring
Properties::getProperty(const tstring& key) const 
{
    StringMap::const_iterator it (data.find(key));
    if (it == data.end())
        return LOG4CPLUS_TEXT("");
    else
        return it->second;
}



tstring
Properties::getProperty(const tstring& key,
                                 const tstring& defaultVal) const
{
    StringMap::const_iterator it (data.find (key));
    if (it == data.end ())
        return defaultVal;
    else
        return it->second;
}


std::vector<tstring>
Properties::propertyNames() const 
{
    std::vector<tstring> tmp;
    for (StringMap::const_iterator it=data.begin(); it!=data.end(); ++it)
        tmp.push_back(it->first);

    return tmp;
}



void
Properties::setProperty(const tstring& key, const tstring& value)
{
    data[key] = value;
}


bool
Properties::removeProperty(const tstring& key)
{
    return (data.erase(key) > 0);
}


Properties 
Properties::getPropertySubset(
    const tstring& prefix) const
{
    Properties ret;
    std::vector<tstring> const keys = propertyNames();
    size_t const prefix_len = prefix.size ();
    for (std::vector<tstring>::const_iterator it = keys.begin();
        it != keys.end(); ++it)
    {
        int result = it->compare (0, prefix_len, prefix);
        if (result == 0)
            ret.setProperty (it->substr (prefix_len), getProperty(*it));
    }

    return ret;
}

} // namespace helpers

} // namespace log4cplus
