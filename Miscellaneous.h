//
// Created by michalsulek on 13.01.2020.
//

#ifndef ZPR_MISCELLANEOUS_H
#define ZPR_MISCELLANEOUS_H

#include <iostream>

template <typename type>
std::ostream& operator<< (std::ostream& os, const std::vector<type>& container) {
    for(const auto& i: container) {
        os << i;
    }
    return os;
}

#endif //ZPR_MISCELLANEOUS_H
