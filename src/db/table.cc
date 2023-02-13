
#include<tlux/db/table.h>


namespace tux::db
{

#pragma region field
static field null_field = {};

field::field(object *atable, const std::string &aid, field::type atype):object(atable,aid),
_type(atype){}

field::field(const std::string &aid, field::type atype, uint8_t aconstrain):object(nullptr, aid)
{
    _type = atype;
    _attr = aconstrain;

}

field::field(field &&af) noexcept : object(af)
{
    _description = std::move(af._description);
    _attr = af._attr;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    _type = af._type;
}

field::field(const field &af):object(af)
{
    _type = af._type;
}

field &field::operator=(field &&af) noexcept
{
    object::operator=(af);
    _description = std::move(af._description);
    _attr = af._attr;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    _type = af._type;
    return *this;
}

void field::set_null(bool s)
{
    if(s)
        _attr |= Field::Null;
    else
        _attr &= ~Field::Null;
}

void field::set_foreign(bool s)
{
    if(s)
        _attr |= Field::Foreign;
    else
        _attr &= ~Field::Foreign;
}

field &field::operator=(const field &af)
{
    object::operator=(af);
    _description = af._description;
    _attr = af._attr;
    _len  = af._len;
    ref_table = af.ref_table;
    ref_field = af.ref_field;
    _type = af._type;
    return *this;
}

code::T field::attributes_text(stracc& qacc) const
{
    if(!_attr.Null)
        qacc << " not null";

    if(_attr.Foreign)
    {
        qacc << " REFERENCES " << ref_table->id() <<"(" << ref_field->id() << ")";
        return code::accepted;
    }
    else
    {
        if(_attr.Primary)
        {
            qacc << " PRIMARY KEY";
        }
    }
    return code::accepted;
}
/*!
 * @brief std::string operator serializes the column's descriptions for the create or alter pgsql table column.
 * @return std::string resulting SQL execute_query text line.
 */
code::T field::text(stracc& qacc) const
{
    qacc <<  id(); ///< Get the column name from this object::id().
    qacc << " ";
    return sql_type(qacc);
}

code::T field::sql_type(stracc& qacc) const
{
    stracc str;
    switch(_type)
    {
        case field::Binary:qacc<<  "bytea";break;
        case field::String:qacc << "VARCHAR(" << (_len ? _len : 50) << ")"; break;
        case field::Text:qacc << "text"; break;
        case field::Date: qacc <<"date"; break;
        case field::Stamp: qacc << "timestamp"; break;
        case field::Password: qacc << "varchar(120)"; break;
        //case field::Integer: qacc <<  "integer"; break;
        default: qacc << "integer"; break;
    }

    return attributes_text(qacc);
}



code::T field::set_foreign_key(field &aref_field)
{
    auto *tbl = aref_field.parent<table>();
    ref_field = &aref_field;
    ref_table = tbl;
    _attr.Foreign = 1;
    return code::accepted;
}

code::T field::set_unique(stracc::list fields)
{
//    field* f = nullptr;
//    for(auto const& name: fields)
//    {
//        auto i = query_child(name);
//        if(i==_children.end()) return code::rejected;

//        f = (*i)->to<field>();
//        if(f)
//            _uniques.push_back(f);
//    }
    return code::accepted;
}

field::field(const std::string &aid, field::type atype):object(nullptr,aid),
_type(atype){}



#pragma endregion field

//-------------------------------------------------------------------------------------------------------------------


#pragma region table

table::~table() {for(auto* obj: _children) delete obj;}

table::table(object *aparent, const std::string& aid):object(aparent, aid) {}

code::T table::text(stracc &query_acc)
{
    query_acc << "CREATE TABLE " << id() << "(";
    // Iterate fields and accumulate strings to str:
    for_each<field>([&query_acc](field& f, bool is_last_) -> bool {
        f.text(query_acc);
        if(!is_last_)
            query_acc <<", ";
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
        query_acc << ")";
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

field& table::operator[](const std::string& fid)
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
