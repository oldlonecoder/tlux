#include <gtest/gtest.h>
#include <tlux/application.h>
#include <tlux/db/database.h>
#include <tlux/db/field.h>
#include <tlux/db/table.h>

#include <tlux/diagnostic.h>
class tlux_database : public ::testing::Test
{


protected:

    tux::stracc str;
    tux::application app;
    void SetUp() override;
    void TearDown() override;
    tux::db::database *db = nullptr;
};



void tlux_database::SetUp()
{
    app.init(0,nullptr);

}

void tlux_database::TearDown()
{

    using tux::diagnostic;
    using tux::code;

    tux::diagnostic::info(sfnll) << ":" << code::endl;
    tux::diagnostic::clear([](tux::diagnostic::log_entry& e) {
        std::cout << tux::diagnostic::cc(e);
    });
    delete db;
}

// TEST_F(tlux_database, database_check_create)
// {
//     using tux::db::database;
//     using tux::diagnostic;
//     using tux::code;
//     std::cout << "\n\n";
//     diagnostic::test(sfnl) << code::begin << " testing tux::db::database::create:";
//     diagnostic::output() << "new constructor:";
//     db = new tux::db::database(nullptr, "diagnostic","db");
//     try{
//         diagnostic::output() << code::ok << " now invoke create:";
//         auto r = db->create();
//         EXPECT_EQ(code::accepted, r);
//     }
//     catch(tux::diagnostic::log_entry& e)
//     {
//         ;
//     }
//     diagnostic::info() << code::end << " check create done.";
//      EXPECT_TRUE(!diagnostic::empty()) << "diagnostic entries empty?";
// }

// TEST_F(tlux_database, create_table)
// {
//     std::cout << "\n\n";
//     using tux::db::database;
//     using tux::diagnostic;
//     using tux::code;

//     diagnostic::test(sfnl) << code::begin << " testing tux::db::database::add_table:";
//     diagnostic::output() << "new constructor:";
//     db = new tux::db::database(nullptr, "diagnostic","db");
//     try{
//         diagnostic::output() << code::ok << " now invoke add_table:";
//         auto tbl = db->add_table("log_entry");
//         EXPECT_TRUE(tbl != nullptr);
//         diagnostic::info() << "table name :'" << tux::color::Yellow << tbl->id() << tux::color::Reset << "'";
//     }
//     catch(tux::diagnostic::log_entry& e)
//     {
//         ;
//     }
//     diagnostic::info() << code::end << " check create done.";
//      EXPECT_TRUE(!diagnostic::empty()) << "diagnostic entries empty?";
// }

// TEST_F(tlux_database, create_field)
// {
//     std::cout << "\n\n";
//     using tux::db::database;
//     using tux::diagnostic;
//     using tux::code;

//     diagnostic::test(sfnl) << code::begin << " testing tux::db::database::add_table:";
//     diagnostic::output() << "new constructor:";
//     db = new tux::db::database(nullptr, "diagnostic","db");
//     try{
//         diagnostic::output() << code::ok << " now invoke add_table:";
//         auto tbl = db->add_table("log_entry");
//         EXPECT_TRUE(tbl != nullptr);
//         diagnostic::info() << "table name :'" << tux::color::Yellow << tbl->id() << tux::color::Reset << "' - add field :";
//         tbl->add_field({"id", tux::db::field::Integer, tux::db::field::Primary});
//         EXPECT_TRUE(tbl->field_by_id("id") != nullptr);
//         tbl->add_field({"type", tux::db::field::Integer,tux::db::field::Null});
//         EXPECT_TRUE(tbl->field_by_id("type") != nullptr);
//         tbl->add_field({"code", tux::db::field::Integer,tux::db::field::Null});
//         EXPECT_TRUE(tbl->field_by_id("code") != nullptr);
//         tbl->add_field({"content", tux::db::field::Text,tux::db::field::Null});
//         EXPECT_TRUE(tbl->field_by_id("content") != nullptr);
//         tux::stracc txt;
//         auto r = tbl->text(txt);
//         EXPECT_TRUE(!txt().empty());
//         diagnostic::info() << "table description:" << code::endl << txt;

//     }
//     catch(tux::diagnostic::log_entry& e)
//     {
//         ;
//     }
//     diagnostic::info() << code::end << " check create done.";
//      EXPECT_TRUE(!diagnostic::empty()) << "diagnostic entries empty?";
// }

TEST_F(tlux_database, database_do_create)
{
    std::cout << "\n\n";
    using tux::db::database;
    using tux::diagnostic;
    using tux::code;

    diagnostic::test(sfnl) << code::begin << " create table 'log_entry'  and the fields:";
    diagnostic::output() << "new constructor:";
    db = new tux::db::database(nullptr, "diagnostic","db");
    try{
        diagnostic::output() << code::ok << " now invoke add_table:";
        auto tbl = db->add_table("log_entry");
        EXPECT_TRUE(tbl != nullptr);
        diagnostic::info() << "table name :'" << tux::color::Yellow << tbl->id() << tux::color::Reset << "' - add field :";
        tbl->add_field({"id", tux::db::field::Integer, tux::db::field::Primary});
        EXPECT_TRUE(tbl->field_by_id("id") != nullptr);
        tbl->add_field({"type", tux::db::field::Integer,tux::db::field::Null});
        EXPECT_TRUE(tbl->field_by_id("type") != nullptr);
        tbl->add_field({"code", tux::db::field::Integer,tux::db::field::Null});
        EXPECT_TRUE(tbl->field_by_id("code") != nullptr);
        tbl->add_field({"stamp", tux::db::field::Text,tux::db::field::Null});
        tbl->add_field({"content", tux::db::field::Text,tux::db::field::Null});
        EXPECT_TRUE(tbl->field_by_id("content") != nullptr);
        tux::stracc txt;
        auto r = tbl->text(txt);
        EXPECT_TRUE(!txt().empty());
        diagnostic::info() << "table description:";
        diagnostic::output() << txt;
        diagnostic::output() << "create the database for real:";
        r = db->create();
        EXPECT_EQ(r, code::accepted);
    }
    catch(tux::diagnostic::log_entry& e)
    {
        ;
    }
    diagnostic::info() << code::end << " check create done.";
     EXPECT_TRUE(!diagnostic::empty()) << "diagnostic entries empty?";

}