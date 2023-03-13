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

#include <tuxls/db/table.h>


namespace tux::db
{

// class query; //< forward declaration parce que les structures ci-dessous .....



// struct select : public object
// {
//     field::list field;
// };

// struct insert : public object
// {

// };


// struct update : public object
// {
//     field::list fields;
// };

// struct delete : public object
// {
//     field::list fields;
// };

// // struct  alter : public object
// // {

// // };


// /*!
//     @brief sql_operator n'est PAS un processeur arithmétique!
//     Il sert, parmi les autres objets,  à générer le texte SQL pour les query
//     à partir des objets c++ et ce, le plus naturellement fluide qu'il m'est possible de réaliser.
// */
// struct sql_operator : public object
// {
//     enum op:int8_t {
//         add = 0,
//         sub,
//         inc,
//         dec,
//         between,
//         lequ,
//         gequ,
//         equ,
//         mul,
//         mod,
//         div,
//         in,
//         nin,
//         like,
//         glob,
//         not,
//         or,
//         null,
//         exist,
//         concat,
//         unique,
//         bor,
//         band,
//         bxor,
//         lshift,
//         rshift,
//         c1,
//         count ///< pas vraiment un operateur mais quand-meme..
//     };

// };


// struct where : public object
// {
//     sql_operator::list  operators;
//     //...

// };

/*!
    @brief As of now, this version only holds query composition built from the provided list of tables::fields and bare-base 'WHERE' clause where applied.
*/
class query : public object
{
    field::list _fields; ///< Chaque field a une ref sur la table parent. Donc pas besoin de la liste de tables en plus ici!
    //...
    stracc querystr;

public:
    query();
    query(query&& Q) noexcept : object(Q) {}
    query(const query& Q);


    //query operator << (db::field* af);
    //query operator << (db::field::list&& af);
    query operator << (db::table* atbl);

};
} // namespace tux::db
