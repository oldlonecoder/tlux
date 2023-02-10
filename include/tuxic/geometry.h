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
#include <tuxic/stracc.h>



namespace tux
{

template<typename T = int> struct point
{
    T x;
    T y;
    point<T>& operator += (point&& pt ) noexcept { x += pt.x;y += pt.y;return *this; }
    point<T>& operator -= (point&& pt ) noexcept { x -= pt.x;y -= pt.y;return *this; }
    point<T>& operator -= (const point& pt ) { x -= pt.x;y -= pt.y;return *this; }
    point<T>& operator += (const point& pt ) { x += pt.x;y += pt.y;return *this; }
    point<T> operator + (const point& pt) { return {x+pt.x, y+pt.y}; }
    point<T> operator - (const point& pt) { return {x-pt.x, y-pt.y}; }
    bool operator <= (const point& pt) { return (x <= pt.x) && (y <=pt.y); }
    bool operator < (const point& pt) { return (x < pt.x) && (y < pt.y); }
    bool operator >= (const point& pt) { return (x >= pt.x) && (y >= pt.y); }
    bool operator > (const point& pt) { return (x > pt.x) && (y > pt.y); }
    bool operator == (const point& pt) { return (x == pt.x) && (y == pt.y); }
    bool operator != (const point& pt) { return (x != pt.x) || (y !=pt.y); }
    point<T>& operator=(point&& pt) noexcept { x = pt.x; y = pt.y; return *this; }
    point<T>& operator=( const point& pt) { x = pt.x; y = pt.y; return *this; }

    explicit operator std::string() const
    {
        stracc str;
        str << '(' << x << ',' << y << ')';
        return str();
    }
};


template<typename T=int> struct  dim
{
    T w = T{0};
    T h = T{0};


    explicit operator std::string() const
    {
        stracc str;
        str << '{' << w << ',' << h << '}';

        return str();
    }

    T area() { return w*h; }

};


template<typename T = int> struct  rectangle
{
    point<T> a;
    point<T> b;
    dim<T>   wh;

    explicit operator bool() const
    {
        return ( wh.h > T{0} ) && ( wh.w > T{0} );
    }


    explicit operator std::string() const
    {
        stracc str;
        str << "({" << a.x << ',' << a.y << "},{" << b.x << ',' << b.y << "})[" << wh.w << '*' << wh.h << "]=" << wh.w*wh.h;
        return str();
    }

    /*!
     * @brief assign this instance with new values.
     *
     * @param x
     * @param y
     * @param w
     * @param h
     *
     * @note Some glitches is to be seen when T=float
     */
    void assign( T x ,T y, T w, T h )
    {
        a.x = a;
        a.y = b;
        b.x = a.x + w - T{1};
        b.y = a.y + h - T{1};
        wh  = { w,h };
    }
    /*!
     * @brief assign this instance with new values.
     *
     * @param x
     * @param y
     * @param w
     * @param h
     *
     * @note Some glitches is to be seen when T=float
     */
    void assign ( const point<T>& _a, const point<T>& _b )
    {
        a = _a;
        b = _b;
        s ( b.x - a.x + T{1}, b.y - a.y +T{1} );
    }



    /*!
     * @brief build a rectangle from the intersection with the rhs.
     * @param rhs Righ-Hand-Side rectangle<T> argument
     *
     * @code
     * +--------------------+
     * |                    |
     * |                    |   +---------------------------+
     * +--------------------+   |                           |
     *                          |                           |
     *                          |                           |
     *                          +---------------------------+
     * =========================================================
     * +--------------------------+
     * |                          |
     * |                  +-------+----------+
     * |                  |-------|          |
     * |                  |-------|          |
     * |                  +-------+----------+
     * |                          |
     * +--------------------------+
     *
     * @return rectangl<T> resulting from the intersection.
     */
    rectangle<T> intersection(const rectangle<T>& rhs)
    {
        rectangle<T> r;
        point<T> ra,rb;
        ra  = {a.x <= rhs.a.x ? rhs.a.x : a.x, a.y <= rhs.a.y ? rhs.a.y : a.y};
        rb  = {b.x <= rhs.b.x ? b.x : rhs.b.x, b.y <= rhs.b.y ? b.y : rhs.b.y};

        if ( (!contains(ra)) || (!contains(rb)) ) return {};
        r.assign ( ra,rb );
        return r;
    }


    bool contains(point <T> ab)
    {
        return ab >= a && ab <= b;
    }



    rectangle<T> U(const rectangle<T>& rhs)
    {
        T xx1,yy1,xx2,yy2;
        rectangle<T> r;
        xx1 = rhs.a.x < a.x ? rhs.a.x : a.x;
        xx2 = rhs.b.x > b.x ? rhs.b.x : b.x;
        yy1 = rhs.a.y < a.y ? rhs.a.y : a.y;
        yy2 = rhs.b.y > b.y ? rhs.b.y : b.y;

        r.assign ( {xx1,yy1},{xx2,yy2} );
        return r;
    }
    rectangle<T>& operator & (const rectangle<T>& rhs) { return intersection(rhs);}
    rectangle<T>& operator | (const rectangle<T>& rhs) { return U(rhs);}


};

struct  winbuffer
{
    stracc* win = nullptr;
    point<> cxy;
    rectangle<> r;

    winbuffer& gotoxy(int x, int y);
    winbuffer& operator << (point<> xy);
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
