/*
 * Nick Codignotto
 *
 * nick.codignotto@gmail.com / twitter: @nickcoding / blog: nickcoding.com
 *
 */

#include <sstream>
#include <chrono>
#include <vector>
#include <algorithm>

#include "common.h"

namespace common {
  std::string trim(const std::string& str) {
    const size_t len = str.length();
    size_t a = 0, b = len - 1;
    char c;
    while (a < len && ((c = str.at(a)) == ' ' || c == '\t'))
      a++;
    while (b > a && ((c = str.at(b)) == ' ' || c == '\t'))
      b--;
    return str.substr(a, 1 + b - a);
  }

  std::string collapse_line(const std::string& message) {
    std::string oneline = message;
    std::replace(oneline.begin(), oneline.end(), '\n', ',');
    return oneline;
  }
} // namespace common