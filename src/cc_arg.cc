#include "../include/tlux/cc_arg.h"

namespace tux {

_argument::_argument(const std::string &opt_name_, char letter_, uint8_t opt_, int require_narg)
{
    _required_args = require_narg;
    _opt = opt_;
    _opt_name = opt_name_;
    _c = letter_;
}


}
