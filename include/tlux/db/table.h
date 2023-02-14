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
#include<tlux/object.h>
#include <sqlite3.h>
#include <tlux/db/field.h>

namespace tux::db
{




/*!
 * @brief Database table class representing the table::schema in sqlite.
 *
 * The parent object pointer refers to the instance of the sqlite (database file) schema where this table/table is defined.
 * The children object pointers are the columns/fields of this table/table;
 * The table's [select] list is the selected columns/fields
 * @note   The tux::object::id field is the name of the column in the sqlite table schema.
 * @author &copy; 2023, Serge Lussier (oldlonecoder@gmail.com)
 */
class  table : public object
{
    // definitions
    object::list _selected; ///< Selected field for
    object::list _uniques;

    //-----------------
public:

    enum class type : uint8_t
    {
        Primary,
        Link,
        View,
        Trigger
    } _type = table::type::Primary;

    table() = default;
    ~table() override;

    table(object* aparent, const std::string& aid);

    field* add_field(field&& af);
    table& operator << (field&& f);
    field& operator[](const std::string& fid);
    code::T add_foreign(const std::string& fname, const std::string& ftablename, const std::string& fcolname);
    field& query_field_by_name(const std::string& f_id);
    field* field_by_id(const std::string& fname);
    code::T set_unique(stracc::list fields);
    code::T text(stracc& query_acc);
};

} // namespace tux:db

