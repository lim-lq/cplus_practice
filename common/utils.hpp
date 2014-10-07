#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <sstream>
#include <boost/uuid/sha1.hpp>

std::string& strip(std::string& o_str, const char& ch)
{
    if ( *o_str.rbegin() == ch ) {
        o_str.erase(o_str.begin() + o_str.size() - 1);
    }

    return o_str;
}

std::string gen_sha1(const std::string& passwd)
{
    boost::uuids::detail::sha1 sha;
    std::ostringstream oss;

    sha.process_bytes(passwd.c_str(), passwd.size());
    unsigned int digest[5];

    sha.get_digest(digest);
    for ( int i = 0; i < 5; ++i ) {
        oss << std::hex << digest[i];
    }
    return oss.str();
}

template <typename T>
T stringTo(const std::string& s)
{
    T t;
    std::istringstream iss(s);
    iss >> t;
    return t;
}

#endif