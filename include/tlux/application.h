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
//#include <tux/object.h>
#include <thread>
#include <tlux/tux_signal.h>
#include <tlux/diagnostic.h>


//#include <tux/db/database.h>

namespace tux {


class application : public object
{
    std::thread _thid;
    tux_signal<code::T> _app_start;
    tux_signal<code::T> _app_end;
    static application* _self;
    diagnostic diagn;
    stracc::list  _args;
    //db::database* _regitery = nullptr;


    code::T init_regitery();
public:
    application();
    ~application() override;

    virtual code::T init(int argc, char** argv);
    virtual code::T run();
    virtual code::T terminate();

    const stracc::list& args()  { return _args; }
    static application& self();
    static diagnostic& diagnostic_instance();
    static code::code_attribute_table& codes_data();
};

} // tux


