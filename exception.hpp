/** @file Exception.h
 *  Base exception class in GA namespace
 *
 *  Authors: Michal Swiatek, Michal Sulek
 *	Update:	 19.01.2020
 *
 *	Github repository: https://github.com/MichalKonradSulek/ZPR
 */

#ifndef ZPR_ERROR_H
#define ZPR_ERROR_H

#include <string>

namespace ga {
    /**
	 *	@brief	Class for reporting all kinds of errors
	 *
	 *	@details It is derived from std::exception
	 */
class Exception : public std::exception {
private:
    std::string message_;
public:
    explicit Exception(const char* message) noexcept : message_(message) {}
    Exception(const Exception& other) noexcept : message_(other.message_) {}
    const char* what() const noexcept override {
        return message_.data();
    }
    Exception& operator= (const Exception& other) noexcept {
        if(this == &other) return *this;
        message_ = other.message_;
    }
};
}

#endif //ZPR_ERROR_H
