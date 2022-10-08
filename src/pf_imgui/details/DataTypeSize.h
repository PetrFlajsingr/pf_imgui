//
// Created by xflajs00 on 08.10.2022.
//

#ifndef PF_IMGUI_DETAILS_DATATYPESIZE_H
#define PF_IMGUI_DETAILS_DATATYPESIZE_H

#include <imgui.h>
#include <cstddef>

namespace ImGui {

constexpr std::size_t SizeForDataType(ImGuiDataType dataType) {
  switch (dataType) {
    case ImGuiDataType_S8: return sizeof(char);
    case ImGuiDataType_U8: return sizeof(unsigned char);
    case ImGuiDataType_S16: return sizeof(short);
    case ImGuiDataType_U16: return sizeof(unsigned short);
    case ImGuiDataType_S32: return sizeof(int);
    case ImGuiDataType_U32: return sizeof(unsigned int);
    case ImGuiDataType_S64: return sizeof(ImS64);
    case ImGuiDataType_U64: return sizeof(ImU64);
    case ImGuiDataType_Float: return sizeof(float);
    case ImGuiDataType_Double: return sizeof(double);
    default: break;
  }
  return {};
}

}

#endif  //PF_IMGUI_DETAILS_DATATYPESIZE_H
