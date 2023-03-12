
#include <tlux/stml/text.h>
#include <map>




namespace tux
{
[[maybe_unused]] std::string_view text_tokenizer::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";


std::string text_tokenizer::token_t::operator()() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;
}

std::string text_tokenizer::token_t::operator*() const
{
    std::string Str;
    if(mStart == mEnd)
        Str.insert(Str.begin(), mStart, mEnd + 1);
    else
        Str.insert(Str.begin(), mStart, mEnd + 1);

    return Str;

}

std::string text_tokenizer::token_t::Mark() const
{
    std::string Str;
    string_view::iterator CStart = mStart - mPosition;

    //int                   l  = 1;
    string_view::iterator cc = CStart;
    // localiser le debut de la ligne;
    while(*cc && (cc > CStart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= CStart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != mSE) && (*cc != '\n') && (*cc != '\r'))
            Str += *cc++;
    }

    Str += '\n'; ///@todo Encode new line.
    for(int x = 1; x < mCol; x++)
        Str += ' ';
    Str += '^';
    return Str;
}

std::string text_tokenizer::token_t::location()
{
    std::ostringstream Str;
    Str << '(' << mLine << ',' << mCol << ')';
    return Str.str();
}


// ------------------------ text_tokenizer::s_p_s --------------------------------


text_tokenizer::s_p_s::s_p_s(std::string_view aStr):
    _begin(aStr.begin()),
    _pos(aStr.begin()),
    _end(aStr.begin())
{

}

/*!
    @note Enleveer le code de trop (suivi du num de ligne et de la col).
*/
bool text_tokenizer::s_p_s::skip_ws()
{
     if(end())
        return false;

    while(isspace(*_pos))
    {
        switch(*_pos)
        {
            case 10:
            {
                if((++_pos) >= _end)
                    return false;
                if(*_pos == 13)
                    ++_pos;
                ++_line;
                _col = 1;
            }
                break;
            case 13:
            {
                if((++_pos) >= _end)
                    return false;
                if(*_pos == 10)
                    ++_pos;
                ++_line;
                _col = 1;
            }
                break;
            case '\t':++_pos;
                ++_col;
                break;
            default:++_pos;
                ++_col;
                break;
        }
    }
    return _pos < _end;

}


bool text_tokenizer::s_p_s::end() const
{
    return _pos >= _end;
}


bool text_tokenizer::s_p_s::operator++()
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}
bool text_tokenizer::s_p_s::operator++(int)
{
    if(_pos >= _end)
        return false;
    ++_pos;
    ++_col;
    if(_pos >= _end)
        return false;
    return skip_ws();
}

text_tokenizer::s_p_s &text_tokenizer::s_p_s::operator>>(text_tokenizer::token_t &aWord)
{
    aWord.mStart    = _pos;
    aWord.mLine     = _line;
    aWord.mCol      = _col;
    aWord.mPosition = _index = (uint64_t) (_pos - _begin);
    return *this;
}

text_tokenizer::iterator text_tokenizer::scan_to(text_tokenizer::iterator aStart, char c) const
{
    text_tokenizer::iterator p = aStart;
    ++p;
    while((p != _d.end()) && (*p != c))
        ++p;
    return p;
}


/*!
    * @brief break/split/tokenize,etc... the content of a string into pieces.
    * @param text_tokenizer::token_t::list  OUTPUT reference to the 'Tokens array' containter, filled by this method.
    * @param aDelimiters Separators in the form of a std::sting_view of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as they appear
    * @return number of "Words/tokens" contained into the collection.
    * @notice : After several years of experience and experimentations, We have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c aDelimiters.
    */

