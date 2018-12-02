#ifndef COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_CASING_HPP_
#define COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_CASING_HPP_

#include <string>

namespace strutil {

std::string toUpper(const std::string& str);

std::string toLower(const std::string& str);

std::string stripTrailingWhitespace(std::string str);

std::string strip(std::string str);

}  // namespace strutil

#endif  // COMMON_STRUTIL_INCLUDE_STRUTIL_IMPL_CASING_HPP_
