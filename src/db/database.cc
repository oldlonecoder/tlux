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
#include<tlux/db/database.h>
#include <unistd.h>
#include<tlux/diagnostic.h>


namespace tux::db
{
int database::sqlite3_callback(void *aobj, int argc, char **argv, char **azColName)
{
    object* obj = reinterpret_cast<object*>(aobj);
    database* this_db = obj->to<database>();
    if(this_db)
    {
        diagnostic::output() << code::begin << " Db '" << this_db->id() << "' query result:";
        for(int i =0; i < argc; i++)
        {
            diagnostic::output() << color::White << "Field[" << color::Yellow << azColName[i] << color::White << "] = '" << color::Lime << (argv[i] ? argv[i] : "NULL") << color::White << "';";
        }
        diagnostic::debug() << code::end << " Now teacc_signal the row...";
        int r = data_row_in(argc,argv,azColName);

        return 0;
    }
    diagnostic::error(sfnll) << "callback obj has no address! - data ignored.";
    return code::failed;
}

database::database(object *aparent, const string &ais, const std::string& aext) : object(aparent, ais),_ext(aext) {}

database::~database()
{
    for(auto* obj: _children) delete obj;
}

code::T database::open()
{
    stracc str = id();
    str << _ext;
    //diagnostic::debug(sfnl) << code::begin << "file:" << color::Yellow << str;
#ifdef  _WIN32
    if (!PathFileExistsA(str().c_str()))
        return
        {
            (utils::notification::push(), " sqlite3 open db error(", _dbname, ") - no such database file.\n")
        };
#else
    int ok = access(str().c_str(),F_OK) == 0; // Just check if the db file exists.
    if(!ok)
    {
        // That's it. Intercept "file does not exist" to prevent systematic creation by sqlite lib.
        // We use explicit database::create method to create it.
        //diagnostic::error(sfnl) << code::end << "error opening database '" << id() << "' - " << code::endl << strerror(errno) << code::end;
        return code::failed;
    }
#endif
    //diagnostic::output() << code::ok;
    int res = sqlite3_open(str().c_str(), &_file);
    if (res != SQLITE_OK)
    {
        throw diagnostic::error(sfnll) << code::failed << " database connect error(" << res << ") - " <<  sqlite3_errmsg(_file);
        //return code::failed;
    }
    //diagnostic::error(sfnl) << code::end << code::ok;
    return code::success;
}

code::T database::create()
{
    stracc str;
    str << id() << '.' << _ext;
    int ok = access(str().c_str(),F_OK) == 0;
    if(ok)
        throw diagnostic::except(sfnll) <<  "database " << id() << " already " << code::exist;

    int res = sqlite3_open(str.str().c_str(),&_file);
    if(res != 0)
        throw diagnostic::except(sfnl) << code::failed << " -  Error creating database '" << id() << ": " << strerror(errno);

    str.clear();
    str << "PRAGMA foreign_keys = ON;";
    // Loop table children
    for_each<table>([&str](table& e, bool last_table) -> bool {
         code::T r =  e.text(str);
         if(r == code::ok)
             str << "\n";
         return r;
    });

    diagnostic::test(sfnll) << code::begin << "Before creating db schema:";
    diagnostic::output() << str;
    diagnostic::test() << code::end << code::ok << "Now sending the database creation query to sqlite:";// << code::endl;

    auto r = execute_query(str());



    return code::accepted;
}




code::T database::close()
{
    int r = sqlite3_close(_file);
    if(r != 0)
    {
        std::cerr << " cannot close db'" << id() << "...";
    }
    _file = nullptr;
    return 0;
}

code::T database::execute_query(const string &aquery_string)
{
    char* errmsg;
    int ok = sqlite3_exec(_file,aquery_string.c_str(),&database::sqlite3_callback, this, &errmsg);
    if(ok != SQLITE_OK)
    {
        diagnostic::error() << code::failed << color::Red4 << sqlite3_errmsg(_file);
        sqlite3_free(errmsg);
        return code::rejected;
    }

    return 0;
}

table& database::operator [](const std::string &eid)
{
    auto it = query_child(eid);
    if(it != _children.end())
    {
        auto* obj = *it;
        table* e = obj->to<table>();
        if(e) return *e;
    }
    throw diagnostic::except(sfnll) << code::notexist << " register '"
                                    << color::Yellow << eid
                                    << color::White << "' in registry '"
                                    << color::Yellow << id()
                                    << color::White << "'...";
}

table *database::add_table(const string &ename)
{
    auto *e = new table(this, ename);
    return e;
}


} // namespace tux::db
