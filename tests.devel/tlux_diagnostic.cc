#include <gtest/gtest.h>
#include <tuxls/application.h>

class tuxls_diagnostic : public ::testing::Test
{


protected:

    tux::stracc str;
    tux::application app;
    void SetUp() override;
    void TearDown() override;
};



void tuxls_diagnostic::SetUp()
{
    app.init(0,nullptr);

}

void tuxls_diagnostic::TearDown()
{

}

TEST_F(tuxls_diagnostic, diagnostic_instance_and_clear)
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
