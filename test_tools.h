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

const size_t COUNTER_DEFAULT_VALUE = 0;

const char* BASE_EXT = ".log";
const char* OK_MSG = "OK. ";
const char* FAIL_MSG = "Test failed. ";
const char* TEST_SMALL_SEPARATOR = "\n<< ---------------------------------------------------------------------- >>\n";
const char* TEST_BIG_SEPARATOR = "\n<< ====================================================================== >>\n";

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
    static size_t reset_counter() { _total_tests = COUNTER_DEFAULT_VALUE; return 0; }
    static size_t reset_success() { _success_count = COUNTER_DEFAULT_VALUE; return 0; }
    static size_t reset_failed() { _failed_count = COUNTER_DEFAULT_VALUE; return 0; }
    static size_t get_total_tests() { return _total_tests; }
    static size_t get_success_count() { return _success_count; }
    static size_t get_failed_count() { return _failed_count; }
    static size_t get_success_percentage();
    static size_t print_test_data();
  protected:
    static size_t _total_tests;
    static size_t _success_count;
    static size_t _failed_count;
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
  std::vector<std::string> result_names;
  result_names.clear();
  result_names.resize(CR_SIZE);
  for (size_t i = 0; i < result_names.size(); ++i) {
    result_names[i].clear();
  }
  result_names[CR_EQUAL].append("EQUAL");
  result_names[CR_LESS].append("LESS");
  result_names[CR_MORE].append("MORE");
  ++_total_tests;
  if (result == check_value) {
    printf(OK_MSG);
    printf("Required result: %s, comparison result: %s.\n", result_names[check_value].data(), result_names[result].data());
    if (Logger::is_open()) {
      Logger::write<const char*>(OK_MSG);
      Logger::write<const char*>("Required result: ");
      Logger::write<const char*>(result_names[check_value].data());
      Logger::write<const char*>(", comparison result: ");
      Logger::write<const char*>(result_names[result].data());
      Logger::write<const char*>(".\n");
    }
    ++_success_count;
    return TR_SUCCESS;
  } else {
    printf(FAIL_MSG);
    printf("Required result: %s, comparison result: %s.\n", result_names[check_value].data(), result_names[result].data());
    if (Logger::is_open()) {
      Logger::write<const char*>(FAIL_MSG);
      Logger::write<const char*>("Required result: ");
      Logger::write<const char*>(result_names[check_value].data());
      Logger::write<const char*>(", comparison result: ");
      Logger::write<const char*>(result_names[result].data());
      Logger::write<const char*>(".\n");
    }
    ++_failed_count;
    return TR_FAILED;
  }
}

#endif

