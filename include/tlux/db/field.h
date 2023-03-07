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
//#include <sqlite3.h>
#include<tlux/diagnostic.h>

namespace tux::db
{

class field : public object
{
    std::string _label="field";
    object* ref_table = nullptr;
    field*  ref_field = nullptr;
    int _len = 0;
    std::vector<uint8_t> _rows;  ///< in case field needs to hold data, then here it is.
public:
    enum type:uint8_t {
        Integer,
        Float,
        String,
        Text,
        Password,
        Binary,
        Date,
        Time,
        DateTime,
        Unset
    };



    static constexpr uint8_t Primary        = 1;
    static constexpr uint8_t Reference      = 2;
    static constexpr uint8_t Unique         = 4;
    static constexpr uint8_t Null           = 8;
    static constexpr uint8_t DefaultDate    = 0x10;
    static constexpr uint8_t DefaultTime    = 0x20;
    static constexpr uint8_t DefaultStamp   = 0x20;

private:
    field::type T = field::type::Unset;
    uint8_t     A = 0;
public:
    field();
    ~field()  override;

    field(field&& f) noexcept;
    field(const field& f);

    field(object* atable, const std::string& aid, field::type atype, uint8_t attr = 0);
    field(const std::string& aid, field::type atype, uint8_t attr = 0);




    field& operator = (field&& f)noexcept;
    field& operator = (const field& f);
    field& operator = (field& f);

    bool is_primary_key() const { return A & field::Primary; } ///< field::primary_key() = true;
    void set_primary() { A |= field::Primary; }
    void set_null(bool s);
    bool is_null(bool s) const  { return A & field::Null;}
    void set_foreign(bool s);
    bool is_foreign() const { return A & field::Reference; }
    void set_length(int l)   { _len = l; }

    code::M attributes_text(stracc& qacc) const;
    code::M set_foreign_key(field& aref_field);

    code::M text(stracc& qacc) const;
    code::M sql_type(stracc& qacc) const;
    operator bool() const{ return T != field::type::Unset; }

};


}