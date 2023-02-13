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




#pragma once

#include<tlux/db/table.h>
#include<tlux/strbrk.h>

#include<tlux/tux_signal.h>

namespace tux::db {

class  database : public object
{
    sqlite3*    _file = nullptr;
    object::list _tables;
    std::string _ext = ".data";
    static int sqlite3_callback(void *NotUsed, int argc, char **argv, char **azColName);
    tux_signal<code::T, int, char **, char **> callback_signal;

public:


    database():object(){} // Prevent the compiler to delete this constructor by the compiler when " = default;"
    database(object* aparent, const std::string& aid, const std::string& aext = ".data");
    ~database() override;

    code::T open();
    code::T create();
    code::T close();
    auto set_callback(tux_signal<code::T, int, char **, char **>::slot cb)
    { return callback_signal.connect(cb); }

    code::T execute_query(const std::string& aquery_string);
    table& operator [](const std::string& eid);
    table* add_table(const std::string& ename);
};

} // namespace tux::db
