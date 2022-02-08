#include "imcmd_command_palette.h"

#include "imcmd_fuzzy_search.h"

#include <imgui.h>
#include <algorithm>
#include <cstring>
#include <limits>
#include <utility>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

namespace ImCmd
{
// =================================================================
// Private forward decls
// =================================================================

struct StackFrame;
class ExecutionManager;

struct SearchResult;
class SearchManager;

struct CommandOperationRegister;
struct CommandOperationUnregister;
struct CommandOperation;
struct Context;

struct ItemExtraData;
struct Instance;

// =================================================================
// Private interface
// =================================================================

// TODO we should switch to either ImHashStr in imgui.cpp or std::hash (doesn't support const char* natively, and std::string_view is only available in C++17 or above)
// Ad-hoc string hash function
// Adapted from https://stackoverflow.com/a/34597485
static size_t HashCString(const char* p)
{
    size_t result = 0;
    constexpr size_t kPrime = 31;
    for (size_t i = 0; p[i] != '\0'; ++i) {
        result = p[i] + (result * kPrime);
    }
    return result;
}

struct StackFrame
{
    std::vector<std::string> Options;
    int SelectedOption = -1;
};

class ExecutionManager
{
private:
    Instance* m_Instance;
    Command* m_ExecutingCommand = nullptr;
    std::vector<StackFrame> m_CallStack;

public:
    ExecutionManager(Instance& instance)
        : m_Instance{ &instance } {}

    int GetItemCount() const;
    const char* GetItem(int idx) const;
    void SelectItem(int idx);

    void PushOptions(std::vector<std::string> options);
};

struct SearchResult
{
    int ItemIndex;
    int Score;
    int MatchCount;
    uint8_t Matches[32];
};

class SearchManager
{
private:
    Instance* m_Instance;

public:
    std::vector<SearchResult> SearchResults;
    char SearchText[std::numeric_limits<uint8_t>::max() + 1];

public:
    SearchManager(Instance& instance)
        : m_Instance{ &instance }
    {
        std::memset(SearchText, 0, sizeof(SearchText));
    }

    int GetItemCount() const;
    const char* GetItem(int idx) const;

    bool IsActive() const;

    void SetSearchText(const char* text);
    void ClearSearchText();
    void RefreshSearchResults();
};

struct CommandOperationRegister
{
    Command Candidate;
};

struct CommandOperationUnregister
{
    const char* Name;
};

struct CommandOperation
{
    enum OpType
    {
        OpType_Register,
        OpType_Unregister,
    };

    OpType Type;
    int Index;
};

struct Context
{
    ImGuiStorage Instances;
    Instance* CurrentCommandPalette = nullptr;
    std::vector<Command> Commands;
    std::vector<CommandOperationRegister> PendingRegisterOps;
    std::vector<CommandOperationUnregister> PendingUnregisterOps;
    std::vector<CommandOperation> PendingOps;
    ImFont* Fonts[ImCmdTextType_COUNT] = {};
    ImU32 FontColors[ImCmdTextType_COUNT] = {};
    int CommandStorageLocks = 0;
    bool HasFontColorOverride[ImCmdTextType_COUNT] = {};
    bool IsExecuting = false;
    bool IsTerminating = false;

    struct
    {
        bool ItemSelected = false;
    } LastCommandPaletteStatus;

    struct
    {
        const char* NewSearchText = nullptr;
        bool FocusSearchBox = false;
    } NextCommandPaletteActions;

    void RegisterCommand(Command command)
    {
        auto location = std::lower_bound(
            Commands.begin(),
            Commands.end(),
            command,
            [](const Command& a, const Command& b) -> bool {
                return strcmp(a.Name, b.Name) < 0;
            });
        Commands.insert(location, std::move(command));
    }

    bool UnregisterCommand(const char* name)
    {
        struct Comparator
        {
            bool operator()(const Command& command, const char* str) const
            {
                return strcmp(command.Name, str) < 0;
            }

            bool operator()(const char* str, const Command& command) const
            {
                return strcmp(str, command.Name) < 0;
            }
        };

        auto range = std::equal_range(Commands.begin(), Commands.end(), name, Comparator{});
        Commands.erase(range.first, range.second);

        return range.first != range.second;
    }

