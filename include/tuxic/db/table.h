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
#include<tuxic/object.h>
#include <sqlite3.h>
#include<tuxic/diagnostic.h>

namespace tux::db
{



class field: public object
{
    std::string _description="no description";
    object* ref_table = nullptr;
    field*  ref_field = nullptr;

public:
    enum type:uint8_t {
        Serial, ///< AUTO_INCREMENT
        Integer,
        String,
        Text,
        Password,
        Binary,
        Date,
        Stamp,
        Unset
    };


    static constexpr uint8_t Primary   = 1;
    static constexpr uint8_t Reference = 2;
    static constexpr uint8_t Unique    = 4;
    static constexpr uint8_t Null      = 8;

private:
    friend class table;
    field::type _type = field::type::Unset;
    int  _len = 0;

    uint8_t _attr = 0;


public:
    field() = default;
    field(object* atable, const std::string& aid, field::type atype);
    field(const std::string& aid, field::type atype, uint8_t aconstrain);
    field(field&& af)noexcept;
    field(const field& af);
    field& operator=(field&& af)noexcept;
    field& operator=(const field& af);

    bool is_primary_key() const { return _attr & field::Primary; } ///< field::primary_key() = true;
    void set_primary() { _attr |= Primary; }
    void set_null(bool s);
    bool is_null(bool s) const  { return _attr & field::Null}
    void set_foreign(bool s);
    bool is_foreign() const { return _attr & field::Reference; }
    void set_length(int l)   { _len = l; }

    code::T attributes_text(stracc& qacc) const;
    code::T set_foreign_key(field& aref_field);

    code::T text(stracc& qacc) const;
    code::T sql_type(stracc& qacc) const;
    operator bool() const{ return _type != field::type::Unset; }
};



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
    code::T set_unique(stracc::list fields);
    code::T text(stracc& query_acc);
};

} // namespace tux:db

