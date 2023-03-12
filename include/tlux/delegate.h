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


template <typename... Args> class signal {

public:
    std::string _id{ "anonymous signal" };
    using slot = std::function<expect<>(Args...)>;
    using list = std::vector<typename signal::slot>;
    using iterator = typename signal::list::iterator;
    using accumulator = std::vector<expect<>>;
    signal() = default;
    ~signal() = default;
    signal(const std::string& id_) : _id(id_) {}
    signal(const std::string& id_, typename signal::accumulator& acc) : _id(id_), _acc(&acc) {}

    // Copy constructor and assignment create a new delegator.
    signal(signal const& /*unused*/) {}

    signal& operator=(signal const& other) {
        if (this != &other) {
            disconnect_all();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    signal(signal&& other) noexcept :
        _slots(std::move(other._slots)),
        _id(std::move(other._id)),
        _acc(std::move(other._acc))
    {}

    signal& operator=(signal&& other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _id = std::move(other._id);
            _acc = std::move(other._acc);
        }

        return *this;
    }


    // Connects a std::function to the delegator. The returned
    // value can be used to disconnect the function again.
    typename signal::iterator connect(std::function<expect<>(Args...)> const& aslot) const {
        _slots.push_back(aslot);
        return --_slots.end();
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T> typename signal::iterator connect_member(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T> typename signal::iterator connect(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }


    // Convenience method to connect a const member function
    // of an object to this delegator.
    template <typename T> typename signal::iterator connect(T* inst, expect<>(T::* func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Disconnects a previously connected function.
    void disconnect(typename signal::iterator id) const {
        _slots.erase(id);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _slots.clear();
    }

    //// Calls all connected functions.
    //expect<> emit(Args... p) {
    //    expect<> R;
    //    for (auto const& fn : _delegates) {
    //        R = fn(p...);
    //        if (_acc) _acc->push_back(R);
    //        if (!R || *R == code::rejected) return R;
    //    }
    //    return R;
    //}
    // Calls all connected functions.
    expect<> operator()(Args... p) {
        expect<> R;
        for (auto const&fn : _slots) {
            R = fn(p...);
            if (_acc) _acc->push_back(R);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    expect<> emit_for_all_but_one(const std::string& id_, Args... p) {
        expect <> R;
        for (auto const& it : _slots) {
            if (it._id != id_) {
                R = it(p...);
                if (_acc) _acc->push_back(R);
                if (!R || *R == code::rejected) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    expect<> emit_for(typename signal::iterator id_, Args... p) {
        expect<> R;
        if (id_ != _slots.end()) {
            R = (*id_)(p...);
            if (_acc) _acc->push_back(R);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }

private:

    mutable typename signal::list _slots;
    mutable typename signal::accumulator* _acc{ nullptr };
};


}
