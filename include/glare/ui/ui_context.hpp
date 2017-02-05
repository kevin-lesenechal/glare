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
