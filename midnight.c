#include <yed/plugin.h>

#define ALT(rgb, _256)     (tc ? (rgb) : (_256))

#define base00             (ALT(RGB_32_hex(171831), 16))
#define base00_but_lighter (ALT(RGB_32_hex(31324b), 235))

#define ORANGE             (ALT(RGB_32_hex(3c3836), 208))

#define YELLOW             (ALT(RGB_32_hex(ffc24b), 214))

#define YELLOW2            (ALT(RGB_32_hex(ffcc00), 220))

#define GREEN              (ALT(RGB_32_hex(b4ce5c), 119))

#define CYAN               (ALT(RGB_32_hex(50eafa), 80))

#define BLUE               (ALT(RGB_32_hex(32b4ff), 33))

#define RED                (ALT(RGB_32_hex(ff3c41), 160))

#define CRIMSONRED         (ALT(RGB_32_hex(ff3c41), 160))

#define TEAL               (ALT(RGB_32_hex(23d4ac), 37))

#define PURPLE             (ALT(RGB_32_hex(3a208a), 99))

#define WEAKPURPLE         (ALT(RGB_32_hex(a884f3), 98))

#define BEAUTIFULBLUE      (ALT(RGB_32_hex(5ac8fa), 75))

#define WHITE              (ALT(RGB_32_hex(e3e4fd), 189))

#define base0A             (ALT(RGB_32_hex(fabd2f), 214))

#define base0B             (ALT(RGB_32_hex(b8bb26), 142))

#define base0C             (ALT(RGB_32_hex(8ec07c), 108))

#define base0D             (ALT(RGB_32_hex(83a598), 109))

#define base0E             (ALT(RGB_32_hex(d3869b), 175))

#define DARK               (ALT(RGB_32_hex(000111), 239))

PACKABLE_STYLE(midnight)
{
    yed_style s;
    int       tc, attr_kind;

    YED_PLUG_VERSION_CHECK();

    tc        = !!yed_get_var("truecolor");
    attr_kind = tc ? ATTR_RGB : ATTR_256;

    memset(&s, 0, sizeof(s));

    s.active.flags        = attr_kind;
    s.active.fg           = WHITE;
    s.active.bg           = base00_but_lighter;

    s.inactive.flags      = attr_kind;
    s.inactive.fg         = WHITE;
    s.inactive.bg         = base00;

    s.active_border       = s.active;
    s.active_border.fg    = ORANGE;

    s.inactive_border     = s.inactive;

    s.cursor_line.flags   = attr_kind;
    s.cursor_line.fg      = WHITE;
    s.cursor_line.bg      = base00_but_lighter;

    s.search.flags        = attr_kind | ATTR_BOLD;
    s.search.fg           = DARK;
    s.search.bg           = BLUE;

    s.search_cursor.flags = attr_kind | ATTR_BOLD;
    s.search_cursor.fg    = WHITE;
    s.search_cursor.bg    = CRIMSONRED;

    s.selection.flags     = attr_kind;
    s.selection.fg        = WHITE;
    s.selection.bg        = DARK;

    s.attention.flags     = attr_kind | ATTR_BOLD;
    s.attention.fg        = RED;

    s.associate.flags     = attr_kind | ATTR_BOLD;
    s.associate.bg        = DARK;

    s.command_line        = s.active;

    s.status_line.flags   = attr_kind | ATTR_BOLD;
    s.status_line.fg      = s.active.fg;
    s.status_line.bg      = base00;

    s.active_gutter       = s.active;
    s.inactive_gutter     = s.inactive;

    s.code_comment.flags  = attr_kind;
    s.code_comment.fg     = WEAKPURPLE;

    s.code_keyword.flags  = attr_kind | ATTR_BOLD;
    s.code_keyword.fg     = TEAL;

    s.code_control_flow = s.code_typename = s.code_keyword;

    s.code_preprocessor.flags             = attr_kind | ATTR_BOLD;
    s.code_preprocessor.fg                = PURPLE;

    s.code_fn_call.flags                  = attr_kind | ATTR_BOLD;
    s.code_fn_call.fg                     = YELLOW;

    s.code_number.flags                   = attr_kind;
    s.code_number.fg                      = PURPLE;

    s.code_constant.flags                 = ATTR_RGB;
    s.code_constant.fg                    = ORANGE;

    s.code_string.flags                   = attr_kind;
    s.code_string.fg                      = GREEN;

    s.code_character                      = s.code_string;

    yed_plugin_set_style(self, "midnight", &s);

    return 0;
}
