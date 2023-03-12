/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@tuxweb.homelinux.net)                                  *
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

#pragma once
#include <tlux/stracc.h>
#include <tlux/stracc.h>

namespace tux
{
//using std::string;
using std::string;

class TUXLIB strbrk
{
    std::string _d;

     [[maybe_unused]] static std::string _default_token_separators;
     using iterator = std::string::const_iterator;
     using list     = std::vector<std::string>;
 public:
     strbrk() = default;
     explicit strbrk(const std::string& aStr);
     explicit strbrk(char* aStr);
     strbrk(const char* aStr);
     explicit strbrk(std::string, std::string Delim, bool KeepDelim);
     explicit strbrk(char* aStr, std::string Delim, bool KeepDelim);
     explicit strbrk(const std::string& aStr, std::string Delim, bool KeepDelim);

     ~strbrk() = default;

     struct TUXLIB  token_t
     {
         std::string::const_iterator mStart;
         std::string::const_iterator mEnd;
         std::string::const_iterator mSE;

         std::string operator()() const;
         std::string operator*() const;

         using list = std::vector<strbrk::token_t>;
         using iterator = strbrk::token_t::list::iterator;
         [[maybe_unused]] [[nodiscard]] std::string Mark() const;

         int         mLine     = 1;
         int         mCol      = 1;
         std::size_t mPosition = 0;

         std::string location();
     };

     struct TUXLIB  config_data
     {
         strbrk::token_t::list tokens;
         std::string delimiters;
         bool keep;
         size_t z;
     };


     std::size_t operator()(strbrk::token_t::list &Collection, std::string aDelimiters = "", bool KeepAsWord = true) const;
     //strbrk& operator>>(strbrk::Token::list& Collection);

     strbrk::iterator scan_to(strbrk::iterator start_, char c_) const;
 private:
     struct s_p_s
     {
         std::string::const_iterator _begin;
         std::string::const_iterator _pos;
         std::string::const_iterator _end; /// ...

         int      _line  = 1;
         int      _col   = 1;
         uint64_t _index = 0;

         s_p_s() = default;
         ~s_p_s() = default;

         explicit s_p_s(const std::string& str_);
         bool skip_ws();
         [[nodiscard]] bool end() const;
         bool operator++();
         bool operator++(int);
         void reset(const std::string& str_)
         {
             _pos   = _begin = str_.cbegin();
             _line  = _col   = 1;
             _index = 0;
             _end  = str_.cend();
         }
         s_p_s &operator>>(strbrk::token_t &word_);
     } _cursor;
 public:
     strbrk& operator=(const std::string& str_);
     strbrk& operator=(const char* str_);
     std::string operator()(){ return _d.data(); }
 private:
     config_data _cfg;

};


}
