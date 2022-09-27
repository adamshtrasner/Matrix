#include <exception>
#include <string>

class MatrixException : public std::exception{
 private:
  std::string message_;
 public:
  MatrixException(const std::string& message):message_(message){}
  const char* what() const noexcept override{
    return message_.c_str();
  }
};