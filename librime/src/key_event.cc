//
// Copyleft 2011 RIME Developers
// License: GPLv3
//
// 2011-04-20 GONG Chen <chen.sst@gmail.com>
//
#include <string>
#include <sstream>
#include <boost/format.hpp>
#include <rime/key_event.h>
#include <rime/key_table.h>

namespace rime {

KeyEvent::KeyEvent(const std::string &repr) {
  if (!Parse(repr))
    keycode_ = modifier_ = 0;
}

std::string KeyEvent::repr() const {
  // stringify modifiers
  std::ostringstream modifiers;
  if (modifier_) {
    int k = modifier_ & kModifierMask;
    const char *modifier_name = NULL;
    for (int i = 0; k; ++i, k >>= 1) {
      if (!(k & 1))
        continue;
      modifier_name = RimeGetModifierName(k << i);
      if (modifier_name) {
        modifiers << modifier_name << '+';
      }
    }
  }
  // first lookup predefined key name
  const char *name = RimeGetKeyName(keycode_);
  if (name)
    return modifiers.str() + name;
  // no name :-| return its hex value
  std::string value;
  if (keycode_ <= 0xffff) {
    value = boost::str(boost::format("0x%4x") % keycode_);
  }
  else if (keycode_ <= 0xffffff) {
    value = boost::str(boost::format("0x%6x") % keycode_);
  }
  else {
    return "(unknown)";  // invalid keycode
  }
  return modifiers.str() + value;
}

bool KeyEvent::Parse(const std::string &repr) {
  keycode_ = modifier_ = 0;
  if (repr.empty()) {
    return false;
  }
  if (repr.size() == 1) {
    keycode_ = static_cast<int>(repr[0]);
  }
  else {
    size_t start = 0;
    size_t found = 0;
    std::string token;
    int mask = 0;
    while ((found = repr.find('+', start)) != std::string::npos) {
      token = repr.substr(start, found - start);
      mask = RimeGetModifierByName(token.c_str());
      if (mask) {
        modifier_ |= mask;
      }
      else {
        LOG(ERROR) << "parse error: unrecognized modifier '" << token << "'";
        return false;
      }
      start = found + 1;
    }
    token = repr.substr(start);
    keycode_ = RimeGetKeycodeByName(token.c_str());
    if (keycode_ == XK_VoidSymbol) {
      LOG(ERROR) << "parse error: unrecognized key '" << token << "'";
      return false;
    }
  }
  return true;
}

KeySequence::KeySequence(const std::string &repr) {
  if (!Parse(repr))
    clear();
}

std::string KeySequence::repr() const {
  std::ostringstream result;
  std::string k;
  for (const_iterator it = begin(); it != end(); ++it) {
    k = it->repr();
    if (k.size() == 1) {
      result << k;
    }
    else {
      result << '{' << k << '}';
    }
  }
  return result.str();
}

bool KeySequence::Parse(const std::string &repr) {
  clear();
  size_t n = repr.size();
  size_t start = 0;
  size_t len = 0;
  KeyEvent ke;
  for (size_t i = 0; i < n; ++i) {
    if (repr[i] == '{' && i + 1 < n) {
      start = i + 1;
      size_t j = repr.find('}', start);
      if (j == std::string::npos) {
        LOG(ERROR) << "parse error: unparalleled brace in '" << repr << "'";
        return false;
      }
      len = j - start;
      i = j;
    }
    else {
      start = i;
      len = 1;
    }
    if (!ke.Parse(repr.substr(start, len))) {
      LOG(ERROR) << "parse error: unrecognized key sequence";
      return false;
    }
    push_back(ke);
  }
  return true;
}

}  // namespace rime