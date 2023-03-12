/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@teaccweb.homelinux.net)                                  *
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

#include <tlux/stml/text.h>
#include <tlux/application.h>


class test : public tux::application
{
    tux::text text_str;

    tux::signal<int> Fini;
    tux::signal<int>::accumulator acc;

public:

    test(int argc, char** argv);
    ~test() override;

    tux::code::M init() override;
    tux::code::M run() override;
    tux::code::M terminate() override;

    tux::expect<> text(tux::cmd::arg& a, tux::cmd::arg::context& ctx);

    template<typename T> auto connect_term(T* obj, tux::expect<>(T::* fun)(int))
    {
        return Fini.connect(obj, fun);
    }
    

};

