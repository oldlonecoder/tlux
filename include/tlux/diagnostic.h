//
// Created by oldlonecoder on 1/27/23.
//

/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@ooweb.homelinux.net)                                   *
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
/*******************************************************************
    File diagnostic created on Friday, 27 of January 2023 at 7:20 PM by oldlonecoder
********************************************************************/

// ----------------------------------
// Get rid of obsolete include guard use this instead! :
#pragma once
// ----------------------------------
#include <tlux/object.h>
#include <tlux/textattr.h>
#include <tlux/utf/Icons.h>
#include <map>
#include <tlux/geometry.h>




namespace tux {

struct  TUXLIB source_location
{
    int line = 0;
    std::string_view filename = "";
    std::string_view function_sig = "";
    explicit operator bool()const { return !((line == 0) && filename.empty() && function_sig.empty()); }
};

#define sfnl    {__LINE__,__FILE__,__FUNCTION__}
#define sfnll   {__LINE__,__FILE__,__PRETTY_FUNCTION__}



struct  TUXLIB code
{
    using M = uint16_t;
    using type = uint8_t;

    static constexpr type none   = 0;
    static constexpr type error  = 1;
    static constexpr type warning= 2;
    static constexpr type fatal  = 3;
    static constexpr type except = 4;
    static constexpr type message= 5;
    static constexpr type output = 6;
    static constexpr type debug  = 7;
    static constexpr type info   = 8;
    static constexpr type comment= 9;
    static constexpr type syntax = 10;
    static constexpr type status = 11;
    static constexpr type test   = 12;

    static constexpr M ok               = 0; ///< Obviously;
    static constexpr M accepted         = 1;
    static constexpr M success          = 2;
    static constexpr M rejected         = 3;
    static constexpr M failed           = 4;
    static constexpr M empty            = 5;
    static constexpr M full             = 6;
    static constexpr M notempty         = 7;
    static constexpr M implemented      = 8; ///< Like notimplemented or already implemented
    static constexpr M notimplemented   = 9; ///< "Please, implement"
    static constexpr M untested         = 10;
    static constexpr M eof              = 11; ///< end of file
    static constexpr M eos              = 12; ///< end of stream or string or statement or state ( machine state )
    static constexpr M null_ptr         = 13; ///< It will happen. Trust me :)
    static constexpr M endl             = 14; ///< end of line code, input format
    static constexpr M enter            = 15; ///< enter bloc or indent
    static constexpr M leave            = 16; ///< leaving bloc or unindent
    static constexpr M begin            = 17; ///< begin (sel)section or indent
    static constexpr M end              = 18; ///< end (logger: end of entry accumulators, so do commit); end of (sel)section, attribute ( auto- color::Reset ) and unindent
    static constexpr M notexist         = 19; ///< does not exist
    static constexpr M exist            = 20; ///< does already exist
    static constexpr M unexpected       = 21; ///< unexpected
    static constexpr M expected         = 22; ///< expected
    static constexpr M blocked          = 23; ///< thread trying to lock a mutex has failed because the mutex is already locked in another thread...
    static constexpr M locked           = 24; ///< thread trying to lock a mutex has became the owner of the lock.
    static constexpr M ci               = 25; ///< Commit action;
    static constexpr M function         = 1000;
    static constexpr M file             = 1001;
    static constexpr M line             = 1002;
    static constexpr M stamp            = 4000; ///< fully detailed timestamp
    static constexpr M hour             = 4001;
    static constexpr M minute           = 4002;
    static constexpr M seconds          = 4003;
    static constexpr M weekday          = 4004;





    using attribute_data = std::pair<Icon::Type, textattr::pair>;

    using type_attribute_pair = std::pair<code::type , code::attribute_data>;
    using code_attribute_pair = std::pair<code::M , code::attribute_data>;



    static std::string text(code::type ty);
    static std::string text(code::M co);

    using type_attribute_table = std::vector<code::type_attribute_pair>;
    using code_attribute_table = std::vector<code::code_attribute_pair>;

};


class  TUXLIB diagnostic : public object
{
    code::type_attribute_table types_data;
    code::code_attribute_table codes_data;
    friend class application;
    int _indent = 0; ///< indent shift; default to 4 spaces.

public:


    struct  TUXLIB log_entry
    {
        using component_list = std::vector<std::string>;
        using memory = std::vector<diagnostic::log_entry>;
        int indent = 0; ///< Current indentation [state] -  not used yet;
        textattr::format encode = textattr::format::ansi256;

        source_location loc = {};
        code::type  typ = code::test;
        code::M     cod = code::ok;

        stracc str_acc;

        log_entry();
        ~log_entry();

        explicit log_entry(code::type ty, source_location&& _l={});
        log_entry(log_entry&& le) noexcept;
        log_entry(const log_entry& le);


        diagnostic::log_entry& operator=(log_entry&& le) noexcept;
        diagnostic::log_entry& operator=(const log_entry& le);

        diagnostic::log_entry& operator << (const std::string& val_);
        diagnostic::log_entry& operator << (code::M c);
        diagnostic::log_entry& operator << (code::type ty);
        diagnostic::log_entry& operator << (textattr::pair&& p );
        diagnostic::log_entry& operator << (color::type c);
        diagnostic::log_entry& operator << (point<> pt);
        diagnostic::log_entry& operator << (point<float> pt);
        diagnostic::log_entry& operator << (dim<> dxy);
        diagnostic::log_entry& operator << (dim<float> dxy);
        diagnostic::log_entry& operator << (rectangle<> r);
        diagnostic::log_entry& operator << (rectangle<float> r);
        diagnostic::log_entry& operator << (const stracc& accstr);

        template<typename T> diagnostic::log_entry& operator << (const T& val_)
        {
            str_acc << val_;
            return *this;
        }

        void clear()
        {
            str_acc.clear();
        }

    };


private:
    log_entry::memory log_entries; ///< in-memory logs output.

public:
    diagnostic();
    diagnostic(object* parent, const std::string& id);

    ~diagnostic() override;

    code::attribute_data & operator[](code::type ty);
    code::attribute_data & operator[](code::M ty);

    code::M init();
    static int indentation();
    static diagnostic::log_entry& error(source_location&& loc_ = {});
    static diagnostic::log_entry& warning (source_location&& loc = {});
    static diagnostic::log_entry& fatal(source_location&& loc = {});
    static diagnostic::log_entry& except(source_location&& loc = {});
    static diagnostic::log_entry& message(source_location&& loc = {});
    static diagnostic::log_entry& output(source_location&& loc = {});
    static diagnostic::log_entry& debug(source_location&& loc = {});
    static diagnostic::log_entry& info(source_location&& loc = {});
    static diagnostic::log_entry& comment(source_location&& loc = {});
    static diagnostic::log_entry& syntax(source_location&& loc = {});
    static diagnostic::log_entry& status(source_location&& loc = {});
    static diagnostic::log_entry& test(source_location&& loc = {});
    static diagnostic::log_entry& tail();

    static std::string cc(const diagnostic::log_entry& loge);
    static void clear(std::function<void(diagnostic::log_entry& le)> fn);
    static void indent();
    static void unindent();
    static bool empty();

};

} // oo
