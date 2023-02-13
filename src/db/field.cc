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



#include <tlux/db/table.h>

namespace tux::db
{



#pragma region field

field::field():object() {}

field::field(object *atable, const std::string &aid, field::type atype, uint8_t a):object(atable,aid),
T(atype), A(a){}

field::field(const std::string &aid, field::type atype, uint8_t aconstrain):object(nullptr, aid)
{
    T = atype;
    A = aconstrain;
}

field::field(field &&af) noexcept : object(af)
{
    _label = std::move(af._label);
    A = af.A;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    T = af.T;
}

field::field(const field &af):object(af)
{
    _label = std::move(af._label);
    A = af.A;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    T = af.T;

}

field::~field()
{
    _label.clear();

}


field &field::operator=(field &&af) noexcept
{
    object::operator=(af);
    _label = std::move(af._label);
    A = af.A;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    A = af.A;
    return *this;
}

void field::set_null(bool s)
{
    if(s)
        A |= field::Null;
    else
        A &= ~field::Null;
}

void field::set_foreign(bool s)
{
    if(s)
        A |= field::Reference;
    else
        A &= ~field::Reference;
}

field &field::operator=(const field &af)
{
    object::operator=(af);
    _label = af._label;
    A = af.A;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    T = af.T;
    return *this;
}

code::T field::attributes_text(stracc& qacc) const
{
    if(!(A & field::Null))
        qacc << " not null";

    if(A & field::Reference)
    {
        qacc << " REFERENCES " << ref_table->id() <<"(" << ref_field->id() << ")";
        return code::accepted;
    }
    else
    {
        if(A & field::Primary)
        {
            qacc << " PRIMARY KEY";
        }
    }
    return code::accepted;
}
/*!
 * @brief std::string operator serializes the column's descriptions for the create or alter pgsql table column.
 * @return std::string resulting SQL execute_query text line.
 */
code::T field::text(stracc& qacc) const
{
    qacc <<  id(); ///< Get the column name from this object::id().
    qacc << " ";
    return sql_type(qacc);
}

code::T field::sql_type(stracc& qacc) const
{
    stracc str;
    switch(T)
    {
        case field::Binary:qacc<<  "bytea";break;
        case field::String:qacc << "VARCHAR(" << (_len ? _len : 50) << ")"; break;
        case field::Text:qacc << "text"; break;
        case field::Date: qacc <<"date"; break;
        case field::Stamp: qacc << "timestamp"; break;
        case field::Password: qacc << "varchar(120)"; break;
        //case field::Integer: qacc <<  "integer"; break;
        default: qacc << "integer"; break;
    }

    return attributes_text(qacc);
}



code::T field::set_foreign_key(field &aref_field)
{
    auto *tbl = aref_field.parent<table>();
    ref_field = &aref_field;
    ref_table = tbl;
    A |= field::Reference;
    return code::accepted;
}


#pragma endregion field

//-------------------------------------------------------------------------------------------------------------------

}
