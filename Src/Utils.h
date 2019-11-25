#ifndef VPN_UTILS
#define VPN_UTILS

#include <cstdlib>
#include <string>
#include <cstdio>
#include <iostream>


namespace vpn {



template <class T>
std::string str(T const& n) {
    return std::to_string(n);
}


void exit_with_error(const char * error, int err_no=1);


template <class T>
void user_message(T const& t) 
{
    std::cout << t << std::endl;
}


template <class T, class... Args>
void user_message(T const& t, Args... args)
{
    std::cout << t << " ";
    user_message(args...);
}


}

#endif // VPN_UTILS
