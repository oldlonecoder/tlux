/***************************************************************************
 *   Copyright (C) 2005/2023 by Serge Lussier                              *
 *   2005: (bretzel@tuxweb.homelinux.net)                                  *
 *   2023: lussier.serge@gmail.com, oldlonecoder@gmail.com                 *
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

#include "tlux/application.h"


namespace tux{

application* application::_self = nullptr;

code::T application::init_regitery()
{
    return code::notimplemented;
}

application::application():object()
{
    application::_self = this;
    diagn = diagnostic(this,"application::diagnostic");
}

application::~application()
{
    application::_self = nullptr;
    diagn.codes_data.clear();
    diagn.types_data.clear();

}

code::T application::init(int argc, char** argv)
{

    diagn.types_data = {
        {code::none,    {Icon::Computer,    {color::Aquamarine1         ,color::Reset }}},
        {code::error,   {Icon::Err1,        {color::Red4                ,color::Reset }}},
        {code::warning, {Icon::Warning,     {color::Yellow              ,color::Reset }}},
        {code::fatal,   {Icon::DeadHead,    {color::HotPink4            ,color::Reset }}},
        {code::except,  {Icon::Bolt,        {color::SkyBlue3            ,color::Reset }}},
        {code::message, {Icon::Comment,     {color::Green4              ,color::Reset }}},
        {code::output,  {Icon::NotePad,     {color::Grey39              ,color::Reset }}},
        {code::debug,   {Icon::Bug,         {color::Pink3               ,color::Reset }}},
        {code::info,    {Icon::Info,        {color::LightSkyBlue4       ,color::Reset }}},
        {code::comment, {Icon::Comment,     {color::Grey42              ,color::Reset }}},
        {code::syntax,  {Icon::Handwrite,   {color::LighcoreateBlue     ,color::Reset }}},
        {code::status,  {Icon::File,        {color::LightCyan3          ,color::Reset }}},
        {code::test,    {Icon::Handwrite,   {color::Aquamarine3         ,color::Reset }}}
        //...
    };

    diagn.codes_data = {
        {code::ok,              {Icon::ThumbUp,         {color::LightGreen2       ,color::Reset }}},
        {code::accepted,        {Icon::Success,         {color::LightGreen2       ,color::Reset }}},
        {code::success,         {Icon::Success,         {color::LightGreen2       ,color::Reset }}},
        {code::rejected,        {Icon::ThumbDown,       {color::Orange5           ,color::Reset }}},
        {code::failed,          {Icon::Poop,            {color::DarkGoldenrod     ,color::Reset }}},
        {code::empty,           {Icon::ArrowRight,      {color::LighcoreateGrey   ,color::Reset }}},
        {code::full,            {Icon::SmallDot,        {color::White             ,color::Reset }}},
        {code::notempty,        {Icon::BigDot,          {color::White             ,color::Reset }}},
        {code::implemented,     {Icon::ArrowHeadRight,  {color::Lime              ,color::Reset }}},
        {code::notimplemented,  {Icon::Err1,            {color::Orange3           ,color::Reset }}},
        {code::untested,        {Icon::Flag,            {color::Yellow            ,color::Reset }}},
        {code::eof,             {Icon::Baseline,        {color::White             ,color::Reset }}},
        {code::eos,             {Icon::Baseline,        {color::White             ,color::Reset }}},
        {code::null_ptr,        {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
        {code::endl,            {Icon::CircleX,         {color::HotPink4          ,color::Reset }}},
        {code::enter,           {Icon::KeyEnter,        {color::Green4            ,color::Reset }}},
        {code::leave,           {Icon::Enter,           {color::Green4            ,color::Reset }}},
        {code::begin     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
        {code::end       ,      {Icon::Downtobottomleft,{color::White             ,color::Reset }}},
        {code::notexist  ,      {Icon::CircleX ,        {color::White             ,color::Reset }}},
        {code::exist     ,      {Icon::Star5 ,          {color::White             ,color::Reset }}},
        {code::unexpected,      {Icon::Flag ,            {color::Yellow             ,color::Reset }}},
        {code::expected  ,      {Icon::Cat ,            {color::White             ,color::Reset }}},
        {code::blocked   ,      {Icon::UnAuth ,         {color::IndianRed3        ,color::Reset }}},
        {code::locked    ,      {Icon::Err3 ,           {color::White             ,color::Reset }}}
//        {code::function  ,      {Icon::Function ,       {color::SkyBlue3          ,color::Reset }}},
//        {code::file      ,      {Icon::File ,           {color::White             ,color::Reset }}},
//        {code::line      ,      {Icon::Baseline ,       {color::White             ,color::Reset }}},
//        {code::stamp     ,      {Icon::Chronos ,        {color::Yellow            ,color::Reset }}},
//        {code::hour      ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
//        {code::minute    ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
//        {code::seconds   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}},
//        {code::weekday   ,      {Icon::Chronos ,        {color::White             ,color::Reset }}}
    };

    for(int a = 0; a< argc; a++)
        _args.push_back(argv[a]);

     (void)init_regitery();
    //...
    return code::ok;
}



code::T application::run()
{
    if(!application::_self)
        return code::null_ptr;

    return _app_start();
}

code::T application::terminate()
{
    //...
    return _app_end();
}



application &application::self()
{
    if(!application::_self)
        throw "illegal use of non-instanciated application ";

    return *application::_self;
}

diagnostic &application::diagnostic_instance()
{
    auto& app = application::self();
    return app.diagn;
}

code::code_attribute_table &application::codes_data()
{
    return application::diagnostic_instance().codes_data;
}




} // tux
