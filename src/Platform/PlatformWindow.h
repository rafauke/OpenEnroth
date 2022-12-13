#pragma once

#include <string>
#include <memory>

#include "Utility/Geometry/Size.h"
#include "Utility/Geometry/Point.h"
#include "Utility/Geometry/Margins.h"

#include "PlatformOpenGLOptions.h"

class PlatformOpenGLContext;

 /**
  * Abstracted generic window modes.
  *
  * WINDOWED is a normal window with frames.
  * WINDOWED_BORDERLESS is a window without frames.
  * FULLSCREEN is a traditional exclusive fullscreen mode which changes display resolution and make window sole user of display.
  * FULLSCREEN_BORDERLESS is a modern fake fullscreen AKA frameless window resized to display resolution and it's top-left corner moved to 0,0 position.
  */
enum class WindowMode {
    WINDOWED = 0,
    WINDOWED_BORDERLESS = 1,
    FULLSCREEN = 2,
    FULLSCREEN_BORDERLESS = 3,
};
using enum WindowMode;

/**
 * Abstraction for accessing platform-specific window API.
 *
 * This class is essentially an API handle and doesn't hold any state. Windows users can think about it as
 * a thin wrapper atop of `HWND`, so a name like `PlatformWindowApiHandle` might've been more suitable here.
 * Fortunately we're not writing in Java and prefer shorter names.
 *
 * Event processing & state storage can be done by subclassing `PlatformEventHandler` and then using it when calling
 * into `PlatformEventLoop`.
 */
class PlatformWindow {
 public:
    virtual ~PlatformWindow() = default;

    virtual void SetTitle(const std::string &title) = 0;
    virtual std::string Title() const = 0;

    virtual void Resize(const Sizei &size) = 0;
    virtual Sizei Size() const = 0;

    virtual void SetPosition(const Pointi &pos) = 0;
    virtual Pointi Position() const = 0;

    virtual void SetVisible(bool visible) = 0;
    virtual bool IsVisible() const = 0;

    virtual void SetMode(WindowMode mode) = 0;
    virtual WindowMode GetMode() = 0;

    virtual void SetGrabsMouse(bool grabsMouse) = 0;
    virtual bool GrabsMouse() const = 0;

    virtual Marginsi FrameMargins() const = 0;

    virtual uintptr_t SystemHandle() const = 0;

    virtual void Activate() = 0;

    virtual std::unique_ptr<PlatformOpenGLContext> CreateOpenGLContext(const PlatformOpenGLOptions &options) = 0;

    // TODO(captainurist): compat methods, drop
    int GetWidth() const {
        return Size().w;
    }

    int GetHeight() const {
        return Size().h;
    }
};