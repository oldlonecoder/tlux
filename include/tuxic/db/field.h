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


namespace tux::db
{

class field : public object
{
    std::string _label="field";
    object* ref_table = nullptr;
    field*  ref_field = nullptr;

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
        Stamp,
        Unset
    };



    static constexpr uint8_t Primary   = 1;
    static constexpr uint8_t Reference = 2;
    static constexpr uint8_t Unique    = 4;
    static constexpr uint8_t Null      = 8;

private:
    field::type T = field::type::Unset;
    uint8_t     A = 0;

public:
    field();
    ~field()  override;

    field(field&& f) noexcept;
    field(const field& f) noexcept;

    field& operator = (field&& f)noexcept;
    field& operator = (const field& f)noexcept;
    field& operator = (field& f);

};


}