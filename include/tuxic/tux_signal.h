/***************************************************************************
*   Copyright (C) 2005/2023 by Serge Lussier                              *
*   2005: (bRTzel@tuxweb.homelinux.net)                                  *
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

//
// Created by oldlonecoder on 1/11/23.
//



#pragma once


#include <tlux/diagnostic.h>

// ------- copied from, intellectual proprietary to:
// https://schneegans.github.io/tutorials/2015/09/20/signal-slot :
#include <functional>
#include <map>

namespace tux
{


/*!
    @brief Thread non-safe signal-slots.

    @note typename RT must be default-constructible.

*/
template<typename RT, typename ...Args> class tux_signal
{
    using slots = std::vector<std::function<RT(Args...)>>;
    using iterator = typename tux_signal::slots::iterator;

    std::string _id;
    slots _slots;

public:
    using slot = std::function<RT(Args...)>;

    tux_signal() = default;
    using accumulator = std::vector<RT>;

    tux_signal(std::string&& aid,typename tux_signal::accumulator & acc):
    _id(std::move(aid)),
    _accumulator(&acc){}

    tux_signal(tux_signal&& other) noexcept:
    _id(std::move(other._id)),
    _slots(std::move(other._slots)),
    _accumulator(std::move(other._accumulator))
    {}

    tux_signal& operator=(tux_signal&& other) noexcept {
        if (this != &other)
        {
            _slots      = std::move(other._slots);
            _id         = std::move(other._id);
            _accumulator= std::move(other._accumulator);
        }
        return *this;
    }

    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    typename slots::iterator connect(std::function<RT(Args...)>  slot) {
        _slots.push_back(slot);
        return --_slots.end();
    }

    template <typename T> typename slots::iterator connect(T *inst, RT(T::*func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Convenience method to connect a const member function
    // of an object to this signal.
    template <typename T> typename slots::iterator connect(T *inst, RT (T::*func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
        });
    }

    // Disconnects a previously connected function.
    void disconnect(typename tux_signal::slots::iterator c) const {
        //----------------------------------------------------
        // This addition is slow'ing the solution but it is mandatory:
        if(std::find(_slots.begin(),_slots.end(), *c) == _slots.end())
            return;
        //----------------------------------------------------
        _slots.erase(c);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _slots.clear();
    }

    RT operator()(Args...args)
    {
        RT R;
        try
        {
            for (auto const &fn: _slots)
            {
                R = fn(args...);
                if(_accumulator) _accumulator->push_back(R);
            }
            return R;
        }
        catch(code::T interrupt_code){
            return {};
        }
        return {};
    }

private:
    typename tux_signal::accumulator* _accumulator = nullptr;
};

}
