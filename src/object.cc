//
// Created by oldlonecoder on 1/19/23.
//

#include "tlux/object.h"

namespace tux {

//impl_rttn(object)


object::object(object *aparent, const std::string &aobj_name):
 _children({}),_id(aobj_name),_parent(aparent)
{
    if(_parent)
        _parent->append_child(this);
}

object::~object()
{

    _children.clear();
}

object::iterator object::query_child(object *obj)
{
    if(_children.empty()) return _children.end();
    auto ot = _children.begin();
    while(ot != _children.end())
    {
        if(*ot == obj) return ot;
        ++ot;
    }
    return _children.end();
}

object::iterator object::query_child(const std::string &aid)
{
    //std::cerr << __PRETTY_FUNCTION__ << " - " << id() << "::" << aid << ":\n";
    if(_children.empty())
    {
        std::cerr << " .. no children...\n";
        return _children.end();
    }
    auto ot = _children.begin();
    while(ot != _children.end())
    {
        if((*ot)->_id == aid) return ot;
        ot++;
    }
    std::cerr << attr<textattr::format::ansi256>::fg(color::White) << id() << " has no child identified by '"
              << attr<textattr::format::ansi256>::fg(color::Yellow) << aid
              <<attr<textattr::format::ansi256>::fg(color::White) << "'\n";

    return _children.end();
}

object::object(object &&obj): _id(std::move(obj._id)),
                              _children(std::move(obj._children)), _parent(obj._parent) {}

object::object(const object &obj): _id(obj._id),
                                   _children(obj._children), _parent(obj._parent){}

void object::append_child(object *o)
{
    if(query_child(o) != _children.end()) return;
    _children.push_back(o);
    o->_parent = this;
}


} // oo