    bool CommitOps()
    {
        if (IsCommandStorageLocked()) {
            return false;
        }

        for (auto& operation : PendingOps) {
            switch (operation.Type) {
                case CommandOperation::OpType_Register: {
                    auto& op = PendingRegisterOps[operation.Index];
                    RegisterCommand(std::move(op.Candidate));
                } break;

                case CommandOperation::OpType_Unregister: {
                    auto& op = PendingUnregisterOps[operation.Index];
                    UnregisterCommand(op.Name);
                } break;
            }
        }

        bool had_action = !PendingOps.empty();
        PendingRegisterOps.clear();
        PendingUnregisterOps.clear();
        PendingOps.clear();

        return had_action;
    }

    bool IsCommandStorageLocked() const
    {
        return CommandStorageLocks > 0;
    }
};

struct ItemExtraData
{
    bool Hovered = false;
    bool Held = false;
};

struct Instance
{
    ExecutionManager Session;
    SearchManager Search;
    std::vector<ItemExtraData> ExtraData;

    int CurrentSelectedItem = 0;

    struct
    {
        bool RefreshSearch = false;
        bool ClearSearch = false;
    } PendingActions;

    Instance()
        : Session(*this)
        , Search(*this) {}
};

static Context* gContext = nullptr;

// =================================================================
// Private implementation
// =================================================================

int ExecutionManager::GetItemCount() const
{
    if (m_ExecutingCommand) {
        return static_cast<int>(m_CallStack.back().Options.size());
    } else {
        return static_cast<int>(gContext->Commands.size());
    }
}

const char* ExecutionManager::GetItem(int idx) const
{
    if (m_ExecutingCommand) {
        return m_CallStack.back().Options[idx].c_str();
    } else {
        return gContext->Commands[idx].Name;
    }
}

template <class... Ts>
static void InvokeSafe(const std::function<void(Ts...)>& func, Ts... args)
{
    if (func) {
        func(std::forward<Ts>(args)...);
    }
}

void ExecutionManager::SelectItem(int idx)
{
    auto cmd = m_ExecutingCommand;
    size_t initial_call_stack_height = m_CallStack.size();
    if (cmd == nullptr) {
        cmd = m_ExecutingCommand = &gContext->Commands[idx];
        ++gContext->CommandStorageLocks;

        gContext->IsExecuting = true;
        InvokeSafe(m_ExecutingCommand->InitialCallback); // Calls ::Prompt()
        gContext->IsExecuting = false;
    } else {
        m_CallStack.back().SelectedOption = idx;

        gContext->IsExecuting = true;
        InvokeSafe(cmd->SubsequentCallback, idx); // Calls ::Prompt()
        gContext->IsExecuting = false;
    }

    size_t final_call_stack_height = m_CallStack.size();
    if (initial_call_stack_height == final_call_stack_height) {

        gContext->IsTerminating = true;
        InvokeSafe(m_ExecutingCommand->TerminatingCallback); // Shouldn't call ::Prompt()
        gContext->IsTerminating = false;

        m_ExecutingCommand = nullptr;
        m_CallStack.clear();
        --gContext->CommandStorageLocks;

        // If the executed command involved subcommands...
        if (final_call_stack_height > 0) {
            m_Instance->PendingActions.ClearSearch = true;
            m_Instance->CurrentSelectedItem = 0;
        }

        gContext->LastCommandPaletteStatus.ItemSelected = true;
    } else {
        // Something new is prompted
        // It doesn't make sense for "current selected item" to persists through completely different set of options
        m_Instance->PendingActions.ClearSearch = true;
        m_Instance->CurrentSelectedItem = 0;
    }
}

void ExecutionManager::PushOptions(std::vector<std::string> options)
{
    m_CallStack.push_back({});
    auto& frame = m_CallStack.back();

    frame.Options = std::move(options);

    m_Instance->PendingActions.ClearSearch = true;
}

int SearchManager::GetItemCount() const
{
    return static_cast<int>(SearchResults.size());
}

const char* SearchManager::GetItem(int idx) const
{
    int actualIdx = SearchResults[idx].ItemIndex;
    return m_Instance->Session.GetItem(actualIdx);
}

bool SearchManager::IsActive() const
{
    return SearchText[0] != '\0';
}

void SearchManager::SetSearchText(const char* text)
{
    // Note: must detect clang first because clang-cl.exe defines both _MSC_VER and __clang__, but only accepts #pragma clang
#if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4996)
#endif
    // Copy at most IM_ARRAYSIZE(SearchText) chars from `text` to `SearchText`
    std::strncpy(SearchText, text, IM_ARRAYSIZE(SearchText));
#if defined(__GNUC__)
#    pragma GCC diagnostic pop
#elif defined(__clang__)
#    pragma clang diagnostic pop
#elif defined(_MSC_VER)
#    pragma warning(pop)
#endif
    RefreshSearchResults();
}

void SearchManager::ClearSearchText()
{
    std::memset(SearchText, 0, IM_ARRAYSIZE(SearchText));
    SearchResults.clear();
}

void SearchManager::RefreshSearchResults()
{
    m_Instance->CurrentSelectedItem = 0;
    SearchResults.clear();

    int item_count = m_Instance->Session.GetItemCount();
    for (int i = 0; i < item_count; ++i) {
        const char* text = m_Instance->Session.GetItem(i);
        SearchResult result;
        if (FuzzySearch(SearchText, text, result.Score, result.Matches, IM_ARRAYSIZE(result.Matches), result.MatchCount)) {
            result.ItemIndex = i;
            SearchResults.push_back(result);
        }
    }

    std::sort(
        SearchResults.begin(),
        SearchResults.end(),
        [](const SearchResult& a, const SearchResult& b) -> bool {
            // We want the biggest element first
            return a.Score > b.Score;
        });
}

// =================================================================
// API implementation
// =================================================================

Context* CreateContext()
{
    auto ctx = new Context();
    if (!gContext) {
        gContext = ctx;
    }
    return ctx;
}

void DestroyContext()
{
    DestroyContext(gContext);
    gContext = nullptr;
}

void DestroyContext(Context* context)
{
    delete context;
}

void SetCurrentContext(Context* context)
{
    gContext = context;
}

Context* GetCurrentContext()
{
    return gContext;
}

void AddCommand(Command command)
{
    IM_ASSERT(gContext != nullptr);

    if (gContext->IsCommandStorageLocked()) {
        gContext->PendingRegisterOps.push_back(CommandOperationRegister{ std::move(command) });
        CommandOperation op;
        op.Type = CommandOperation::OpType_Register;
        op.Index = static_cast<int>(gContext->PendingRegisterOps.size()) - 1;
        gContext->PendingOps.push_back(op);
    } else {
        gContext->RegisterCommand(std::move(command));
    }

    if (auto current = gContext->CurrentCommandPalette) {
        current->PendingActions.RefreshSearch = true;
    }
}

void RemoveCommand(const char* name)
{
    IM_ASSERT(gContext != nullptr);

    if (gContext->IsCommandStorageLocked()) {
        gContext->PendingUnregisterOps.push_back(CommandOperationUnregister{ name });
        CommandOperation op;
        op.Type = CommandOperation::OpType_Unregister;
        op.Index = static_cast<int>(gContext->PendingUnregisterOps.size()) - 1;
        gContext->PendingOps.push_back(op);
    } else {
        gContext->UnregisterCommand(name);
    }

    if (auto current = gContext->CurrentCommandPalette) {
        current->PendingActions.RefreshSearch = true;
    }
}

void SetStyleFont(ImCmdTextType type, ImFont* font)
{
    gContext->Fonts[type] = font;
}

void SetStyleColor(ImCmdTextType type, ImU32 color)
{
    gContext->FontColors[type] = color;
    gContext->HasFontColorOverride[type] = true;
}

void ClearStyleColor(ImCmdTextType type)
{
    gContext->HasFontColorOverride[type] = false;
}

void SetNextCommandPaletteSearch(const char* text)
{
    IM_ASSERT(gContext != nullptr);
    IM_ASSERT(text != nullptr);
    gContext->NextCommandPaletteActions.NewSearchText = text;
}

void SetNextCommandPaletteSearchBoxFocused()
{
    IM_ASSERT(gContext != nullptr);
    gContext->NextCommandPaletteActions.FocusSearchBox = true;
}

void CommandPalette(const char* name)
{
    IM_ASSERT(gContext != nullptr);

    auto& gg = *gContext;
    auto& gi = *[&]() {
        auto id = HashCString(name);
        if (auto ptr = gg.Instances.GetVoidPtr(id)) {
            return reinterpret_cast<Instance*>(ptr);
        } else {
            auto instance = new Instance();
            gg.Instances.SetVoidPtr(id, instance);
            return instance;
        }
    }();

    float width = ImGui::GetWindowContentRegionWidth();
    float search_result_window_height = 400.0f; // TODO config

    // BEGIN this command palette
    gg.CurrentCommandPalette = &gi;
    ImGui::PushID(name);

    gg.LastCommandPaletteStatus = {};

    // BEGIN processing PendingActions
    bool refresh_search = gi.PendingActions.RefreshSearch;
    refresh_search |= gg.CommitOps();

    if (auto text = gg.NextCommandPaletteActions.NewSearchText) {
        refresh_search = false;
        if (text[0] == '\0') {
            gi.Search.ClearSearchText();
        } else {
            gi.Search.SetSearchText(text);
        }
    } else if (gi.PendingActions.ClearSearch) {
        refresh_search = false;
        gi.Search.ClearSearchText();
    }

    if (refresh_search) {
        gi.Search.RefreshSearchResults();
    }

    gi.PendingActions = {};
    // END procesisng PendingActions

    if (gg.NextCommandPaletteActions.FocusSearchBox) {
        // Focus the search box when user first brings command palette window up
        // Note: this only affects the next frame
        ImGui::SetKeyboardFocusHere(0);
    }
    ImGui::SetNextItemWidth(width);
    if (ImGui::InputText("##SearchBox", gi.Search.SearchText, IM_ARRAYSIZE(gi.Search.SearchText))) {
        // Search string updated, update search results
        gi.Search.RefreshSearchResults();
    }

    ImGui::BeginChild("SearchResults", ImVec2(width, search_result_window_height));

    auto window = ImGui::GetCurrentWindow();
    auto draw_list = window->DrawList;

    auto font_regular = gg.Fonts[ImCmdTextType_Regular];
    if (!font_regular) {
        font_regular = ImGui::GetDrawListSharedData()->Font;
    }
    auto font_highlight = gg.Fonts[ImCmdTextType_Highlight];
    if (!font_highlight) {
        font_highlight = ImGui::GetDrawListSharedData()->Font;
    }

    ImU32 text_color_regular;
    ImU32 text_color_highlight;
    if (gg.HasFontColorOverride[ImCmdTextType_Regular]) {
        text_color_regular = gg.FontColors[ImCmdTextType_Regular];
    } else {
        text_color_regular = ImGui::GetColorU32(ImGuiCol_Text);
    }
    if (gg.HasFontColorOverride[ImCmdTextType_Highlight]) {
        text_color_highlight = gg.FontColors[ImCmdTextType_Highlight];
    } else {
        text_color_highlight = ImGui::GetColorU32(ImGuiCol_Text);
    }

    auto item_hovered_color = ImGui::GetColorU32(ImGuiCol_HeaderHovered);
    auto item_active_color = ImGui::GetColorU32(ImGuiCol_HeaderActive);
    auto item_selected_color = ImGui::GetColorU32(ImGuiCol_Header);

    int item_count;
    if (gi.Search.IsActive()) {
        item_count = gi.Search.GetItemCount();
    } else {
        item_count = gi.Session.GetItemCount();
    }

    if (gi.ExtraData.size() < item_count) {
        gi.ExtraData.resize(item_count);
    }

    // Flag used to delay item selection until after the loop ends
    bool select_focused_item = false;
    for (int i = 0; i < item_count; ++i) {
        auto id = window->GetID(static_cast<int>(i));

        ImVec2 size{
            ImGui::GetContentRegionAvail().x,
            ImMax(font_regular->FontSize, font_highlight->FontSize),
        };
        ImRect rect{
            window->DC.CursorPos,
            window->DC.CursorPos + ImGui::CalcItemSize(size, 0.0f, 0.0f),
        };

        bool& hovered = gi.ExtraData[i].Hovered;
        bool& held = gi.ExtraData[i].Held;
        if (held && hovered) {
            draw_list->AddRectFilled(rect.Min, rect.Max, item_active_color);
        } else if (hovered) {
            draw_list->AddRectFilled(rect.Min, rect.Max, item_hovered_color);
        } else if (gi.CurrentSelectedItem == i) {
            draw_list->AddRectFilled(rect.Min, rect.Max, item_selected_color);
        }

        if (gi.Search.IsActive()) {
            // Iterating search results: draw text with highlights at matched chars

            auto& search_result = gi.Search.SearchResults[i];
            auto text = gi.Search.GetItem(i);

            auto text_pos = window->DC.CursorPos;
            int range_begin;
            int range_end;
            int last_range_end = 0;

            auto DrawCurrentRange = [&]() {
                if (range_begin != last_range_end) {
                    // Draw normal text between last highlighted range end and current highlighted range start
                    auto begin = text + last_range_end;
                    auto end = text + range_begin;
                    draw_list->AddText(text_pos, text_color_regular, begin, end);

                    auto segment_size = font_regular->CalcTextSizeA(font_regular->FontSize, std::numeric_limits<float>::max(), 0.0f, begin, end);
                    text_pos.x += segment_size.x;
                }

                auto begin = text + range_begin;
                auto end = text + range_end;
                draw_list->AddText(font_highlight, font_highlight->FontSize, text_pos, text_color_highlight, begin, end);

                auto segment_size = font_highlight->CalcTextSizeA(font_highlight->FontSize, std::numeric_limits<float>::max(), 0.0f, begin, end);
                text_pos.x += segment_size.x;
            };

            IM_ASSERT(search_result.MatchCount >= 1);
            range_begin = search_result.Matches[0];
            range_end = range_begin;

            int last_char_idx = -1;
            for (int j = 0; j < search_result.MatchCount; ++j) {
                int char_idx = search_result.Matches[j];

                if (char_idx == last_char_idx + 1) {
                    // These 2 indices are equal, extend our current range by 1
                    ++range_end;
                } else {
                    DrawCurrentRange();
                    last_range_end = range_end;
                    range_begin = char_idx;
                    range_end = char_idx + 1;
                }

                last_char_idx = char_idx;
            }

            // Draw the remaining range (if any)
            if (range_begin != range_end) {
                DrawCurrentRange();
            }

            // Draw the text after the last range (if any)
            draw_list->AddText(text_pos, text_color_regular, text + range_end); // Draw until \0
        } else {
            // Iterating everything else: draw text as-is, there is no highlights

            auto text = gi.Session.GetItem(i);
            auto text_pos = window->DC.CursorPos;
            draw_list->AddText(text_pos, text_color_regular, text);
        }

        ImGui::ItemSize(rect);
        if (!ImGui::ItemAdd(rect, id)) {
            continue;
        }
        if (ImGui::ButtonBehavior(rect, id, &hovered, &held)) {
            gi.CurrentSelectedItem = i;
            select_focused_item = true;
        }
    }

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
        gi.CurrentSelectedItem = ImMax(gi.CurrentSelectedItem - 1, 0);
    } else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
        gi.CurrentSelectedItem = ImMin(gi.CurrentSelectedItem + 1, item_count - 1);
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)) || select_focused_item) {
        if (gi.Search.IsActive() && !gi.Search.SearchResults.empty()) {
            auto idx = gi.Search.SearchResults[gi.CurrentSelectedItem].ItemIndex;
            gi.Session.SelectItem(idx);
        } else {
            gi.Session.SelectItem(gi.CurrentSelectedItem);
        }
    }

    ImGui::EndChild();

    gg.NextCommandPaletteActions = {};

    ImGui::PopID();
    gg.CurrentCommandPalette = nullptr;
    // END this command palette
}

