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


namespace tux::cmd
{


class TUXLIB arg
{
    int _required_args = 0;
    stracc::list _args;
    uint8_t _opt = 0;
    std::string _opt_name;
    std::string _desc = "no descriptions";
    char        _c=0;

public:
    struct TUXLIB context
    {
        stracc::iterator begin;
        stracc::iterator end;
        stracc::iterator cur;
    };

    signal<arg&, arg::context&> _signal{ "arg" };

    static constexpr uint8_t Required = 0x01;
    static constexpr uint8_t ValRequired = 0x01;

    using list = std::vector<arg>;
    using iterator = arg::list::iterator;
    
    arg(const std::string& opt_name_, char letter_, uint8_t opt_ = 0, int require_narg = 0);
    ~arg() {_args.clear();}
    
    template<typename T> signal::iterator  connect(T* inst_, expect<>(T::* fn)(arg&, arg::context&))
    {
        return _signal.connect(inst_, fn);
    }
private:

    signal<arg&, arg::context&> _signal{ "arg" };
    friend class env_args;
};



class TUXLIB env_args
{
    cmd::arg::list  _args;
    stracc::list    _argv;

public:
    env_args() {}
    env_args(int argc, char** argv);
    ~env_args();

    arg& operator += (arg&& _arg);
    arg& add(arg&& _arg);
    expect<> compile();
    expect<> execute();

};

} // namespace tux::cmd


