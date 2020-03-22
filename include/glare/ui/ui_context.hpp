/******************************************************************************
 * Copyright © 2020 Kévin Lesénéchal <kevin.lesenechal@gmail.com>             *
 *                                                                            *
 * This file is part of the GLARE project, published under the terms of the   *
 * MIT license; see LICENSE file for more information.                        *
 ******************************************************************************/

#pragma once

namespace glare {

class Context;

class UiContext
{
public:
    explicit UiContext(Context& context);

    ~UiContext() noexcept;

    void frame_start();
    void frame_end();
};

} // ns glare
