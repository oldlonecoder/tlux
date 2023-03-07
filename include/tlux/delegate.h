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
 *   MERCHANTABILITY or FITNESS FOR A PAcode::MICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#pragma once

#include <tlux/diagnostic.h>
#include <functional>

namespace tux
{

//template<typename T> delegator;

template <typename... Args> class delegator {



public:

    using delegate = std::function<code::M(Args...)>;

    delegator() = default;
    ~delegator() = default;

    // Copy constructor and assignment create a new signal.
    delegator(delegator const& /*unused*/) {}

    delegator& operator=(delegator const& other) {
        if (this != &other) {
            disconnect_all();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    delegator(delegator&& other) noexcept :
        _slots(std::move(other._slots)),
        _current_id(other._current_id) {}

    delegator& operator=(delegator&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _current_id = other._current_id;
        }

        return *this;
    }

    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<code::M(Args...)>  slot)  {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }

    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<code::M(Args...)> const& slot) const {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }


    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T> int connect_member(T* inst, code::M(T::* func)(Args...)) {
        return connect([=](Args... args) {
            (inst->*func)(args...);
            });
    }

    // Convenience method to connect a const member function
    // of an object to this delegator.
    template <typename T>
    int connect_member(T* inst, code::M(T::* func)(Args...) const) {
        return connect([=](Args... args) {
            (inst->*func)(args...);
            });
    }

    // Disconnects a previously connected function.
    void disconnect(int id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _slots.clear();
    }

    // Calls all connected functions.
    code::M operator()(Args... p) {
        code::M R{};
        for (auto& [i, fn] : _slots) R = fn(p...);
        return R;
    }

    //// Calls all connected functions except for one.
    //code::M emit_for_all_but_one(int excludedConnectionID, Args... p) {
    //    code::M R{};
    //    for (auto &[i,fn] : _slots) {
    //        if (i != excludedConnectionID) {
    //            fn(p...);
    //        }
    //    }
    //}

    //// Calls only one connected function.
    //code::M emit_for(int connectionID, Args... p) {
    //    auto const& it = _slots.find(connectionID);
    //    if (it != _slots.end()) {
    //        it->second(p...);
    //    }
    //}

private:
    mutable std::map<int, std::function<code::M(Args...)>> _slots;
    mutable int  _current_id{ 0 };
};





}


