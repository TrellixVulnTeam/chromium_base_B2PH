#ifndef QUICHE_SPDY_CORE_SPDY_HEADER_STORAGE_H_
#define QUICHE_SPDY_CORE_SPDY_HEADER_STORAGE_H_

#include "net/third_party/quiche/src/spdy/core/spdy_simple_arena.h"
#include "net/third_party/quiche/src/spdy/platform/api/spdy_export.h"
#include "net/third_party/quiche/src/spdy/platform/api/spdy_string_piece.h"

namespace spdy {

// This class provides a backing store for SpdyStringPieces. It previously used
// custom allocation logic, but now uses an UnsafeArena instead. It has the
// property that SpdyStringPieces that refer to data in SpdyHeaderStorage are
// never invalidated until the SpdyHeaderStorage is deleted or Clear() is
// called.
//
// Write operations always append to the last block. If there is not enough
// space to perform the write, a new block is allocated, and any unused space
// is wasted.
class SPDY_EXPORT_PRIVATE SpdyHeaderStorage {
 public:
  SpdyHeaderStorage();

  SpdyHeaderStorage(const SpdyHeaderStorage&) = delete;
  SpdyHeaderStorage& operator=(const SpdyHeaderStorage&) = delete;

  SpdyHeaderStorage(SpdyHeaderStorage&& other) = default;
  SpdyHeaderStorage& operator=(SpdyHeaderStorage&& other) = default;

  SpdyStringPiece Write(SpdyStringPiece s);

  // If |s| points to the most recent allocation from arena_, the arena will
  // reclaim the memory. Otherwise, this method is a no-op.
  void Rewind(SpdyStringPiece s);

  void Clear() { arena_.Reset(); }

  // Given a list of fragments and a separator, writes the fragments joined by
  // the separator to a contiguous region of memory. Returns a SpdyStringPiece
  // pointing to the region of memory.
  SpdyStringPiece WriteFragments(const std::vector<SpdyStringPiece>& fragments,
                                 SpdyStringPiece separator);

  size_t bytes_allocated() const { return arena_.status().bytes_allocated(); }

  size_t EstimateMemoryUsage() const { return bytes_allocated(); }

 private:
  SpdySimpleArena arena_;
};

// Writes |fragments| to |dst|, joined by |separator|. |dst| must be large
// enough to hold the result. Returns the number of bytes written.
SPDY_EXPORT_PRIVATE size_t Join(char* dst,
                                const std::vector<SpdyStringPiece>& fragments,
                                SpdyStringPiece separator);

}  // namespace spdy

#endif  // QUICHE_SPDY_CORE_SPDY_HEADER_STORAGE_H_
