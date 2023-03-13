/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier   *
 *   2005: (bretzel@teaccweb.homelinux.net)
 *   2023: lussier.serge@gmail.com
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

//

//
#pragma once
#include <tuxls/stracc.h>



namespace tux
{

using T = int;
struct TUXLIB point
{

    T x = 0;
    T y = 0;

    using list = std::vector<point>;
    using iterator = point::list::iterator;
    using const_iterator = point::list::const_iterator;

    point() {};
    point(point&&) noexcept = default;
    ~point() {}

    point(T x_, T y_) :x(x_), y(y_) {}
    point(const point& p)
    {
        x = p.x;
        y = p.y;
    }


    point& operator = (const point& p) { x = p.x; y = p.y; return *this; }

    point& operator -= (const point& dxy)
    {
        x -= dxy.x;
        y -= dxy.y;
        return *this;
    }


    point& operator += (const point& dxy)
    {
        x += dxy.x;
        y += dxy.y;
        return *this;
    }

    point operator + (const point& dxy) const
    {
        return {dxy.x+x, dxy.y+y};
    }


    point operator - (const point& dxy) const
    {
        return { x - dxy.x, y - dxy.y };
    }

    point& operator ()(T x_, T y_)
    {
        x = x_;
        y = y_;
        return *this;
    }
    operator std::string() const
    {
        stracc str = "{%d,%d}";
        str << x << y;
        return str();
    }
};


struct TUXLIB dim
{
    point min;
    point max;
    T w = 0;
    T h = 0;

    operator bool() const { return ((w > 0) && ( h > 0)); }
    operator std::string() const;
    T area() { return w * h; }
};


struct TUXLIB rect
{

    point a;
    point b;
    dim   sz;

    using list = std::vector<rect>;
    using iterator = rect::list::iterator;
    using const_iterator = rect::list::const_iterator;

    rect() {}
    ~rect() {}
    rect(rect&& r) noexcept;
    rect(const rect& r);


    rect& operator=(rect&& r) noexcept;
    rect& operator=(const rect& r) ;

    rect(point a_, point b_)
    {
        a = a_;
        b = b_;
        //sz = { {T{100},T{100}}, T{std::abs(b.x - a.x + 1)}, T{std::abs(b.y - a.y + 1)} };
        //sz = { {T{1},T{1}}, {T{b_.x+1 - a_.x},T{b_.y+1 - a_.y}},  T{std::abs(b.x - a.x + 1)}, T{std::abs(b.y - a.y + 1)} };
        sz.w = b.x-a.x + a.x==0?1:0;
        sz.h = b.y-a.y + a.y==0?1:0;
        sz.min = {1,1};
        sz.max = {10000,10000};
    }

    rect(point a_, dim d)
    {
        a = a_;
        sz = d;
        b = { a + point{a.x==0?sz.w-1:sz.w, a.y==0? sz.h-1:sz.h} };
    }


    rect(T x, T y, T dx, T dy)
    {
        a = { x,y };
        b = { x+dx, y+dy };
        sz.min = {1,1};
        sz.max = {10000,10000};
        sz.w = dx;
        sz.h = dy;
    }

    void assign(T x, T y, T w, T h)
    {
        a = { x,y };
        b = { x + w - 1, y + h - 1 };
        sz = { {1,1},{10000,10000}, w,h};
    }

    void assign(point a_, point b_)
    {
        a = a_;
        b = b_;
        sz.w = (b.x+1) - a.x;
        sz.h = (b.y+1) - a.y;
    }

    void assign(point a_, dim dxy)
    {
        a = a_;
        sz = dxy;
        b = { a.x + dxy.w - 1, a.y + dxy.h - 1 };
    }


    rect& operator += (point dx)
    {
        a += dx;
        b += dx;
        return *this;
    }
    rect& operator -= (point dx)
    {
        a -= dx;
        b -= dx;
        return *this;
    }
    void resize(dim new_sz)
    {
        assign({ a.x, a.y }, new_sz);
    }

    void move_at(const point& p)
    {
        a.x = p.x;
        a.y = p.y;
        b.x = a.x + sz.w - 1;
        b.y = a.y + sz.h - 1;
    }


    bool in(point p) const
    {
        return ((p.x >= a.x) && (p.x <= b.x) && (p.y >= a.y) && (p.y <= b.y));
    }


    void move(point dt)
    {
        a += dt;
        b += dt;
    }

    int width()  const { return sz.w; }
    int height() const  { return sz.h; }

    /*!
        @brief intersection between this and r
    */
    rect operator & (rect r) const
    {
        rect ret;
        point a_ = { a.x <= r.a.x ? r.a.x : a.x, a.y <= r.a.y ? r.a.y : a.y };
        point b_ = { b.x >= r.b.x ? r.b.x : b.x, b.y >= r.b.y ? r.b.y : b.y };

        auto c = in(a_) || in(b_);
        ret.assign(a_, b_);
        if (!c)
        {
            ret.sz = {  };
        }

        return ret;
    }

    /*!
        @brief merges this and r
    */
    rect operator | (rect r) const
    {
        rect ret;
        point a_ = { r.a.x <= a.x ? r.a.x : a.x, r.a.y <= a.y ? r.a.y : a.y };
        point b_ = { r.b.x <= b.x ? r.b.x : b.x, r.b.y <= b.y ? r.b.y : b.y };
        ret.assign(a_, b_);
        return ret;
    }

    rect operator + (const point& xy)const {
        return {{a.x + xy.x, a.y + xy.y}, sz};
    }

    std::string to_string() const;
    operator std::string() const ;
    operator bool() const
    {

        return sz.operator bool();
    }
};



struct TUXLIB winbuffer
{
    stracc* win = nullptr;
    point cxy;
    rect r;

    winbuffer& gotoxy(int x, int y);
    winbuffer& operator << (point xy);
    void set_geometry(int w, int h);

    // internal cursor movements:
    winbuffer& operator ++();
    winbuffer& operator ++(int);
    winbuffer& operator --();
    winbuffer& operator --(int);
    // --------------------------
    template<typename T> winbuffer& operator << (T v)
    {
        stracc str;
        str << v;
        return put(str());
    }




    winbuffer& put(const std::string& txt);

    void clear();
    void release();
    std::string details();
    operator std::string();// {return win ? win->str() : "";}

};
}



