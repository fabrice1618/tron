#ifndef INITERROR_H
#define INITERROR_H

#include <string>

/*
Exception : InitError
*/
class InitError : public std::exception
{
private:
    std::string msg;

public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

#endif // INITERROR_H
