#ifndef TEST_TOOLS_H
#define TEST_TOOLS_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

enum ComparisonResults { CR_EQUAL, CR_LESS, CR_MORE, CR_SIZE };
enum TestResponses { TR_SUCCESS, TR_FAILED, TR_SIZE };
enum LogResponses { LR_SUCCESS, LR_NOT_OPENED, LR_CANT_WRITE, LR_CANT_OPEN, LR_BAD_NAMEXT, LR_SIZE };

enum BadNamexts { BN_EMPTY, BN_ZERO, BN_PREV_DIR, BN_CURR_DIR, BN_SIZE };

const char* BASE_EXT = ".log";
const char* OK_MSG = "OK. ";
const char* FAIL_MSG = "Test failed. ";

class Logger {
  public:
    Logger() {}
    ~Logger() {}
    static size_t open_log(const char* fname, const char* ext = BASE_EXT);
    static size_t close_log();
    static size_t is_open() { return _file.is_open(); }
    template <class T> static size_t write(const T& message);
  protected:
    static std::ofstream _file;
};

template <class T>
size_t Logger::write(const T& message) {
  if (!_file.is_open()) {
    return LR_NOT_OPENED;
  }
  _file << message;
  return LR_SUCCESS;
}

class Tester {
  public:
    template <class T> static size_t compare(const T& lhs, const T& rhs);
    template <class T> static size_t test(const T& lhs, const T& rhs, const size_t& check_value);
};

template <class T>
size_t Tester::compare(const T& lhs, const T& rhs) {
  if (lhs == rhs) {
    return CR_EQUAL;
  }
  if (lhs < rhs) {
    return CR_LESS;
  } else {
    return CR_MORE;
  }
}

template <class T>
size_t Tester::test(const T& lhs, const T& rhs, const size_t& check_value) {
  size_t result = Tester::compare(lhs, rhs);
  if (result == check_value) {
    printf(OK_MSG);
    printf("Required result: %d, comparison result: %d.\n", check_value, result);
    if (Logger::is_open()) {
      Logger::write<const char*>(OK_MSG);
      Logger::write<const char*>("Required result: ");
      Logger::write<size_t>(check_value);
      Logger::write<const char*>(", comparison result: ");
      Logger::write<size_t>(result);
      Logger::write<const char*>(".\n");
    }   
    return TR_SUCCESS;
  } else {
    printf(FAIL_MSG);
    printf("Required result: %d, comparison result: %d.\n", check_value, result);
    if (Logger::is_open()) {
      Logger::write<const char*>(FAIL_MSG);
      Logger::write<const char*>("Required result: ");
      Logger::write<size_t>(check_value);
      Logger::write<const char*>(", comparison result: ");
      Logger::write<size_t>(result);
      Logger::write<const char*>(".\n");
    }
    return TR_FAILED;
  }
}

#endif

