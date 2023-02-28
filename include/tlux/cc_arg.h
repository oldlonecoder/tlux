/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@teaccweb.homelinux.net)                                *
 *   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
 *   2023: oldlonecoder@arknowledge.page                                   *
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

//#include <tlux/diagnostic.h>
#include <tlux/tux_signal.h>


namespace tux {


class _argument
{
    int _required_args = 0;
    stracc::list _args;
    uint8_t _opt = 0;
    std::string _opt_name;
    char        _c=0;
public:
    static constexpr uint8_t Required = 0x01;
    static constexpr uint8_t ValRequired = 0x01;
    using list = std::vector<cc_arg>;
    using iterator = cc_arg::list::iterator;
    virtual code::T run() = 0;

    _argument(const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0);
    virtual _argument() {_args.clear();}

};


template<typename T> class cc_arg: public _argument
{

    T* _user = nullptr;

public:
    using handler_t = code::T (T::*)(cc_arg<T>&);

    cc_arg() = default;
    cc_arg(cc_arg&&) noexcept = default;
    cc_arg(const cc_arg&) noexcept = default;

    cc_arg(T* user_, typename cc_arg::handler_t h, const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0 ):
        _argument(opt_name_,letter_,require_narg),
        _user(user_),
        _handler(h)
    {}

    ~cc_arg() override {  }

    cc_arg& operator = (cc_arg&&) noexcept = default;
    cc_arg& operator = (const cc_arg&) = default;
    inline int values_count() { return _args.size(); }
private:
    handler_t _handler = nullptr;
};

} // namespace tux


