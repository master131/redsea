#ifndef RDSSTRING_H_
#define RDSSTRING_H_

#include <map>
#include <string>
#include <vector>

#include "src/common.h"

namespace redsea {

class RDSChar {
 public:
  LCDchar();
  LCDchar(uint8_t _code);
  uint8_t code() const;
  std::string str() const;
  void setCodeTable(eCodeTable codetable);

 private:
  uint8_t code_;
  eCodeTable codetable_;
};

class RDSString {
 public:
  RDSString(int len = 8);
  void setAt(int pos, LCDchar chr);
  void setAt(int pos, LCDchar chr1, LCDchar chr2);
  void setRepertoire(int pos, eCodeTable codetable);
  eCodeTable repertoireAt(int pos) const;
  size_t lengthReceived() const;
  size_t lengthExpected() const;
  std::vector<RDSChar> chars() const;
  std::string str() const;
  std::string getTrimmedString() const;
  std::string last_complete_string() const;
  std::string last_complete_string(int start, int len) const;
  std::string last_complete_string_trimmed() const;
  std::string last_complete_string_trimmed(int start, int len) const;
  bool has_chars(int start, int len) const;
  bool complete() const;
  void clear();
  void resize(int n);

 private:
  std::vector<RDSChar> chars_;
  std::vector<RDSChar> last_complete_chars_;
  std::vector<bool> is_char_sequential_;
  int prev_pos_;
  std::string last_complete_string_;
  std::map<int, eCodeTable> repertoire_;
};

}  // namespace redsea
#endif  // RDSSTRING_H_
