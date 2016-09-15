#include "test_tools.h"

std::ofstream Logger::_file;

size_t Logger::open_log(const char* fname, const char* ext) {
  std::vector<std::string> bad_namexts;
  bad_namexts.clear();
  bad_namexts.resize(BN_SIZE);
  for (size_t i = 0; i < bad_namexts.size(); ++i) {
    bad_namexts[i].clear();
  }
  // bad_namexts content may alter
  bad_namexts[BN_ZERO].append("\0");
  bad_namexts[BN_PREV_DIR].append("..");
  bad_namexts[BN_CURR_DIR].append(".");
  for (size_t i = 0; i < bad_namexts.size(); ++i) {
    if (strcmp(fname, bad_namexts[i].data()) || strcmp(ext, bad_namexts[i].data())) {
      return LR_BAD_NAMEXT;
    }
  }
  if (_file.is_open()) {
    _file.close();
  }
  std::string full_name;
  full_name.clear();
  full_name.append(fname);
  full_name.append(ext);
  _file.open(full_name.data(), std::ios::out | std::ios::trunc);
  return LR_SUCCESS;
}

size_t Logger::close_log() {
  if (_file.is_open()) {
    _file.close();
    return LR_SUCCESS;
  } else {
    return LR_NOT_OPENED;
  }
}
