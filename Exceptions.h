#include "string"

class InvalidValueError : public std::exception {
private:
    std::string message;
public:
    InvalidValueError() : message{"Invalid value of record"} {

    }

    const char *what() const noexcept override { return message.c_str(); }
};
