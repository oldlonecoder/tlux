#include <tlux/stracc.h>






namespace tux
{

//impl_rttn(stracc)

std::string::size_type stracc::_scan_arg()
{
    _arg_pos = _d.find("%", 0);
    return _arg_pos;
}

stracc::stracc()
{
}


stracc::~stracc()
{
    _d.clear();
}

stracc &stracc::fill(char c, int count)
{
    //std::fill_n(_d.begin(), count, c);
    _d += std::string(count, c);
    return *this;
}

stracc& stracc::operator << (textattr::pair arg_)
{
//    if(_scan_arg() != std::string::npos)
//        return _format(attr<textattr::format::ansi256>::fg(arg_));
    _d += attr<textattr::format::ansi256>::bg(arg_.bg);
    _d += attr<textattr::format::ansi256>::fg(arg_.fg);
    return *this;

}


stracc& stracc::operator<<(color::type arg_)
{

    if(_scan_arg() != std::string::npos)
        return _format(attr<textattr::format::ansi256>::fg(arg_));

    _d += attr<textattr::format::ansi256>::fg(arg_);
    return *this;
}

stracc &stracc::operator<<(const stracc &acc)
{
    if(_scan_arg() != std::string::npos)
        return _format(acc());
    _d += acc();
    return *this;
}

stracc &stracc::operator<<(Icon::Type Ic)
{
    if(_scan_arg() != std::string::npos)
        return _format(Icon::Data[Ic]);
    _d += Icon::Data[Ic]; /// DUH!!!!!!!!!!!!!!!!!!
    return *this;
}

stracc &stracc::operator<<(Accent::Type Ac)
{
    if(_scan_arg() != std::string::npos)
        return _format(Accent::Data[Ac]);
    _d += Accent::Data[Ac]; /// DUH!!!!!!!!!!!!!!!!!!
    return *this;
}

void stracc::clear()
{
    _d.clear();
}


}
