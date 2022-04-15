//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMGUI_BLUEPRINT_COMMON_H
#define PF_IMGUI_BLUEPRINT_COMMON_H

#include <static_type_info.h>
#include <type_traits>

#define PF_IMGUI_BLUEPRINT_OVERRIDE_GETTYPEID(type)                                                                    \
  [[nodiscard]] inline static_type_info::TypeIndex getTypeId() const override {                                        \
    return static_type_info::getTypeIndex<type>();                                                                     \
  }

namespace pf::ui::ig::bp {
namespace details {

using IntPtr_t = std::conditional_t<
    sizeof(void *) == sizeof(std::int64_t), std::int64_t,
    std::conditional_t<sizeof(void *) == sizeof(std::int32_t), std::int32_t,
                       std::conditional_t<sizeof(void *) == sizeof(std::int16_t), std::int16_t, std::int8_t>>>;

}
}  // namespace pf::ui::ig::bp

#endif  //PF_IMGUI_BLUEPRINT_COMMON_H
