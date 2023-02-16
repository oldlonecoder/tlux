
#include<tlux/db/table.h>




namespace tux::db
{
static field null_field = {};

#pragma region table

table::~table() {for(auto* obj: _children) delete obj;}

table::table(object *aparent, const std::string& aid):object(aparent, aid) {}

code::T table::text(stracc &query_acc)
{
    query_acc << "CREATE TABLE " << id() << "(\n";
    // Iterate fields and accumulate strings to str:
    for_each<field>([&query_acc](field& f, bool is_last_) -> bool {
        f.text(query_acc);
        if(!is_last_)
            query_acc <<",";
        query_acc << '\n';
        return true;
    });
    if(!_uniques.empty())
    {
        query_acc << ", UNIQUE(";
        for(auto fi = _uniques.begin(); fi != _uniques.end(); fi++)
        {
            query_acc << (*fi)->id();
            auto e = fi;
            e ++;
            if(e != _uniques.end()) query_acc << ',';
        }
        query_acc << ")\n";
    }
    query_acc << ");";
    return 0;
}

field *table::add_field(field &&af)
{
    auto * f = new db::field(af);
    append_child(f);
    return f;
}

table &table::operator<<(field &&f)
{
    add_field(std::move(f));
    return *this;
}

field &table::operator[](const std::string &fid)
{
    auto it = query_child(fid);
    if(it != _children.end())
    {
        auto* obj = *it;
        field* f = obj->to<field>();
        if(f) return *f;
    }
    throw diagnostic::except(sfnll) << code::notexist << " field named '" << color::Yellow << fid << color::White << "' in register '" << color::Yellow << id() << color::White << "'...";
}


code::T table::add_foreign(const std::string &fname, const std::string &ftablename, const std::string &fcolname)
{
    auto& f = query_field_by_name(fname);
    if(!f)
    {
        //diagnostic::error() << code::notexist << " field ' " << color::Yellow << fname << color::While << "'";
        return code::notexist;
    }
    return code::accepted;
}

field &table::query_field_by_name(const std::string &f_id)
{
    for(auto* f : _children)
        if(f->id() == f_id)
            return *(f->as<field>());

    return null_field;
}

field *table::field_by_id(const std::string &fname)
{
        for(auto* f : _children)
            if(f->id() == fname)
                return (f->as<field>());

    return &null_field;
}

code::T table::set_unique(stracc::list fields)
{
    field* f = nullptr;
    for(auto const& name: fields)
    {
        auto i = query_child(name);
        if(i==_children.end()) return code::rejected;

        f = (*i)->to<field>();
        if(f)
            _uniques.push_back(f);
    }
    return code::accepted;
}


#pragma endregion table



} // tux::db
