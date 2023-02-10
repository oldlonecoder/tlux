//
// Created by oldlonecoder on 1/16/23.
//
#include <tuxic/geometry.h>



namespace tux{

winbuffer &winbuffer::gotoxy(int x, int y)
{
    cxy = {x, y};
    return *this;
}

void winbuffer::set_geometry(int w, int h)
{
    r = rectangle<int>({{1,1},{1,1},w,h});

    clear();
}

winbuffer &winbuffer::operator++()
{
    if(cxy.x >= r.wh.w)
    {
        if(cxy.y <= r.wh.h)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;

    return *this;
}

winbuffer &winbuffer::operator++(int)
{
    if(cxy.x >= r.wh.w)
    {
        if(cxy.y <= r.wh.h)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;
    return *this;
}

winbuffer &winbuffer::operator--()
{

    return *this;
}

winbuffer &winbuffer::operator--(int)
{
    return *this;
}

/// <summary>
/// Serge Lussier
/// p = 3
/// 10-13 + 3;
///        *   Serge L*
/// </summary>
/// <param name="txt"></param>
/// <returns></returns>
winbuffer &winbuffer::put(const std::string &txt)
{
    int line_width = r.wh.w - cxy.x;
    auto ln         = txt.length();

    auto dx = line_width <= ln ? line_width : ln;

    std::string::iterator crs = win->str().begin() + cxy.y * r.wh.w + cxy.x;
    auto                  p   = txt.begin();
    for(int               x   = 0; x < dx; x++)
        *crs++ = *p++;

    cxy.x += line_width;

    return *this;
}

void winbuffer::clear()
{
    if(!win)
        win = new stracc;
    else
        win->clear();

    *win = std::string(r.wh.w * r.wh.h, ' ');
    //std::cerr << __PRETTY_FUNCTION__ << "contents: '" << win->_d << "'\n";
}

void winbuffer::release()
{
    delete win;
}

std::string winbuffer::details()
{
    stracc str = "winbuffer details:\n";
    str << (std::string)r << " cursor: " << std::string(cxy);
    return str();
}



winbuffer & winbuffer::operator<<(point<> xy)
{
    gotoxy(xy.x, xy.y);
    return *this;
}




winbuffer::operator std::string()
{
    std::string str;
    if(!win)
        return "oops! there is no such defined and initialised area!";
    str += '\n';
    for(int l = 0; l < r.wh.h; l++)
    {
        for(int c = 0; c < r.wh.w; c++)
            str += *(win->str().begin() + (l * r.wh.w + c));
        str += '\n';
    }
    return str;
}


}
