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
    std::string _id{ "anon delegator" };
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
        _delegates(std::move(other._delegates)),
        _id(std::move(other._id))
    {}

    delegator& operator=(delegator&& other) noexcept {
        if (this != &other) {
            _delegates = std::move(other._delegates);
            _id = std::move(other._id);
        }

        return *this;
    }


    // Connects a std::function to the delegator. The returned
    // value can be used to disconnect the function again.
    typename iterator connect(std::function<expect<>(Args...)> const& aslot) const {
        _delegates.push_back(aslot);
        return --_delegates.end();
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T> typename iterator connect_member(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Convenience method to connect a member function of an
    // object to this delegator.
    template <typename T> typename iterator connect(T* inst, expect<>(T::* func)(Args...)) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }


    // Convenience method to connect a const member function
    // of an object to this delegator.
    template <typename T> typename iterator connect(T* inst, expect<>(T::* func)(Args...) const) {
        return connect([=](Args... args) {
            return (inst->*func)(args...);
            });
    }

    // Disconnects a previously connected function.
    void disconnect(typename delegator::iterator id) const {
        _delegates.erase(id);
    }

    // Disconnects all previously connected functions.
    void disconnect_all() const {
        _delegates.clear();
    }

    // Calls all connected functions.
    expect<> emit(Args... p) {
        expect<> R;
        for (auto const& fn : _delegates) {
            R = fn(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }
    // Calls all connected functions.
    expect<> operator()(Args... p) {
        expect<> R;
        for (auto const&fn : _delegates) {
            R = fn(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }


    // Calls all connected functions except for one.
    expect<> emit_for_all_but_one(const std::string& id_, Args... p) {
        expect <> R;
        for (auto const& it : _delegates) {
            if (it._id != id_) {
                R = it(p...);
                if (!R || *R == code::rejected) return R;
            }
        }
        return R;
    }

    // Calls only one connected function.
    expect<> emit_for(typename delegator::iterator id_, Args... p) {
        expect<> R;
        if (id_ != _delegates.end()) {
            R = (*id_)(p...);
            if (!R || *R == code::rejected) return R;
        }
        return R;
    }

private:
    //mutable std::map<int, std::function<expect<>(Args...)>> _slots;
    mutable typename delegator::list _delegates;

};


}