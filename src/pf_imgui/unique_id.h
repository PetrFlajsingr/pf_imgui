/**
 * @file unique_id.h
 * @brief Unique id generator for elements.
 * @author Petr Flajšingr
 * @date 20.4.21
 */

#ifndef PF_IMGUI_SRC_PF_IMGUI_UNIQUE_ID_H
#define PF_IMGUI_SRC_PF_IMGUI_UNIQUE_ID_H

#include <pf_common/coroutines/Sequence.h>
#include <random>

namespace pf::ui::ig {
namespace details {
static inline auto generator = iota<std::size_t>();

/**
 * Generate random string of length. Statically seeded.
 * @param length desired length
 * @return random string of desired length
 */
inline std::string random_string(std::size_t length) {
  static auto randGen = std::mt19937{465768687};
  auto randchar = []() -> char {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[randGen() % max_index];
  };
  std::string str(length, 0);
  std::generate_n(str.begin(), length, randchar);
  return str;
}
}// namespace details
/**
 * Generates unique ID. It's seeded by a constant, so it should provide the same sequence of IDs every time - usable for Savable elements.
 * @param length length of the id
 * @return ID
 */
inline std::string uniqueId(std::size_t length = 10) {
  return details::random_string(length) + std::to_string(getNext(details::generator));
}
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_UNIQUE_ID_H
