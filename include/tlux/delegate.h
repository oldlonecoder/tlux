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

#include <tlux/expect.h>
#include <functional>

 // ------- copied from, intellectual proprietary to:
 // https://schneegans.github.io/tutorials/2015/09/20/signal-slot :



namespace tux
{


template <typename... Args> class delegator {

public:
    std::string _id{"anon delegator"};
    using slot = std::function<expect<>(Args...)>;
    using list = std::vector<typename delegator::slot>;
    using iterator = typename delegator::list::iterator;
    delegator() = default;
    ~delegator() = default;
    delegator(const std::string& id_) : _id(id_) {}

    // Copy constructor and assignment create a new delegator.
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
        _id(std::move(other._id)),
        _current_id(other._current_id) {}

    delegator& operator=(delegator&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _id = std::move(other._id);
            _current_id = other._current_id;
        }

        return *this;
    }


    // Connects a std::function to the delegator. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<expect<>(Args...)> const& slot) const {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T>
    int connect_member(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T>
    int connect(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }


    // Convenience method to connect a const member function
    // of an object to this delegator.
    template <typename T>
    int connect(T* inst, expect<>(T::* func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
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
    expect<> emit(Args... p) {
        expect<> R;
        for (auto const& [f, fn] : _slots) {
            R = fn(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }
    // Calls all connected functions.
    expect<> operator()(Args... p) {
        expect<> R;
        for (auto const& [f, fn] : _slots) {
            R = fn(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    expect<> emit_for_all_but_one(int excludedConnectionID, Args... p) {
        expect <> R;
        for (auto const& it : _slots) {
            if (it.first != excludedConnectionID) {
                R = it.second(p...);
                if (!R || *R == code::rejected) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    expect<> emit_for(int connectionID, Args... p) {
        expect<> R;
        auto const& it = _slots.find(connectionID);
        if (it != _slots.end()) {
            R = it->second(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }

private:
    mutable std::map<int, std::function<expect<>(Args...)>> _slots;
    mutable typename delegator::list _delegates;
    mutable int _current_id{ 0 };



};


}