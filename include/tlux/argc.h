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
#include <tlux/delegate.h>


namespace tux {

namespace cmd
{


class arg
{
    int _required_args = 0;
    stracc::list _args;
    uint8_t _opt = 0;
    std::string _opt_name;
    std::string _desc = "no descriptions";
    char        _c=0;

public:
    static constexpr uint8_t Required = 0x01;
    static constexpr uint8_t ValRequired = 0x01;

    using list = std::vector<arg*>;
    using iterator = arg::list::iterator;
    virtual code::M run() = 0;

    arg(const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0);
    virtual ~arg() {_args.clear();}

};


template<typename T> class cc_arg: public arg
{

    T* _user = nullptr;

public:
    using handler_t = code::M (T::*)(cc_arg<T>&);

    cc_arg() = default;
    cc_arg(cc_arg&&) noexcept = default;
    cc_arg(const cc_arg&) noexcept = default;

    cc_arg(T* user_, typename cc_arg::handler_t h, const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0 ):
        arg(opt_name_,letter_,require_narg),
        _user(user_),
        _handler(h)
    {}

    ~cc_arg() override {  }

    cc_arg& operator = (cc_arg&&) noexcept = default;
    cc_arg& operator = (const cc_arg&) = default;
    inline int values_count() { return _args.size(); }
    code::M run() override {
        return code::notimplemented;
    }

private:
    handler_t _handler = nullptr;
};

}

class cc_args
{
    cmd::arg::list _args;

public:
    cc_args() {}

    ~cc_args();


    code::M process_args(int argc, char** argv);

    cc_args& operator << (cmd::arg* _arg);



};

} // namespace tux


