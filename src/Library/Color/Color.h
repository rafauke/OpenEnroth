#pragma once

#include <cstdint>
#include <cstring>

#include "Utility/Format.h"

namespace fmt {} // Make Doxygen happy.

namespace detail {
template<class Color>
struct ColorTag {
    Color color;
};
} // namespace detail

/**
 * Color in A8B8G8R8 format.
 */
struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    constexpr Color() = default;
    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255): r(r), g(g), b(b), a(a) {}

    static constexpr Color fromC16(uint16_t color) {
        // 16-bit zero was used as a marker for 'default color', but in new code that marker is Color(), so we have
        // to special-case the old behavior.
        if (color == 0)
            return Color();

        Color result;
        result.b = (color & 31) * 8;
        result.g = ((color >> 5) & 63) * 4;
        result.r = ((color >> 11) & 31) * 8;
        result.a = 255;
        return result;
    }

    [[nodiscard]] uint32_t c32() const {
        uint32_t result;
        memcpy(&result, this, 4);
        return result;
    }

    /**
     * This function returns a tag that can then be used in `fmt::format` calls to print out a color introducer.
     *
     * For example:
     * ```
     * std::string s = fmt::format("{::}{}{::} more text", Color(255, 255, 255).tag(), "text", Color().tag());
     * ```
     *
     * The code above will set string `s` to `"\fFFFFFtext\f00000 more text"`.
     *
     * Note that the only supported format specifier for color tags is `"{::}"`. This is done intentionally so that
     * the user won't accidentally mix up color introducers with other `fmt::format` args.
     *
     * @return                          Tag that can be used with `fmt::format`.
     */
    [[nodiscard]] constexpr detail::ColorTag<Color> tag() const {
        return {*this};
    }

    friend bool constexpr operator==(const Color &l, const Color &r) = default;
};
static_assert(sizeof(Color) == 4);
static_assert(alignof(Color) == 1);


using ColorTag = detail::ColorTag<Color>;


template<>
struct fmt::formatter<ColorTag> {
    constexpr auto parse(format_parse_context &ctx) {
        // Require {::} format spec. What's special about it is that it will fail for other types, so can only be used
        // for ColorTag. Also makes it easier to distinguish color tags in the format string.
        auto pos = ctx.begin();
        auto end = ctx.end();
        if (pos == end || *pos != ':' || ++pos == end || *pos != '}')
            fmt::detail::throw_format_error("ColorTag needs {::} format specifier");
        return pos; // pos points to '}'
    }

    auto format(const ColorTag &tag, format_context &ctx) const {
        return fmt::format_to(ctx.out(), "\f{:05}", toC16(tag.color));
    }

 private:
    static uint16_t toC16(const Color &color) {
        return (color.b >> (8 - 5)) | (0x7E0 & (color.g << (6 + 5 - 8))) | (0xF800 & (color.r << (6 + 5 + 5 - 8)));
    }
};
