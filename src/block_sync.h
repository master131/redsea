#ifndef BLOCK_SYNC_H_
#define BLOCK_SYNC_H_

#include <map>
#include <vector>

#include "src/ascii_in.h"
#include "config.h"
#include "src/groups.h"
#include "src/subcarrier.h"

namespace redsea {

uint32_t CalcSyndrome(uint32_t vec);
eOffset OffsetForSyndrome(uint16_t syndrome);
eOffset NextOffsetFor(eOffset o);
uint32_t CorrectBurstErrors(uint32_t block, eOffset offset);

class BlockStream {
 public:
  explicit BlockStream(Options options);
  Group NextGroup();
  bool eof() const;
#ifdef DEBUG
  float t() const;
#endif

 private:
  int NextBit();
  void Uncorrectable();
  bool AcquireSync();

  unsigned bitcount_;
  unsigned prevbitcount_;
  int left_to_read_;
  uint32_t padded_block_;
  unsigned prevsync_;
  unsigned block_counter_;
  eOffset expected_offset_;
  eOffset received_offset_;
  uint16_t pi_;
  bool is_in_sync_;
  std::vector<bool> block_has_errors_;
#ifdef HAVE_LIQUID
  Subcarrier subcarrier_;
#endif
  AsciiBits ascii_bits_;
  const eInputType input_type_;
  bool is_eof_;
};

} // namespace redsea
#endif // BLOCK_SYNC_H_
