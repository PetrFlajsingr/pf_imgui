//
// Created by petr on 4/18/21.
//

#ifndef PF_IMGUI_DETAILS_COMBOFILTER_H
#define PF_IMGUI_DETAILS_COMBOFILTER_H

#include <imgui.h>

// imgui combo filter v1.0, by @r-lyeh (public domain)
// contains code by @harold-b (public domain?)

/*  Demo: */
/*
    {
        // requisite: hints must be alphabetically sorted beforehand
        const char *hints[] = {
            "AnimGraphNode_CopyBone",
            "ce skipaa",
            "ce skipscreen",
            "ce skipsplash",
            "ce skipsplashscreen",
            "client_unit.cpp",
            "letrograd",
            "level",
            "leveler",
            "MacroCallback.cpp",
            "Miskatonic university",
            "MockAI.h",
            "MockGameplayTasks.h",
            "MovieSceneColorTrack.cpp",
            "r.maxfps",
            "r.maxsteadyfps",
            "reboot",
            "rescale",
            "reset",
            "resource",
            "restart",
            "retrocomputer",
            "retrograd",
            "return",
            "slomo 10",
            "SVisualLoggerLogsList.h",
            "The Black Knight",
        };
        static ComboFilterState s = {0};
        static char buf[128] = "type text here...";
        if( ComboFilter("my combofilter", buf, IM_ARRAYSIZE(buf), hints, IM_ARRAYSIZE(hints), s) ) {
            puts( buf );
        }
    }
*/
namespace pf::ui::ig {

/**
 * @brief State of combo filter.
 */
struct ComboFilterState {
  int activeIdx;         /*!< Index of currently 'active' item by use of up/down keys */
  bool selectionChanged; /*!< Flag to help focus the correct item when selecting active item */
};

static inline bool ComboFilter__DrawPopup(ComboFilterState &state, [[maybe_unused]] int START, const char **ENTRIES, int ENTRY_COUNT) {
  bool clicked = 0;

  // Grab the position for the popup
  ImVec2 pos = ImGui::GetItemRectMin();
  pos.y += ImGui::GetItemRectSize().y;
  ImVec2 size = ImVec2(ImGui::GetItemRectSize().x - 60, ImGui::GetFrameHeightWithSpacing() * 4);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoSavedSettings | 0;  //ImGuiWindowFlags_ShowBorders;

  ImGui::SetNextWindowFocus();

  ImGui::SetNextWindowPos(pos);
  ImGui::SetNextWindowSize(size);
  ImGui::Begin("##combo_filter", nullptr, flags);

  ImGui::PushAllowKeyboardFocus(false);

  for (int i = 0; i < ENTRY_COUNT; i++) {
    // Track if we're drawing the active index so we
    // can scroll to it if it has changed
    bool isIndexActive = state.activeIdx == i;

    if (isIndexActive) {
      // Draw the currently 'active' item differently
      // ( used appropriate colors for your own style )
      ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 0, 1));
    }

    ImGui::PushID(i);
    if (ImGui::Selectable(ENTRIES[i], isIndexActive)) {
      // And item was clicked, notify the input
      // callback so that it can modify the input buffer
      state.activeIdx = i;
      clicked = 1;
    }
    if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGui::GetIO().KeyMap[ImGuiKey_Enter])) {
      // Allow ENTER key to select current highlighted item (w/ keyboard navigation)
      state.activeIdx = i;
      clicked = 1;
    }
    ImGui::PopID();

    if (isIndexActive) {
      if (state.selectionChanged) {
        // Make sure we bring the currently 'active' item into view.
        ImGui::SetScrollHereX();
        ImGui::SetScrollHereY();
        state.selectionChanged = false;
      }

      ImGui::PopStyleColor(1);
    }
  }

  ImGui::PopAllowKeyboardFocus();
  ImGui::End();
  ImGui::PopStyleVar(1);

  return clicked;
}

static inline bool ComboFilter(const char *id, char *buffer, int bufferlen, const char **hints, int num_hints, ComboFilterState &s) {
  struct fuzzy {
    static int score(const char *str1, const char *str2) {
      int score = 0, consecutive = 0, maxerrors = 0;
      while (*str1 && *str2) {
        int is_leading = (*str1 & 64) && !(str1[1] & 64);
        if ((*str1 & ~32) == (*str2 & ~32)) {
          int had_separator = (str1[-1] <= 32);
          int x = had_separator || is_leading ? 10 : consecutive * 5;
          consecutive = 1;
          score += x;
          ++str2;
        } else {
          int x = -1, y = is_leading * -3;
          consecutive = 0;
          score += x;
          maxerrors += y;
        }
        ++str1;
      }
      return score + (maxerrors < -9 ? -9 : maxerrors);
    }
    static int search(const char *str, int num, const char *words[]) {
      int scoremax = 0;
      int best = -1;
      for (int i = 0; i < num; ++i) {
        int score = fuzzy::score(words[i], str);
        int record = (score >= scoremax);
        int draw = (score == scoremax);
        if (record) {
          scoremax = score;
          if (!draw) best = i;
          else
            best = best >= 0 && strlen(words[best]) < strlen(words[i]) ? best : i;
        }
      }
      return best;
    }
  };
  bool done = ImGui::InputText(id, buffer, bufferlen, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
  bool hot = s.activeIdx >= 0 && strcmp(buffer, hints[s.activeIdx]);
  if (hot) {
    int new_idx = fuzzy::search(buffer, num_hints, hints);
    int idx = new_idx >= 0 ? new_idx : s.activeIdx;
    s.selectionChanged = s.activeIdx != idx;
    s.activeIdx = idx;
    if (done || ComboFilter__DrawPopup(s, idx, hints, num_hints)) {
      int i = s.activeIdx;
      if (i >= 0) {
        strcpy(buffer, hints[i]);
        done = true;
      }
    }
  }
  return done;
}
}  // namespace pf::ui::ig
#endif  //PF_IMGUI_SRC_PF_IMGUI_DETAILS_COMBOFILTER_H
