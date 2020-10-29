/*
 *
 */

#ifndef __XPARSER_PARSEREXCEPTION_H__
#define __XPARSER_PARSEREXCEPTION_H__

// system includes
#include <exception>
#include <string>

// base object includes

// project sub directory includes - alphabetical order

// project includes - alphabetical order

namespace xparser {

class ParserException: public std::exception {

public:
    ParserException(const std::string& what) {
        mWhat = what;
    }

    virtual const char* what() const throw()
    {
      return mWhat.c_str();
    }

private:
    std::string mWhat;

};

}

#endif /* __XPARSER_PARSEREXCEPTION_H__ */

