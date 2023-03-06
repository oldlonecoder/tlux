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
#include <tlux/stracc.h>

namespace tux {

class  TUXLIB object
{


public:
    using list = std::vector<object*>;
    using iterator = object::list::iterator;

    object() {}
    object(object* aparent, const std::string& aobj_name);
    object(object&& obj);
    object(const object& obj);

    virtual ~object();

    object& operator = (object&& obj) noexcept = default;
    object& operator = (const object& obj) = default;

    object::iterator query_child(object* obj);
    object::iterator query_child(const std::string& aid);
    template<typename T> T* to() { return dynamic_cast<T*>(this); }
    template<typename T> T* as() { return dynamic_cast<T*>(this); }
    template<typename T> T* parent() {
        if(!_parent) return nullptr;
        return dynamic_cast<T*>(_parent);
    }
    template<typename T> T* first_parent_of() {
        if(!_parent) return nullptr;
        object* o = _parent;
        while(o)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) return tp;
            o = o->_parent;
        }
        return nullptr;
    }

    template<typename T> object::list children_of()
    {
        object::list l;
        for(auto* o : _children)
        {
            T* tp = dynamic_cast<T*>(o);
            if(tp) l.push_back(tp);
        }
        return l;
    }

    [[nodiscard]] std::string id() const { return _id; }

    template<typename T> void for_each(std::function<bool(T& obj, bool alast)> fn)
    {
        auto it = _children.begin();
        while(it != _children.end())
        {
            bool l = (it + 1) == _children.end();
            T* ty = (*it)->to<T>();
            if(ty)
                if (fn)
                    fn(*ty,l);
            ++it;
        }
    }
    void append_child(object* o);
protected:
    std::string _id;
    object::list _children = {};
    object* _parent = nullptr;

};

} // tux

