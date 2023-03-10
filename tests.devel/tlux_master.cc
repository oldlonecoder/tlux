#include <gtest/gtest.h>
#include <tuxls/application.h>

class master_test : public ::testing::Test
{


protected:

    tux::stracc str;
    tux::application app;
    void SetUp() override;
    void TearDown() override;
};



void master_test::SetUp()
{
    app.init(0,nullptr);

}

void master_test::TearDown()
{

}

TEST_F(master_test, assign_stracc)
{
    str = "Jello, cruel world.\n";
    EXPECT_EQ(str(), "Jello, cruel world.\n");
}


TEST_F(master_test, app_and_diagnostic)
{
    using tux::diagnostic;
    using tux::code;

    diagnostic::test(sfnll) << code::enter << " testing..."<< code::endl;
    diagnostic::clear([](diagnostic::log_entry& e) {
        std::cout << diagnostic::cc(e);
        //SCOPED_TRACE(diagnostic::cc(e).c_str());
    });
    EXPECT_TRUE(diagnostic::empty()) << " truelly true";
}
