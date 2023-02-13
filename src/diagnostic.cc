//
// Created by oldlonecoder on 1/27/23.
//

#include "tlux/diagnostic.h"
#include "tlux/application.h"
#include "tlux/stml/text.h"

//
// Created by oldlonecoder on 1/27/23.
//

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
// Get rid of rotten old obsolete include guard.
/*******************************************************************
 diagnostic implementation created on Friday, 27 of January 2023 at 7:20 PM by oldlonecoder
********************************************************************/

namespace tux {


std::string code::text(code::type ty)
{
    std::map<code::type, const char*> _ = {
        {error  , "error"   },
        {warning, "warning" },
        {fatal  , "fatal"   },
        {except , "except"  },
        {message, "message" },
        {output , "output"  },
        {debug  , "debug"   },
        {info   , "info"    },
        {comment, "comment" },
        {syntax , "syntax"  },
        {status , "status"  },
        {test   , "test"    }
    };
    return _[ty];
}

std::string code::text(code::T co)
{
    std::map<code::T, const char*> _ = {
        {code::ok,             "ok"},
        {code::accepted,       "accepted"},
        {code::success,        "success "},
        {code::rejected,       "rejected"},
        {code::failed,         "failed"},
        {code::empty,          "empty"},
        {code::full,           "full"},
        {code::notempty,       "notempty"},
        {code::implemented,    "implemented"},
        {code::notimplemented, "notimplemented"},
        {code::untested,       "untested"},
        {code::eof,            "eof"},
        {code::eos,            "eos"},
        {code::null_ptr,       "null_ptr"},
        {code::endl,           "end line"},
        {code::enter,          "enter"},
        {code::leave,          "leave"},
        {code::begin,          "begin"},
        {code::end,            "end"},
        {code::notexist,       "notexist"},
        {code::exist,          "exist"},
        {code::unexpected,     "unexpected"},
        {code::expected,       "expected"},
        {code::blocked,        "blocked"},
        {code::locked,         "locked"},
        {code::function,       "function"},
        {code::file,           "file"},
        {code::line,           "line"},
        {code::stamp,          "stamp"},
        {code::hour,           "hour"},
        {code::minute,         "minute"},
        {code::seconds,        "seconds"},
        {code::weekday,        "weekday"},
    };
    return _[co];
}

diagnostic::log_entry::log_entry(code::type ty, source_location &&_l):loc(_l), typ(ty)
{
    indent = diagnostic::indentation();
}

diagnostic::log_entry::log_entry(diagnostic::log_entry &&le)
 noexcept {
    loc = le.loc;
    typ = le.typ;
    cod = le.cod;
    indent = le.indent;
    str_acc = le.str_acc;
}

diagnostic::log_entry::log_entry(const diagnostic::log_entry &le)
{
    loc = le.loc;
    typ = le.typ;
    cod = le.cod;
    indent = le.indent;
    str_acc = le.str_acc;

}

diagnostic::log_entry &diagnostic::log_entry::operator=(diagnostic::log_entry &&le) noexcept
{
    loc = le.loc;
    typ = le.typ;
    cod = le.cod;
    indent = le.indent;
    str_acc = le.str_acc;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator=(const diagnostic::log_entry &le)
{
    loc = le.loc;
    typ = le.typ;
    cod = le.cod;
    indent = le.indent;
    str_acc = le.str_acc;
    return *this;
}

diagnostic::log_entry::log_entry()
{
    indent = diagnostic::indentation();
}

diagnostic::log_entry::~log_entry()
{
    str_acc.clear();
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(const std::string &val_)
{
    str_acc << val_;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(code::T c)
{
    switch(c)
    {
        case code::begin:
            diagnostic::indent();
            break;
        case code::end:
            diagnostic::unindent();
            break;
        case code::endl:
        {
            if(encode == textattr::format::ansi256)
                str_acc << '\n';
            else
                if(encode == textattr::format::html)
                    str_acc << "<br />";
            break;
        }
        default:
            {
                // Get the color pair from the application mem codes DB:
                auto [ic,p] = application::diagnostic_instance()[c];
                str_acc << color::White << '[' << p.bg << p.fg << Icon::Data[ic] << code::text(c) << color::White << ']';
            }
            break;
    }

    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(code::type ty)
{
    typ = ty;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(textattr::pair &&p)
{
    str_acc << p();
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(color::type c)
{
    str_acc << c;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(point<> pt)
{
    str_acc << (std::string)pt;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(point<float> pt)
{
    str_acc << (std::string)pt;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(dim<> dxy)
{
    str_acc << (std::string)dxy;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(dim<float> dxy)
{
    str_acc << (std::string)dxy;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(rectangle<> r)
{
    str_acc << (std::string)r;
    return *this;
}

diagnostic::log_entry &diagnostic::log_entry::operator<<(rectangle<float> r)
{
    str_acc << (std::string)r;
    return *this;
}

diagnostic::log_entry& diagnostic::log_entry::operator << (const stracc& accstr)
{
    str_acc << accstr;
    return *this;
}

diagnostic::diagnostic():object()
{

}

diagnostic::diagnostic(object *parent, const std::string &id) : object(parent, id)
{

}

diagnostic::~diagnostic()
{
    log_entries.clear();
}

code::attribute_data &diagnostic::operator[](code::type ty)
{
    return types_data[ty].second;
}

code::attribute_data &diagnostic::operator[](code::T cd)
{
    return codes_data[cd].second;
}

code::T diagnostic::init()
{
    return 0;
}

int diagnostic::indentation()
{
    return application::diagnostic_instance()._indent;
}

diagnostic::log_entry &diagnostic::error(source_location &&loc_)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::error,std::move(loc_));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::warning(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::warning,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::fatal(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::fatal,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::except(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::except,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::message(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::message,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::output(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::output,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::debug(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::debug,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::info(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::info,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::comment(source_location &&loc)
{

    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::comment,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::syntax(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::syntax,std::move(loc));
    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::status(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::status,std::move(loc));

    return d.log_entries.back();
}

diagnostic::log_entry &diagnostic::test(source_location &&loc)
{
    auto& d = application::diagnostic_instance();
    if(!d.log_entries.empty())
        d.log_entries.back() << code::endl;

    d.log_entries.emplace_back(code::test,std::move(loc));
    return d.log_entries.back();
}

std::string diagnostic::cc(const diagnostic::log_entry &a_entry)
{
    auto& d = application::diagnostic_instance();
    auto [icon, p] = d[a_entry.typ];
    stracc str;
    stracc ind = "";
    ///@todo Setup indentation length ( in application config for ex. )
    if(a_entry.indent > 0)
        ind.fill(0x20, a_entry.indent*4);

    str << ind;
    if(a_entry.typ != code::output)
    {
        //Get the color pair from the application DB for the given code::type:
        str << color::White << '[' << p <<  Icon::Data[icon] <<  code::text(a_entry.typ) << color::White << "] ";
    }
    else
    {
        // code == output:
        str << ind;
    }
    auto const& loc = a_entry.loc;

    if(loc)
    {
        if(!loc.filename.empty())
        {
            text_tokenizer txt(loc.filename,"/",false);
            text_tokenizer::token_t::list l;
            auto cnt = txt(l,"/",false);
            ///@todo check cnt....

            str << color::White << '['; str << color::CadetBlue2 << Icon::Folder;
            if(cnt>1)
                str << ".../";

            str << l.back()() << color::White << ']';
        }
        if(loc.line > 0)
        {
            str << color::White << '|' << color::Lime << Icon::Baseline << loc.line << color::White << "|";
        }
        if(!loc.function_sig.empty())
        {
            str << color::SteelBlue1 << Icon::Function << loc.function_sig << color::White << ": ";
        }
        //str << (loge.encode == textattr::format::ansi256 ? "\n": "<br />") << ind;
    }

    //auto [icon_code, code_pair] = d[loge.cod];
    //str  << icon_code << code_pair.bg << code_pair.fg << code::text(loge.cod) << ' ' << color::DeepSkyBlue7 << loge.str_acc;
    // ...
    str << a_entry.str_acc;
    return str();
}

void diagnostic::clear(std::function<void(diagnostic::log_entry &le)> fn)
{
    auto& d = application::diagnostic_instance();
    for (auto& e : d.log_entries)
    {
        if(fn)
            fn(e);
        else
            std::cout << diagnostic::cc(e) << '\n';
        e.clear();
    }
    d.log_entries.clear();
}

void diagnostic::indent()
{
    ++application::diagnostic_instance()._indent;
}

void diagnostic::unindent()
{
    auto& d = application::diagnostic_instance();
    --d._indent;
    if(d._indent < 0 ) d._indent = 0;
}


diagnostic::log_entry &diagnostic::tail()
{
    auto& d = application::diagnostic_instance();
    return d.log_entries.back();
}
} // tux
