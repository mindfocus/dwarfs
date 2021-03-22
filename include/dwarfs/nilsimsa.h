/* vim:set ts=2 sw=2 sts=2 et: */
/**
 * \author     Marcus Holland-Moritz (github@mhxnet.de)
 * \copyright  Copyright (c) Marcus Holland-Moritz
 *
 * This file is part of dwarfs.
 *
 * dwarfs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dwarfs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dwarfs.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "dwarfs/compiler.h"

#define DWARFS_NILSIMSA_SIMILARITY(r, a, b)                                    \
  do {                                                                         \
    int bits = 0;                                                              \
    for (int i = 0; i < 4; ++i) {                                              \
      if constexpr (std::is_same_v<unsigned long, uint64_t>) {                 \
        bits += __builtin_popcountl(a[i] ^ b[i]);                              \
      } else if constexpr (std::is_same_v<unsigned long long, uint64_t>) {     \
        bits += __builtin_popcountll(a[i] ^ b[i]);                             \
      }                                                                        \
    }                                                                          \
    r 255 - bits;                                                              \
  } while (false)

namespace dwarfs {

class nilsimsa {
 public:
  using hash_type = std::array<uint64_t, 4>;

  nilsimsa();
  ~nilsimsa();

  void update(uint8_t const* data, size_t size);
  void finalize(hash_type& hash) const;

#ifdef DWARFS_MULTIVERSIONING
  __attribute__((target("popcnt"))) static int
  similarity(uint64_t const* a, uint64_t const* b);

  __attribute__((target("default")))
#endif
  static int
  similarity(uint64_t const* a, uint64_t const* b);

 private:
  class impl;

  std::unique_ptr<impl> impl_;
};

} // namespace dwarfs
