#include <gtest/gtest.h>
#include <tlux/application.h>

class stracc_test : public ::testing::Test
{


protected:

    tux::stracc str;
    tux::application app;
    void SetUp() override;
    void TearDown() override;
};



void stracc_test::SetUp()
{
    app.init(0,nullptr);

}

void stracc_test::TearDown()
{

}

TEST_F(stracc_test, assign)
{
    str = "Jello, cruel world.\n";
    EXPECT_EQ(str(), "Jello, cruel world.\n");
}


TEST_F(stracc_test, app_and_diagnostic)
{
    using tux::diagnostic;
    using tux::code;

    diagnostic::test(sfnll) << code::enter << " testing..."<< code::endl;
    diagnostic::clear([](diagnostic::log_entry& e) {
        std::cerr << diagnostic::cc(e);
        //SCOPED_TRACE(diagnostic::cc(e).c_str());
    });
    EXPECT_TRUE(diagnostic::empty()) << " truelly true";
}