std::size_t text_tokenizer::operator()(text_tokenizer::token_t::list &Collection, string_view aDelimiters, bool KeepAsWord) const
{

    auto Crs = text_tokenizer::s_p_s(_d);
    if(_d.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.reset(_d);
    string_view token_separators = aDelimiters.empty() ? text_tokenizer::_default_token_separators : aDelimiters;
    if(!Crs.skip_ws())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    token_t w;
    Crs >> w;

    while(!Crs.end())
    {
        //if (!wcollection.empty());
        text_tokenizer::iterator cc = Crs._pos;
        if(token_separators.find(*Crs._pos) != string_view::npos)
        {
            cc = Crs._pos;
            if(cc > w.mStart)
            {
                --cc;
                Collection.push_back({w.mStart, cc, Crs._end, w.mLine, w.mCol, w.mPosition});
                Crs >> w;
                cc = Crs._pos;
            }

            // '//' as one token_t instead of having two consecutive '/'
            if((*Crs._pos == '/') && (*(Crs._pos + 1) == '/'))
                ++Crs;

            if(KeepAsWord)
            {
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.end())
                Crs >> w;
            else
            {
                return Collection.size();
            }

        }
        else if((*Crs._pos == '\'') || (*Crs._pos == '"'))
        { // Quoted litteral string...
            Crs >> w;
            if(KeepAsWord)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the token_t coords anyway.
                Collection.push_back({w.mStart, w.mStart, Crs._end, w.mLine, w.mCol, w.mPosition});
            }

            text_tokenizer::iterator p = scan_to(w.mStart + (KeepAsWord ? 0 : 1), *Crs._pos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs._pos < p)
                ++Crs; // compute white spaces!!!

            if(KeepAsWord)
            {
                // then push the litteral that is inside the quotes.
                Collection.push_back({w.mStart + 1, p - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                Collection.push_back({w.mStart, p, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the token_t.
                Collection.push_back({w.mStart, Crs._pos, Crs._end, w.mLine, w.mCol, w.mPosition});
            }
            if(++Crs)
                Crs >> w;
            else
                return Collection.size();

        }
        else
        {
            cc = Crs._pos;
            ++cc;
            if(cc == Crs._end)
            {
                ++Crs._pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.mStart < cc)
                {
                    Collection.push_back({w.mStart, cc - 1, Crs._end, w.mLine, w.mCol, w.mPosition});
                    ++Crs;
                }

                if(Crs.skip_ws())
                {
                    Crs >> w;
                    continue;
                }
                return Collection.size();
            }
            if(!Crs.end())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs._pos > w.mStart)
        Collection.push_back({w.mStart, Crs._pos - 1, Crs._end, w.mLine, w.mCol, w.mPosition});

    return Collection.size();
}

//text_tokenizer& text_tokenizer::operator>>(text_tokenizer::token_t::List& Collection)
//{
//    mConfig.z = Tokenize(mConfig.Collection, mConfig.Delim, mConfig.Keep);
//    return *this;
//}

text_tokenizer::text_tokenizer(string_view aStr):
    _d(aStr)
{

}
text_tokenizer &text_tokenizer::operator=(string_view aStr)
{
    _d = aStr;
    return *this;
}
text_tokenizer &text_tokenizer::operator=(const char *aStr)
{
    _d = aStr;
    return *this;
}
text_tokenizer::text_tokenizer(char *aStr): _d(aStr)
{

}
text_tokenizer::text_tokenizer(string_view aStr, string_view Delim, bool KeepDelim):
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}


text_tokenizer::text_tokenizer(char* aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}




text_tokenizer::text_tokenizer(const std::string& aStr, string_view Delim, bool KeepDelim) :
    _d(aStr)
{
    _cfg.delimiters = Delim;
    _cfg.keep = KeepDelim;
}



text_tokenizer::text_tokenizer(const char* aStr) : _d(aStr)
{}


text_tokenizer::text_tokenizer(std::string aStr): _d(aStr.c_str())
{

}






//--------------------------------------------------------------------------------------------------------------------------------------------


    // Referentiel des tokens du processeur :
    text::token_data::list text::token_data::Referential =
    {
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::OpenTag,     text::token_data::OpenSeq, 0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ClosingTag,  text::token_data::CloseSeq,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSeq,      text::token_data::ArgSeq,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::ArgSep,      text::token_data::ArgSep,0},
        {text::token_data::type::Punctuation,    text::token_data::mnemonic::Eos,         text::token_data::EoSt,0},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Color,       text::token_data::ColorPair,2},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Fg,          text::token_data::Fg,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Bg,          text::token_data::Bg,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Ic,          text::token_data::Icon,1},
        {text::token_data::type::AttrCmd,        text::token_data::mnemonic::Br,          text::token_data::LineBreak,0},
        {text::token_data::type::Accent,         text::token_data::mnemonic::AccentSeq,   text::token_data::Accent,1}
        //...
    };

    // Parsers:




    text::text()
    {

    }

    text::text(const text& other)
    {
        _d = other._d;
    }

    text::text(const std::string& Str) :_d(Str) {}

    text::text(std::string&& Str) noexcept : _d(std::move(Str)) {}

    text::text(const char* aStr) : _d(aStr) {}

    text::~text()
    {
        _d.clear();
        _attributes.clear();
    }



    text::token_data text::token_data::scan(const char* Start)
    {
        //code::Debug(SourceLocation) << " Entering with Start :[" << *Start << "] -> '" << Start << "':";
        if (!*Start)
            return {};

        for (auto TokenRef : text::token_data::Referential)
        {
            TokenRef._location.s_begin = Start;
            while (isspace(*Start)) ++Start;

            auto crs = Start;
            const char* rtxt = TokenRef.L;
            auto sz = std::strlen(rtxt);

            if (toupper(*crs) != toupper(*rtxt)) continue;

            while (*rtxt && *crs && (toupper(*crs) == toupper(*rtxt))) { ++crs; ++rtxt; }
            //diagnostic::debug(sfnl) << "Token.L = [" << color::Yellow << TokenRef.L << color::Reset << "]:";
            if (!*rtxt)
            {   // fin de Token.L :
                if (TokenRef.T == token_data::type::AttrCmd)
                {
                    // Si le mot continu alors ce n'est pas un Token:
                    if (*crs)
                    {   // *crs n'est pas un type d'espace:
                        if (isalnum(*crs))
                            continue;
                    }
                } // Il reste les autres non-espace comme les ponctuations, symboles...
                --crs; // Replacer crs sur le dernier caractere du Token.
                //diagnostic::debug(sfnl) << " Scanned to :'" << color::Yellow << *crs << color::Reset << '\'';
                TokenRef._location.begin = Start;
                TokenRef._location.end = crs; // Fin du Token

                return TokenRef; // On le retourne.... doh!
            }
        }
        diagnostic::debug(sfnl) << " '" << *Start << "' not a TokenRef";
        return {}; // crs ( ou Start )  n'est pas sur un token du referentiel.
    }


    text::token_data::token_data(text::token_data::type aType, text::token_data::mnemonic aMnenomic, text::token_data::Lexem aLexem, int nParams) :
        T(aType), M(aMnenomic), L(aLexem),NParam(nParams)
    {

    }

    text::token_data::token_data(text::token_data&& r) noexcept :
        T(r.T), M(r.M), L(r.L)
    {

        _location = r._location;
        //code::Debug(SourceLocation) << " ->[" << (*this)() << "]";
    }

    std::string text::token_data::mark(const char* Stream)
    {
        if (!*this)
            return " - Can't mark an invalid token...";
        const char* B = _location.begin;
        const char* E = _location.end;
        while ((B >= Stream) && (*B != '\n')) --B;
        ++B;
        while (*E && (*E != '\n')) ++E;
        --E;

        std::string MStr = std::string(_location.begin - Stream, ' ');

        MStr += Icon::CArrowUp;
        stracc Str;
        Str << color::Grey78 << std::string(B, E - B) << '\n' << color::Yellow << MStr << color::Reset;
        return Str();
    }

    code::M text::compile()
    {
        text::compiler Parse{ *this };
        auto A = Parse.execute();

        diagnostic::debug(sfnl) << code::begin << " Number of compiled Attribute(s) :" << _attributes.size();
        for (auto const& Att : _attributes)
        {
            diagnostic::info(sfnl) << Att();
            diagnostic::output() << color::White << "Fg:" << color::Yellow << static_cast<int>(Att._fg) << color::White << ", Bg:" << color::Yellow << static_cast<int>(Att._bg);
            diagnostic::output() << color::White << "Icon:" << color::Yellow << static_cast<int>(Att._icn);
        }
        diagnostic::debug(sfnl) << code::end << ";";
        return code::accepted;
    }

    code::M text::operator>>(std::string& Out)
    {

        const char* r = _d.c_str();
        auto l = _d.length();
        const char* b = r;

        std::string AttrStr;

        for (auto const& A : _attributes)
        {

            while (r != A._begin)
            {
                Out += *r;
                r++;
            }
            r += A().length();
            if(A._assign._for && A._assign._bac)
            {
                textattr::pair{A._fg, A._bg} >> AttrStr;
                Out += AttrStr;
            }
            else
            {

                if (A._assign._bac)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::bg(A._bg) : attr<textattr::format::html>::bg(A._bg);
                    Out += AttrStr;
                }
                else
                if (A._assign._for)
                {
                    AttrStr = _f == textattr::format::ansi256 ? attr<textattr::format::ansi256>::fg(A._fg) : attr<textattr::format::html>::fg(A._fg);
                    Out += AttrStr;
                }
            }

            if (A._assign._acc)
            {
                AttrStr = tux::Accent::Data[A._accnt];
                Out += AttrStr;
            }
            else
            {
                if (A._assign._icn)
                {
                    AttrStr = tux::Icon::Data[A._icn];
                    Out += AttrStr;
                }
                //...
            }
        }
        while (static_cast<size_t>(r - b) < l)
        {
            Out += *r++;
        }
        return code::ok;
    }

    std::string text::operator<<(const std::string& input_str)
    {
        _d = input_str;
        std::string output;
        auto r = compile();
        if (r > 2) return "";
        (*this) >> output;
        return output;
    }

    void text::push_attribute(text::attribute Attr)
    {
        _attributes.push_back(Attr);
    }

    text::token_data text::scan(text::token_data::mnemonic M)
    {
        for (auto iM : text::token_data::Referential)
        {
            if (iM.M == M) return iM;
        }
        return {};
    }


    //     text::Attribute::Attribute(const char* Start)
    //     {
    //         Begin = C = End = Start;
    //     }

    std::string text::token_data::operator()()
    {
        if (L == nullptr)
            return { "null" };

        return { _location.begin, (std::size_t)(_location.end - _location.begin) + 1 };
    }

    text::compiler::compiler(text& aTextInstance) :
        B(aTextInstance._d.c_str()), C(aTextInstance._d.c_str()), E(aTextInstance._d.c_str()), TextRef(aTextInstance)
    {
        E = B + aTextInstance._d.length() - 1;
    }

    bool text::compiler::operator ++()
    {
        return false;
    }

    bool text::compiler::operator ++(int)
    {
        return false;
    }

    //code::M text::Compiler::Skip()
    //{
    //    //code::Debug() << " text::TextParser::Skip(" << color::Yellow << *C << color::Reset << "):";
    //    while (C <= E)
    //    {
    //        ++C;
    //        if (!isspace(*C))
    //            return code::Code::Accepted;
    //    }
    //    return code::Code::Eof;
    //}



    void text::compiler::close_token(token_data& Info)
    {
        Info._location.index = static_cast<int>(Info._location.end - B);
        auto sz = Info._location.end - Info._location.begin;
        C += sz + 1;

        diagnostic::debug({}) << " Token :" << code::endl << Info.mark(B);

    }

    text::token_data text::compiler::scan()
    {
        auto Token = text::token_data::scan(C);
        if (!Token)
        {
            auto R = scan_identifier();
            if (!R)
            {
                diagnostic::syntax({}) << " In text::compiler::scan :" << code::endl << Token.mark(TextRef._d.c_str());
                return {};
            }
        }
        return Token;
    }


    /*!
        @brief Compile le texte pour creer les attributs :

        @code
            Compile :
            loop:
                SkipToAttr
                CompileAttribute
            loop
        @endcode
    */
    code::M text::compiler::execute()
    {
        // Build tokens stream:
        while (!eof())
        {

            auto Token = skip_to_attribute();
            text::attribute Attr;
            text::attribute A;
            if (!Token) return code::eof;
            if (Token.M == text::token_data::mnemonic::AccentSeq)
            {
                Attr._begin = Token._location.begin;
                A = compile_accent(Attr);
            }
            else if (Token.M == text::token_data::mnemonic::OpenTag)
            {
                Attr._begin = Token._location.begin;
                A = compile_attribute(Attr);
            }

            if (!A)
            {
                diagnostic::debug() << " No attribute. Skipping to next \"OpenTag | AccentSeq\"..." << code::endl << mark();
                //Skip();
                continue;
            }
            TextRef.push_attribute(Attr);

        }
        return code::accepted;
    }

    /*!
        @brief Compiler une sequence d'attribut(s).

        Ici c'est moins &eacute;vident... On d&eacute;coupe ce qui est entre '<' et '>' en commandes d'attribut.
        //...
        // '<' 'Fg' ':' 'ColorID' ';'...'Color' : 'ColorID(Fg)' ',' 'ColorID(Bg)' ';' 'Reset' ';' '>'

        @note  "Lexing And Parsing... tout &ccedile;a en m&ecirc;me temps!".
    */
    text::attribute text::compiler::compile_attribute(text::attribute& Attr)
    {

        text::token_data Token;
        // On tokenize les elements d'attribut: ( Stop: ClosingTag('>') Token )
        std::map<text::token_data::mnemonic, text::compiler::parser_fnptr_t> Parsers =
        {
            {text::token_data::mnemonic::Fg, &text::compiler::parse_fg},
            {text::token_data::mnemonic::Bg, &text::compiler::ParseBg},
            {text::token_data::mnemonic::Color, &text::compiler::parse_color},
            {text::token_data::mnemonic::Ic, &text::compiler::parse_icon},
            //... Plus tard, on aura les Grilles et Fenetres
            {text::token_data::mnemonic::ClosingTag, &text::compiler::close_attribute},
        };
        while (!eof())
        {
            // on passe '<'
            //Skip();
            code::M ER;
            // Expecting text::token_data::mnemonic:
            Token = text::token_data::scan(C);
            if (!Token)
            {
                diagnostic::syntax({}) << " Expected ACM (Attribute Command mnemonic) Token" << code::endl << mark();
                return {};
            }
            // Ici c'est obligatoire de faire une boucle qui teste explicitement les mnemonics sp&eacute;cifiques
            // parce qu'on ne prend en charge tous les mnemonics.
            for (auto [M, Fn] : Parsers)
            {
                if (M != Token.M) continue;
                eat_token(Token);
                ER = (this->*Fn)(Attr);
                if (!ER) return {};
                break;
            }
            if (Token.M == text::token_data::mnemonic::ClosingTag)
            {
                eat_token(Token);
                Attr._end = Token._location.end;
                return Attr; // Peut &ecirc;tre vide si on es sur "<>"
            }
        }
        diagnostic::syntax({}) << " Unexpected end of stream in Attribute parsing";
        return {};
    }



    /*!
        @brief Cr&eacute;&eacute; un Attribute de texte Accent.

        @note Encoder un Accent est le plus facile, donc pas besoin d'accumuler les tokens ici: juste "parser" '&' + Id + ';' c'est tout!
    */
    text::attribute text::compiler::compile_accent(text::attribute& Attr)
    {

        text::token_data Token;
        //Skip();
        if(eof())
        {
            diagnostic::syntax({}) << "text::compiler::compile_accent :  Expected identifier.";
            return {};
        }

        Token = scan_identifier();
        if (!Token)
        {
            diagnostic::syntax({}) << " Expected identifier." << code::endl << mark();
        }

        diagnostic::debug() << " Identifier: " << Token();

        Accent::Type T = Accent::Code(Token());
        if (T == Accent::Err)
        {
            diagnostic::syntax({}) << " Unknown Code token(identifier) " << code::endl << Token.mark(B);
            return {};
        }


        Attr = text::attribute(Token);
        Attr._accnt = T;
        Attr._assign._acc = 1;
        //Mandatory expect ';'
        (void)eat_token(Token);
        if(check_eos(Attr)) return Attr;
        diagnostic::syntax({}) << " Expected Eos ';' (End Of Statement token)." << code::endl << mark();
        return {};
    }

    /*!
        @brief Tokenize jusqu'&agrave; '>' ou ';', puis compiler l'attribut.
    */

    text::token_data text::compiler::skip_to_attribute()
    {
        text::token_data Token;

        while (!eof())
        {
            auto c = toupper(*C);
            if (c != token_data::OpenSeq[0])
            {
                if (c != token_data::Accent[0])
                {
                    ++C;
                    continue;
                }
                Token.T = text::token_data::type::Accent;
                Token.L = text::token_data::Accent;
                Token.M = text::token_data::mnemonic::AccentSeq;
            }
            else
            {
                Token.T = text::token_data::type::AttrCmd;
                Token.L = text::token_data::OpenSeq;
                Token.M = text::token_data::mnemonic::OpenTag;
            }
            Token._location.begin = Token._location.end = C;
            Token._location.index = static_cast<int>(C - TextRef._d.c_str());
            eat_token(Token);
            return Token;
        }
        return {};
    }

    bool text::compiler::eof() { return C > E; }

   code::M text::compiler::parse_icon(text::attribute& A)
   {
       auto Token = text::token_data::scan(C);
       if (Token.T != text::token_data::type::Punctuation)
       {
           diagnostic::syntax({}) << " Expected Punctuation token ':'" << code::endl << mark();
           return {};
       }
       eat_token(Token);
       Token = scan_identifier();
        auto R = icon_id(Token);
        if (R == Icon::Bomb)
        {
            diagnostic::syntax({}) << code::notexist << " Icon ID:\n" << code::endl << Token.mark(B);
            return {};
        }
        A._icn = R;
        A._assign._icn = 1;
        return check_eos(A);
    }



    code::M text::compiler::parse_fg(text::attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';

        auto Token = text::token_data::scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
        {
            diagnostic::syntax({}) << " Expected token ':' " << code::endl << mark();
            return code::unexpected;
        }

        eat_token(Token);
        Token = scan_identifier();
        if (!Token)
        {
            diagnostic::syntax({}) << " Expected Identifier token." << code::endl << mark();
            return code::expected;
        }

        auto cid = color_id(Token);
        if (cid==color::Reset ) return cid;
        A._fg = cid;
        A._assign._for = 1;
        diagnostic::output() << " Compiler::ParseFg - Token:" << code::endl << Token.mark(B);
        return check_eos(A);
    }

    code::M text::compiler::ParseBg(text::attribute& A)
    {
        // C sur 'Fg'; ( Consomm&eacute; )
        // Attendus :  ':' , 'ColorID', '; | >';
        auto Token = text::token_data::scan(C);
        if ((Token.T != token_data::type::Punctuation) || (Token.L != text::token_data::ArgSeq))
        {
            diagnostic::syntax({}) << code::rejected << " Expected token ':'" << code::endl << mark();
            return code::rejected;
        }
        eat_token(Token);
        Token = scan_identifier();
        if (!Token)
        {
            diagnostic::syntax({}) << code::rejected << " Expected Identifier token." << code::endl << mark();
            return code::rejected;
        }

        auto cid = color_id(Token);
        if (cid == color::Reset) return cid;
        A._bg = cid;
        A._assign._bac = 1;
        diagnostic::output() << " Compiler::ParseFg - Token:" << code::endl << Token.mark(B);

        return check_eos(A);
    }

   std::string text::compiler::mark()
   {
       const char* Left = C;
       const char* Right = C;
       while ((Left >= B) && (*Left != '\n')) --Left;
       ++Left;
       while (Right && (*Right != '\n')) ++Right;
       --Right;

       std::string MStr = std::string(Right - Left, ' ');

       MStr += Icon::CArrowUp;
       stracc Str;
       Str << color::Grey78 << std::string(Left, Right - Left) << '\n' << color::Yellow << MStr << color::Reset;
       return Str();
   }






    code::M text::compiler::parse_color(text::attribute& A)
    {

        auto Token = text::token_data::scan(C);

        if (Token.M != text::token_data::mnemonic::ArgSeq)
        {
            diagnostic::syntax(sfnl) << code::unexpected << code::endl << Token.mark(B);
            return code::unexpected;
        }

        eat_token(Token);

        Token = scan_identifier();
        if(!Token)
        {
            diagnostic::syntax(sfnl) << code::expected << " Expected Identifier token." << code::endl << mark();
            return code::expected;
        }

        auto cid = color_id(Token);
        if (cid==color::Reset) return cid;

        A._fg = cid;
        A._assign._for = 1;
        // Ici on doit verfifer si on a une virgule ou eos ou closing tag;
        // result "," | ';' | '>'.

        Token = text::token_data::scan(C);
        if ((Token.M != text::token_data::mnemonic::ArgSep) && (Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag))
        {
            diagnostic::syntax() << code::expected << "Expected ',' (arg separator) or eos (';') or closing tag ('>') " << code::endl << Token.mark(B);
            return code::rejected;
        }
        if ((Token.M == text::token_data::mnemonic::Eos) || (Token.M == text::token_data::mnemonic::ClosingTag))
        {
            if (A._fg == color::Reset)
            {
                A._bg = A._fg;
                if(Token.M == text::token_data::mnemonic::Eos)
                    eat_token(Token);

                return code::accepted;
            }

        }
        // Ici on a obligatoirement argsep:

        eat_token(Token);

        Token = scan_identifier();
        //...
        cid = color_id(Token);
        A._bg = cid;
        A._assign._bac = 1;
        return check_eos(A);
    }



    code::M text::compiler::parse_br(text::attribute& A)
    {

        return code::notimplemented;
    }

    code::M text::compiler::close_attribute(text::attribute& A)
    {
        //TextRef.PushAttribute(A);
        return code::accepted;
    }

    code::M text::compiler::check_eos(text::attribute& A)
    {
        //Skip();
        auto Token = text::token_data::scan(C);
        if ( (!Token) || ((Token.M != text::token_data::mnemonic::Eos) && (Token.M != text::token_data::mnemonic::ClosingTag)))
        {
            diagnostic::syntax() << code::expected << ":" << code::endl << Token.mark(B);
            return code::rejected;
        }
        if (Token.M == text::token_data::mnemonic::ClosingTag)
        {

            return code::accepted;
        }
        eat_token(Token);
        A._end = Token._location.end;
        return code::accepted;
    }


    text::token_data text::compiler::scan_identifier()
    {
        const char* Sc = C;
        while (isspace(*Sc)) ++Sc;
        C = Sc;
        if (!isalpha(*Sc) && (*Sc != '_'))
        {
            diagnostic::syntax() << code::expected << " Identifier, got " << *Sc << " instead." << code::endl << mark();
            return {};
        }

        text::token_data Token;
        while (*Sc && (isalnum(*Sc) || (*Sc == '_'))) ++Sc;
        --Sc;
        Token._location.begin = C;
        Token._location.end = Sc;
        Token.L = C;
        Token.T = token_data::type::Identifier;
        return Token;
    }

    code::M text::compiler::eat_token(text::token_data& Token)
    {
        C = Token._location.end;
        C++;
        return code::accepted;
    }


    color::type text::compiler::color_id(token_data& Token)
    {
        auto Str = Token();
        color::type Colr = textattr::scan(Str);
        if (Colr == color::Reset)
        {
            if (Str != "Reset")
            {
                diagnostic::error() << " Expected color::type name (strict case match). Got '" << color::Yellow << Str << color::White << "' instead" << code::endl <<Token.mark(B);
                return color::Reset;
            }
        }
        eat_token(Token);
        return Colr;
    }



    Icon::Type text::compiler::icon_id(token_data& Token)
    {
        auto Str = Token();
        Icon::Type IconId = Icon::Scan(Str);
        if (IconId == Icon::Bomb)
        {
            diagnostic::error({}) << " Expected Icon::type name, got '" << color::Yellow << Str << color::White << "' instead:" << code::endl << Token.mark(B);
            return Icon::Bomb;
        }
        eat_token(Token);
        return IconId;
    }

    text::attribute::attribute(token_data& aInfo) : _begin(aInfo._location.begin-1), _end(aInfo._location.end), _crs(nullptr) {}

    std::string text::attribute::operator()() const
    {
        if (_end)
        {
            std::string Str{ _begin, size_t(_end - _begin) + 1 };
            return Str;
        }
        else
        {
            std::string Str{ _begin };
            return Str;
        }
    }

    std::string text::attribute::informations()
    {
        return "implement";
    }



}
