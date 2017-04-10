#pragma once

#include "Algorithm.h"

#include <exception>
#include <string>

namespace mesh {

namespace ocl {

class Exception : public std::exception
{
public:
    Exception(int error) : error_(error) {};

    const char * what() const override;

private:
    int error_;
};

class BuildError : public Exception
{
public:
    BuildError(std::string log);

    std::string log() const { return log_; };

private:
    std::string log_;
};



}

}