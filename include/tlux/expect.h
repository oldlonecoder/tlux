/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier   *
 *   2005: (bretzel@teaccweb.homelinux.net)
 *   2023: lussier.serge@gmail.com
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
#include <tlux/diagnostic.h>


namespace tux

{


template<typename T = code::M>  class  expect final
{
    diagnostic::log_entry* fail = nullptr;
    T value;
    bool good = false;


public:
    expect()
    {
        good = false;
        fail = nullptr;
    }

    ~expect() {}

    expect(diagnostic::log_entry& msg)
    {
        good = false;
        fail = &msg;
    }

    expect(T value_) :value(value_), good(true) {}

    expect(expect&& r) noexcept
    {
        good = std::move(r.good);
        if (!good)
            fail = std::move(r.fail);
        else
            value = std::move(r.value);
    }

    expect(const expect& r)
    {
        good = r.good;
        if (!good)
            fail = r.fail;
        else
            value = r.value;

    }


    expect& operator=(expect&& r) noexcept
    {
        good = std::move(r.good);
        if (!good)
            fail = std::move(r.fail);
        else
            value = std::move(r.value);
        return *this;
    }

    expect& operator=(const expect& r)
    {
        good = r.good;
        if (!good)
            fail = r.fail;
        else
            value = r.value;
        return *this;
    }

    expect operator=(diagnostic::log_entry& r)
    {
        good = false;
        fail = &r;
        return *this;
    }

    operator bool() { return good; }

    diagnostic::log_entry& operator()()
    {
        if (good)
            throw diagnostic::except() << " invalid call to expect::operator '()', instance is in good state.";
        return *fail;
    }

    T operator *()
    {
        if (!good)
            throw diagnostic::except() << " invalid call to expect::operator '*', instance is in fail state.";
        return value;
    }

    auto operator->() -> T
    {
        if (!good)
            throw diagnostic::except() << " invalid call to expect::operator '->', instance is in fail state.";
        return value;
    }
};

}

