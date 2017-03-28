#include "src/util.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace redsea {

// extract len bits from word, starting at starting_at from the right
uint16_t Bits(uint16_t word, int starting_at, int len) {
  return ((word >> starting_at) & ((1 << len) - 1));
}

std::string Join(std::vector<std::string> strings, std::string d) {
  std::string result("");
  for (size_t i=0; i < strings.size(); i++) {
    result += strings[i];
    if (i < strings.size()-1)
      result += d;
  }
  return result;
}

std::string Join(std::vector<std::uint16_t> nums, std::string d) {
  std::string result("");
  for (size_t i=0; i < nums.size(); i++) {
    result += std::to_string(nums[i]);
    if (i < nums.size()-1)
      result += d;
  }
  return result;
}

std::string HexString(int value, int num_nybbles) {
  std::stringstream ss;

  ss.fill('0');
  ss.setf(std::ios_base::uppercase);

  ss << std::hex << std::setw(num_nybbles) << value;

  return ss.str();
}

// 3.2.1.6
CarrierFrequency::CarrierFrequency(uint16_t code, bool is_lf_mf) :
    code_(code), is_lf_mf_(is_lf_mf) {
}

bool CarrierFrequency::valid() const {
  return ((is_lf_mf_ && code_ >= 1 && code_ <= 135) ||
         (!is_lf_mf_ && code_ >= 1 && code_ <= 204));
}

int CarrierFrequency::kHz() const {
  int khz = 0;
  if (valid()) {
    if (!is_lf_mf_)
      khz = 87500 + 100 * code_;
    else if (code_ <= 15)
      khz = 144 + 9 * code_;
    else
      khz = 522 + (9 * (code_ - 15));
  }

  return khz;
}

std::string CarrierFrequency::str() const {
  std::stringstream ss;
  if (valid()) {
    float num = (is_lf_mf_ ? kHz() : kHz() / 1000.0f);
    ss.precision(is_lf_mf_ ? 0 : 1);
    ss << std::fixed << num << (is_lf_mf_ ? " kHz" : " MHz");
  } else {
    ss << "N/A";
  }
  return ss.str();
}

bool operator== (const CarrierFrequency &f1,
                 const CarrierFrequency &f2) {
  return (f1.kHz() == f2.kHz());
}

bool operator< (const CarrierFrequency &f1,
                const CarrierFrequency &f2) {
  return (f1.kHz() < f2.kHz());
}

std::vector<std::string> SplitLine(std::string line, char delimiter) {
  std::stringstream ss(line);
  std::vector<std::string> result;

  while (ss.good()) {
    std::string val;
    std::getline(ss, val, delimiter);
    result.push_back(val);
  }

  return result;
}

std::vector<std::vector<std::string>> ReadCSV(std::vector<std::string> csvdata,
                                              char delimiter) {
  std::vector<std::vector<std::string>> lines;

  for (std::string line : csvdata)
    lines.push_back(SplitLine(line, delimiter));

  return lines;
}

std::vector<std::vector<std::string>> ReadCSV(std::string filename,
                                              char delimiter) {
  std::vector<std::vector<std::string>> lines;

  std::ifstream in(filename);
  if (!in.is_open())
    return lines;

  for (std::string line; std::getline(in, line); ) {
    if (!in.good())
      break;

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    std::vector<std::string> fields = SplitLine(line, delimiter);
    lines.push_back(fields);
  }

  in.close();

  return lines;
}

std::vector<CSVRow> ReadCSVWithTitles(std::vector<std::string> csvdata,
                                      char delimiter) {
  std::vector<CSVRow> lines;
  std::map<std::string, int> titles;

  bool is_title_row = true;

  for (std::string line : csvdata) {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    std::vector<std::string> fields = SplitLine(line, delimiter);
    if (is_title_row) {
      for (size_t i = 0; i < fields.size(); i++)
        titles[fields[i]] = i;
      is_title_row = false;
    }

    CSVRow csvrow(titles, fields);
    lines.push_back(csvrow);
  }

  return lines;
}

std::vector<CSVRow> ReadCSVWithTitles(std::string filename, char delimiter) {
  std::vector<std::string> lines;

  std::ifstream in(filename);
  if (in.is_open()) {
    for (std::string line; std::getline(in, line); ) {
      if (!in.good())
        break;

      lines.push_back(line);
    }

    in.close();
  }

  return ReadCSVWithTitles(lines, delimiter);
}

CSVRow::CSVRow(std::map<std::string, int> titles,
               std::vector<std::string> values) :
    titles_(titles), values_(values) {
}

std::string CSVRow::at(std::string title) const {
  return values_.at(titles_.at(title));
}

}  // namespace redsea
