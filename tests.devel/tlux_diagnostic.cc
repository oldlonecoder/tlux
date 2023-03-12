#include <gtest/gtest.h>
#include <tlux/application.h>

class tlux_diagnostic : public ::testing::Test
{


protected:

    tux::stracc str;
    tux::application app;
    void SetUp() override;
    void TearDown() override;
};



void tlux_diagnostic::SetUp()
{
    app.init(0,nullptr);

}

void tlux_diagnostic::TearDown()
{

}

TEST_F(tlux_diagnostic, diagnostic_instance_and_clear)
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