bool IsAnyItemSelected()
{
    IM_ASSERT(gContext != nullptr);
    return gContext->LastCommandPaletteStatus.ItemSelected;
}

void RemoveCache(const char* name)
{
    IM_ASSERT(gContext != nullptr);

    auto& instances = gContext->Instances;
    auto id = HashCString(name);
    if (auto ptr = instances.GetVoidPtr(id)) {
        auto instance = reinterpret_cast<Instance*>(ptr);
        instances.SetVoidPtr(id, nullptr);
        delete instance;
    }
}

void RemoveAllCaches()
{
    IM_ASSERT(gContext != nullptr);

    auto& instances = gContext->Instances;
    for (auto& entry : instances.Data) {
        auto instance = reinterpret_cast<Instance*>(entry.val_p);
        entry.val_p = nullptr;
        delete instance;
    }
    instances = {};
}

void SetNextWindowAffixedTop(ImGuiCond cond)
{
    auto viewport = ImGui::GetMainViewport()->Size;

    // Center window horizontally, align top vertically
    ImGui::SetNextWindowPos(ImVec2(viewport.x / 2, 0), cond, ImVec2(0.5f, 0.0f));
}

void CommandPaletteWindow(const char* name, bool* p_open)
{
    auto viewport = ImGui::GetMainViewport()->Size;

    SetNextWindowAffixedTop();
    ImGui::SetNextWindowSize(ImVec2(viewport.x * 0.3f, 0.0f));
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if (ImGui::IsWindowAppearing()) {
        SetNextCommandPaletteSearchBoxFocused();
    }

    CommandPalette(name);

    if (IsAnyItemSelected()) {
        *p_open = false;
    }
    if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
        // Close popup when user unfocused the command palette window (clicking elsewhere)
        *p_open = false;
    }

    ImGui::End();
}

void Prompt(std::vector<std::string> options)
{
    IM_ASSERT(gContext != nullptr);
    IM_ASSERT(gContext->CurrentCommandPalette != nullptr);
    IM_ASSERT(gContext->IsExecuting);
    IM_ASSERT(!gContext->IsTerminating);

    auto& gi = *gContext->CurrentCommandPalette;
    gi.Session.PushOptions(std::move(options));
}
} // namespace ImCmd
