/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@teaccweb.homelinux.net)                                  *
 *   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <tlux/strbrk.h>

namespace tux
{

[[maybe_unused]] string_view strbrk::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";


std::string strbrk::token_t::operator()() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;
}

std::string strbrk::token_t::operator*() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;

}

std::string strbrk::token_t::Mark() const
{
    std::string Str;
    string_view::iterator CStart = mStart - mPosition;

    //int                   l  = 1;
    string_view::iterator cc = CStart;
    // localiser le debut de la ligne;
    while(*cc && (cc > CStart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= CStart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != mSE) && (*cc != '\n') && (*cc != '\r'))
            Str += *cc++;
    }

    Str += '\n'; ///@todo Encode new line.
    for(int x = 1; x < mCol; x++)
        Str += ' ';
    Str += '^';
    return Str;
}

std::string strbrk::token_t::location()
{
    std::ostringstream Str;
    Str << '(' << mLine << ',' << mCol << ')';
    return Str.str();
}


// ------------------------ strbrk::s_p_s --------------------------------


strbrk::s_p_s::s_p_s(std::string_view aStr):
    _begin(aStr.begin()),
    _pos(aStr.begin()),
    _end(aStr.begin())
{

}

/*!
    @note Enleveer le code de trop (suivi du num de ligne et de la col).
*/
bool strbrk::s_p_s::skip_ws()
{
    if(end())
        return false;

    while(isspace(*_pos))
    {
        switch(*_pos)
        {
        case 10:
        {
            if((++_pos) >= _end)
                return false;
            if(*_pos == 13)
                ++_pos;
            ++_line;
            _col = 1;
        }
            break;
        case 13:
        {
            if((++_pos) >= _end)
                return false;
            if(*_pos == 10)
                ++_pos;
            ++_line;
            _col = 1;
        }
            break;
        case '\t':++_pos;
            ++_col;
            break;
        default:++_pos;
            ++_col;
            break;
        }
    }
    return _pos < _end;

}


bool strbrk::s_p_s::end() const
{
    return _pos >= _end;
}


bool strbrk::s_p_s::operator++()
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}
bool strbrk::s_p_s::operator++(int)
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}

strbrk::s_p_s &strbrk::s_p_s::operator>>(strbrk::token_t &aWord)
{
    aWord.mStart    = _pos;
    aWord.mLine     = _line;
    aWord.mCol      = _col;
    aWord.mPosition = _index = (uint64_t) (_pos - _begin);
    return *this;
}

strbrk::iterator strbrk::scan_to(strbrk::iterator aStart, char c) const
{
    strbrk::iterator p = aStart;
    ++p;
    while((p != _d.end()) && (*p != c))
        ++p;
    return p;
}


/*!
    * @brief break/split/tokenize,etc... the content of a string into pieces.
    * @param strbrk::token_t::list  OUTPUT reference to the 'Tokens array' containter, filled by this method.
    * @param aDelimiters Separators in the form of a std::sting_view of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as token where they appear
    * @return number of "Words/tokens" contained into the collection.
    * @notice : After several years of experience and experimentations, We have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c aDelimiters.
    */

std::size_t strbrk::operator()(strbrk::token_t::list &Collection, string_view aDelimiters, bool KeepAsWord) const
{

    auto Crs = strbrk::s_p_s(_d);
    if(_d.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.reset(_d);
    string_view token_separators = aDelimiters.empty() ? strbrk::_default_token_separators : aDelimiters;
    if(!Crs.skip_ws())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    token_t w;
    Crs >> w;

    while(!Crs.end())
    {
        //if (!wcollection.empty());
        strbrk::iterator cc = Crs._pos;
        if(token_separators.find(*Crs._pos) != string_view::npos)
        {
            cc = Crs._pos;
            if(cc > w.mStart)
            {
                --cc;
                Collection.push_back({w.mStart, cc, Crs._end, w.mLine, w.mCol, w.mPosition});
                Crs >> w;
                cc = Crs._pos;
            }

            // '//' as one token_t instead of having two consecutive '/'
            if((*Crs._pos == '/') && (*(Crs._pos + 1) == '/'))
                ++Crs;

            if(KeepAsWord)
            {
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.end())
                Crs >> w;
            else
            {
                return Collection.size();
            }

        }
        else if((*Crs._pos == '\'') || (*Crs._pos == '"'))
        { // Quoted litteral string...
            Crs >> w;
            if(KeepAsWord)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the token_t coords anyway.
                Collection.push_back({w.mStart, w.mStart, Crs._end, w.mLine, w.mCol, w.mPosition});
            }

            strbrk::iterator p = scan_to(w.mStart + (KeepAsWord ? 0 : 1), *Crs._pos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs._pos < p)
                ++Crs; // compute white spaces!!!

            if(KeepAsWord)
            {
                // then push the litteral that is inside the quotes.
                Collection.push_back({w.mStart + 1, p - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                Collection.push_back({w.mStart, p, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the token_t.
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            if(++Crs)
                Crs >> w;
            else
                return Collection.size();

        }
        else
        {
            cc = Crs._pos;
            ++cc;
            if(cc == Crs._end)
            {
                ++Crs._pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.mStart < cc)
                {
                    Collection.push_back({w.mStart, cc - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                    ++Crs;
                }

                if(Crs.skip_ws())
                {
                    Crs >> w;
                    continue;
                }
                return Collection.size();
            }
            if(!Crs.end())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs._pos > w.mStart)
        Collection.push_back({w.mStart, Crs._pos - 1, Crs._end, w.mLine, w.mCol, w.mPosition});

    return Collection.size();
}

//strbrk& strbrk::operator>>(strbrk::token_t::List& Collection)
//{
//    mConfig.z = Tokenize(mConfig.Collection, mConfig.Delim, mConfig.Keep);
//    return *this;
//}

strbrk::strbrk(string_view aStr):
    _d(aStr)
{

}
strbrk &strbrk::operator=(string_view aStr)
{
    _d = aStr;
    return *this;
}
strbrk &strbrk::operator=(const char *aStr)
{
    _d = aStr;
    return *this;
}
strbrk::strbrk(char *aStr): _d(aStr)
{

}
strbrk::strbrk(string_view aStr, string_view Delim, bool KeepDelim):
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}


strbrk::strbrk(char* aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}




strbrk::strbrk(const std::string& aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}



strbrk::strbrk(const char* aStr) : _d(aStr)
{}


strbrk::strbrk(std::string aStr): _d(aStr.c_str())
{

}

}
