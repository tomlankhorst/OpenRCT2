/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../Cheats.h"
#include "../Context.h"
#include "../core/MemoryStream.h"
#include "../drawing/Drawing.h"
#include "../interface/Window.h"
#include "../localisation/Localisation.h"
#include "../localisation/StringIds.h"
#include "../peep/Staff.h"
#include "../windows/Intent.h"
#include "../world/Park.h"
#include "../world/Sprite.h"
#include "GameAction.h"

DEFINE_GAME_ACTION(StaffSetOrderAction, GAME_COMMAND_SET_STAFF_ORDER, GameActionResult)
{
private:
    uint16_t _spriteIndex;
    uint8_t _orderId;

public:
    StaffSetOrderAction()
    {
    }
    StaffSetOrderAction(uint16_t spriteIndex, uint8_t orderId)
        : _spriteIndex(spriteIndex)
        , _orderId(orderId)
    {
    }

    uint16_t GetActionFlags() const override
    {
        return GameAction::GetActionFlags() | GA_FLAGS::ALLOW_WHILE_PAUSED;
    }

    void Serialise(DataSerialiser & stream) override
    {
        GameAction::Serialise(stream);

        stream << DS_TAG(_spriteIndex) << DS_TAG(_orderId);
    }

    GameActionResult::Ptr Query() const override
    {
        if (_spriteIndex >= MAX_SPRITES)
        {
            return std::make_unique<GameActionResult>(
                GA_ERROR::INVALID_PARAMETERS, STR_NONE);
        }

        rct_peep* peep = GET_PEEP(_spriteIndex);
        if (peep->type != PEEP_TYPE_STAFF ||
            (peep->staff_type != STAFF_TYPE_HANDYMAN && peep->staff_type != STAFF_TYPE_MECHANIC))
        {
            log_warning("Invalid game command for sprite %u", _spriteIndex);
            return std::make_unique<GameActionResult>(GA_ERROR::INVALID_PARAMETERS, STR_NONE);
        }

        return std::make_unique<GameActionResult>();
    }

    GameActionResult::Ptr Execute() const override
    {
        rct_peep* peep = GET_PEEP(_spriteIndex);

        peep->staff_orders = _orderId;

        window_invalidate_by_number(WC_PEEP, _spriteIndex);
        auto intent = Intent(INTENT_ACTION_REFRESH_STAFF_LIST);
        context_broadcast_intent(&intent);

        auto res = std::make_unique<GameActionResult>();
        res->Position.x = peep->x;
        res->Position.y = peep->y;
        res->Position.z = peep->z;
        return res;
    }
};
