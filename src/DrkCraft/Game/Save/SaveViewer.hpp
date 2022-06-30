#ifndef DRK_GAME_SAVE_SAVE_VIEWER_HPP
#define DRK_GAME_SAVE_SAVE_VIEWER_HPP

#include "Core/Base.hpp"
#include "Game/Save/SaveData.hpp"

#include <string>

namespace DrkCraft::Game
{
    class SaveViewer
    {
    public:
        enum class Sort
        {
            Unsorted=0,
            RecentlySaved
        };

    friend class SaveManager;

    private:
        SaveViewer(Sort sort=Sort::Unsorted);

    public:
        bool is_loaded(void) const;

        auto begin(void) const { return m_saves.cbegin(); }
        auto end(void)  const  { return m_saves.cend(); }

    private:
        std::vector<SaveInfo*> m_saves;
    };
}

#endif // DRK_GAME_SAVE_SAVE_VIEWER_HPP
