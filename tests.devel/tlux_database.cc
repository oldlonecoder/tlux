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


TEST_F(tlux_database, database_do_create)
{
    std::cout << "\n\n";
    using tux::db::database;
    using tux::db::table;
    using tux::db::field;
    using tux::diagnostic;
    using tux::code;
    code::T r;
    diagnostic::test(sfnl) << code::begin << " create table 'log_entry'  and the fields:";
    diagnostic::output() << "new constructor:";
    db = new tux::db::database(nullptr, "diagnostic","db");
    try{
        diagnostic::output() << code::ok << " now invoke add_table:";
        auto tbl = db->add_table("log_entry");
        ASSERT_TRUE(tbl != nullptr) << " db->add_table failed.";
        diagnostic::info() << "table name :'" <<  tux::color::Yellow << tbl->id() << tux::color::Reset << "' - add field :";

        tbl->add_field({"id", tux::db::field::Integer,  tux::db::field::Primary});
        tbl->add_field({"type", tux::db::field::Integer, tux::db::field::Null});
        tbl->add_field({"code", tux::db::field::Integer, tux::db::field::Null});
        tbl->add_field({"stamp", tux::db::field::Time, tux::db::field::Null|tux::db::field::DefaultStamp});
        tbl->add_field({"content", tux::db::field::Binary,tux::db::field::Null});

        EXPECT_TRUE(tbl->field_by_id("id") != nullptr);
        EXPECT_TRUE(tbl->field_by_id("type") != nullptr);
        EXPECT_TRUE(tbl->field_by_id("code") != nullptr);
        EXPECT_TRUE(tbl->field_by_id("content") != nullptr);

        tux::stracc txt;
        r = tbl->text(txt);
        ASSERT_TRUE(!txt().empty()) << "table schema is empty.";
        diagnostic::info() << "table description:" << code::end;
        diagnostic::output() << txt;
        diagnostic::output() << "create the database for real:";

        tbl = db->add_table("object");
        ASSERT_TRUE(tbl != nullptr) << " db->add_table failed.";
        diagnostic::info() << "table name :'" <<  tux::color::Yellow << tbl->id() << tux::color::Reset << "' - add field :";
        (*tbl)
            << tux::db::field{"id",tux::db::field::Integer, tux::db::field::Primary}
            << tux::db::field{"obj_name", tux::db::field::Text,tux::db::field::Unique};

        tbl = db->add_table("attributes");
        ASSERT_TRUE(tbl != nullptr) << " db->add_table failed.";
        diagnostic::info() << "table name :'" <<  tux::color::Yellow << tbl->id() << tux::color::Reset << "' - add field :";
        (*tbl)
            << field{"id", field::Integer,field::Primary}
            << field{"bg", field::Integer}
            << field{"fg", field::Integer};


        r = db->create();
        ASSERT_EQ(r, code::accepted) << " db->create did not throw, but failed";

    }
    catch(tux::diagnostic::log_entry& e)
    {
        ;
    }
    EXPECT_EQ(r, code::accepted) << "So creating the database failed.";
    diagnostic::info() << code::end << " check create done.";
     EXPECT_TRUE(!diagnostic::empty()) << "diagnostic entries empty?";

}
