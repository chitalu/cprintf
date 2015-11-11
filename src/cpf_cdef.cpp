#include <cprintf/internal/cpf_cdef.h>
#include <assert.h>

/*
        no harm in explicitly defining token string laterals here,
        using macros below is bad enough figuring out what is happening
        in the code.
*/
CPF_API const cpf::type::str_vec_t cpf::intern::std_tokens = {
  /*
          dim text colour no background
  */
  L"r", L"g", L"b", L"y", L"m", L"c", L"w",

  /*
          dim background
  */
  L"r#", L"g#", L"b#", L"y#", L"m#", L"c#", L"w#",

  /*
          bright text colour no background
  */
  L"R", L"G", L"B", L"Y", L"M", L"C", L"W",

  /*
          bright background
  */
  L"R#", L"G#", L"B#", L"Y#", L"M#", L"C#", L"W#",

  /*
          dim text and background colour
  */
  L"rr", L"rb", L"rg", L"ry", L"rm", L"rc", L"rw", /*red*/
  L"gg", L"gb", L"gr", L"gy", L"gm", L"gc", L"gw", /*green*/
  L"bb", L"br", L"bg", L"by", L"bm", L"bc", L"bw", /*blue*/
  L"yy", L"yb", L"yg", L"yr", L"ym", L"yc", L"yw", /*yellow*/
  L"mm", L"mr", L"mg", L"my", L"mb", L"mc", L"mw", /*magenta*/
  L"cc", L"cr", L"cg", L"cy", L"cm", L"cb", L"cw", /*cyan*/
  L"ww", L"wr", L"wg", L"wy", L"wm", L"wc", L"wb", /*white*/

  /*
          bright text colour and dim background colour
  */
  L"Rr", L"Rb", L"Rg", L"Ry", L"Rm", L"Rc", L"Rw", /*red*/
  L"Gg", L"Gb", L"Gr", L"Gy", L"Gm", L"Gc", L"Gw", /*green*/
  L"Bb", L"Br", L"Bg", L"By", L"Bm", L"Bc", L"Bw", /*blue*/
  L"Yy", L"Yb", L"Yg", L"Yr", L"Ym", L"Yc", L"Yw", /*yellow*/
  L"Mm", L"Mr", L"Mg", L"My", L"Mb", L"Mc", L"Mw", /*magenta*/
  L"Cc", L"Cr", L"Cg", L"Cy", L"Cm", L"Cb", L"Cw", /*cyan*/
  L"Ww", L"Wr", L"Wg", L"Wy", L"Wm", L"Wc", L"Wb", /*white*/

  /*
          dim text colour and bright background colour
  */
  L"rR", L"rB", L"rG", L"rY", L"rM", L"rC", L"rW", /*red*/
  L"gG", L"gB", L"gR", L"gY", L"gM", L"gC", L"gW", /*green*/
  L"bB", L"bR", L"bG", L"bY", L"bM", L"bC", L"bW", /*blue*/
  L"yY", L"yB", L"yG", L"yR", L"yM", L"yC", L"yW", /*yellow*/
  L"mM", L"mR", L"mG", L"mY", L"mB", L"mC", L"mW", /*magenta*/
  L"cC", L"cR", L"cG", L"cY", L"cM", L"cB", L"cW", /*cyan*/
  L"wW", L"wR", L"wG", L"wY", L"wM", L"wC", L"wB", /*white*/

  /*
          bright text and background colour
  */
  L"RR", L"RB", L"RG", L"RY", L"RM", L"RC", L"RW", /*red*/
  L"GG", L"GB", L"GR", L"GY", L"GM", L"GC", L"GW", /*green*/
  L"BB", L"BR", L"BG", L"BY", L"BM", L"BC", L"BW", /*blue*/
  L"YY", L"YB", L"YG", L"YR", L"YM", L"YC", L"YW", /*yellow*/
  L"MM", L"MR", L"MG", L"MY", L"MB", L"MC", L"MW", /*magenta*/
  L"CC", L"CR", L"CG", L"CY", L"CM", L"CB", L"CW", /*cyan*/
  L"WW", L"WR", L"WG", L"WY", L"WM", L"WC", L"WB"  /*white*/

#ifndef _WIN32
  /*
          linux only text attribute tokens:

          bold, dim, blink, reverse, hidden
  */
  ,
  L"bld", L"dim", L"uln", L"blk", L"rvs", L"hid", L"?bld", L"?dim", L"?uln",
  L"?blk", L"?rvs", L"?hid",
#endif /*#ifndef _WIN32*/
};

// The macro "REG_COLOUR_VALUES" is used to reduce code repetition. Though this
// does hinder readability a little. To understand what this macro expands to
// please make note of the fact that "cpf::intern::std_token_vals" is a
// std::map. The [key] is a string lateral for tokens i.e "r" or "bw*" The
// [value] represents an OS-specific value used to configure the system terminal
// i.e on windows this would be a value like "FOREGROUND_GREEN" and on linux it
// would be some arbtrary escape sequence corresponding to the [key] token.
// macro expansion is slightly different for Unix and Windows

#ifdef _WIN32

#define REG_COLOUR_VALUES(c, C, val_f, val_b)                                  \
  { L"" L##c L"#", (val_b) }                                                   \
  , { L"" L##C L"#", (val_b | CPF_bgi) }, { L"" L##c, (val_f) },               \
      { L"" L##c L"r", (val_f | CPF_Rb) },                                     \
      { L"" L##c L"b", (val_f | CPF_Bb) },                                     \
      { L"" L##c L"g", (val_f | CPF_Gb) },                                     \
      { L"" L##c L"y", (val_f | CPF_Yb) },                                     \
      { L"" L##c L"m", (val_f | CPF_Mb) },                                     \
      { L"" L##c L"c", (val_f | CPF_Cb) },                                     \
      { L"" L##c L"w", (val_f | CPF_Wb) }, { L"" L##C, (val_f | CPF_fgi) },    \
      { L"" L##C L"r", ((val_f | CPF_fgi) | CPF_Rb) },                         \
      { L"" L##C L"b", ((val_f | CPF_fgi) | CPF_Bb) },                         \
      { L"" L##C L"g", ((val_f | CPF_fgi) | CPF_Gb) },                         \
      { L"" L##C L"y", ((val_f | CPF_fgi) | CPF_Yb) },                         \
      { L"" L##C L"m", ((val_f | CPF_fgi) | CPF_Mb) },                         \
      { L"" L##C L"c", ((val_f | CPF_fgi) | CPF_Cb) },                         \
      { L"" L##C L"w", ((val_f | CPF_fgi) | CPF_Wb) },                         \
      { L"" L##c L"R", (val_f | CPF_Rbi) },                                    \
      { L"" L##c L"B", (val_f | CPF_Bbi) },                                    \
      { L"" L##c L"G", (val_f | CPF_Gbi) },                                    \
      { L"" L##c L"Y", (val_f | CPF_Ybi) },                                    \
      { L"" L##c L"M", (val_f | CPF_Mbi) },                                    \
      { L"" L##c L"C", (val_f | CPF_Cbi) },                                    \
      { L"" L##c L"W", (val_f | CPF_Wbi) },                                    \
      { L"" L##C L"R", ((val_f | CPF_fgi) | CPF_Rbi) },                        \
      { L"" L##C L"B", ((val_f | CPF_fgi) | CPF_Bbi) },                        \
      { L"" L##C L"G", ((val_f | CPF_fgi) | CPF_Gbi) },                        \
      { L"" L##C L"Y", ((val_f | CPF_fgi) | CPF_Ybi) },                        \
      { L"" L##C L"M", ((val_f | CPF_fgi) | CPF_Mbi) },                        \
      { L"" L##C L"M", ((val_f | CPF_fgi) | CPF_Mbi) },                        \
      { L"" L##C L"C", ((val_f | CPF_fgi) | CPF_Cbi) }, {                      \
    L"" L##c L"*W", ((val_f | CPF_fgi) | CPF_Wbi)                              \
  }

const std::map<const cpf::type::str_t, cpf::type::colour>
cpf::intern::std_token_vals{
  /*
          red
  */
  REG_COLOUR_VALUES("r", "R", CPF_Rf, CPF_Rb),

  /*
          green
  */
  REG_COLOUR_VALUES("g", "G", CPF_Gf, CPF_Gb),

  /*
          blue
  */
  REG_COLOUR_VALUES("b", "B", CPF_Bf, CPF_Bb),

  /*
          yellow
  */
  REG_COLOUR_VALUES("y", "Y", CPF_Yf, CPF_Yb),

  /*
          magenta
  */
  REG_COLOUR_VALUES("m", "M", CPF_Mf, CPF_Mb),

  /*
          cyan
  */
  REG_COLOUR_VALUES("c", "C", CPF_Cf, CPF_Cb),

  /*
          white
  */
  REG_COLOUR_VALUES("w", "W", CPF_Wf, CPF_Wb)
};

#else /*#ifdef _WIN32*/
// http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
// http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
// http://linuxgazette.net/issue65/padala.html
// http://misc.flogisoft.com/bash/tip_colors_and_formatting
// http://man7.org/linux/man-pages/man4/console_codes.4.html
// http://www.linuxquestions.org/questions/programming-9/get-cursor-position-in-c-947833/
// http://ascii-table.com/ansi-escape-sequences-vt-100.php
// http://invisible-island.net/xterm/ctlseqs/ctlseqs.html
// http://ispltd.org/mini_howto:ansi_terminal_codes

/*<ESC>[{attr};{fg};{bg}m*/
#define REG_COLOUR_VALUES(c, C, i)                                             \
  { L"" #c, L"\x1B[0;0;3" #i "m" }                                             \
  , { L"" #C "r", L"\x1B[0;9" #i ";41m" },                                     \
      { L"" #c "#", L"\x1B[0;0;4" #i "m" },                                    \
      { L"" #C "#", L"\x1B[0;30;10" #i "m" },                                  \
      { L"" #c "r", L"\x1B[0;3" #i ";41m" },                                   \
      { L"" #c "g", L"\x1B[0;3" #i ";42m" },                                   \
      { L"" #c "b", L"\x1B[0;3" #i ";44m" },                                   \
      { L"" #c "y", L"\x1B[0;3" #i ";43m" },                                   \
      { L"" #c "m", L"\x1B[0;3" #i ";45m" },                                   \
      { L"" #c "c", L"\x1B[0;3" #i ";46m" },                                   \
      { L"" #c "w", L"\x1B[0;3" #i ";47m" }, { L"" #C, L"\x1B[0;0;9" #i "m" }, \
      { L"" #C "g", L"\x1B[0;9" #i ";42m" },                                   \
      { L"" #C "b", L"\x1B[0;9" #i ";44m" },                                   \
      { L"" #C "y", L"\x1B[0;9" #i ";43m" },                                   \
      { L"" #C "m", L"\x1B[0;9" #i ";45m" },                                   \
      { L"" #C "c", L"\x1B[0;9" #i ";46m" },                                   \
      { L"" #C "w", L"\x1B[0;9" #i ";47m" },                                   \
      { L"" #c "R", L"\x1B[0;3" #i ";101m" },                                  \
      { L"" #c "G", L"\x1B[0;3" #i ";102m" },                                  \
      { L"" #c "B", L"\x1B[0;3" #i ";104m" },                                  \
      { L"" #c "Y", L"\x1B[0;3" #i ";103m" },                                  \
      { L"" #c "M", L"\x1B[0;3" #i ";105m" },                                  \
      { L"" #c "C", L"\x1B[0;3" #i ";106m" },                                  \
      { L"" #c "W", L"\x1B[0;3" #i ";107m" },                                  \
      { L"" #C "R", L"\x1B[0;9" #i ";101m" },                                  \
      { L"" #C "G", L"\x1B[0;9" #i ";102m" },                                  \
      { L"" #C "B", L"\x1B[0;9" #i ";104m" },                                  \
      { L"" #C "Y", L"\x1B[0;9" #i ";103m" },                                  \
      { L"" #C "M", L"\x1B[0;9" #i ";105m" },                                  \
      { L"" #C "C", L"\x1B[0;9" #i ";106m" }, {                                \
    L"" #C "W", L"\x1B[0;9" #i ";107m"                                         \
  }

extern const std::map<const cpf::type::str_t, cpf::type::colour>
cpf::intern::std_token_vals{

  /*attributes specifiers*/
  { L"bld", L"\x1B[1m" },
  { L"dim", L"\x1B[2m" },
  { L"uln", L"\x1B[4m" },
  { L"blk", L"\x1B[5m" },
  { L"rvs", L"\x1B[7m" },
  { L"hid", L"\x1B[8m" },
  { L"?bld", L"\x1B[21m" },
  { L"?dim", L"\x1B[22m" },
  { L"?uln", L"\x1B[24m" },
  { L"?blk", L"\x1B[25m" },
  { L"?rvs", L"\x1B[27m" },
  { L"?hid", L"\x1B[28m" },

  /*
          red
  */
  REG_COLOUR_VALUES(r, R, 1),

  /*
          green
  */
  REG_COLOUR_VALUES(g, G, 2),

  /*
          blue
  */
  REG_COLOUR_VALUES(b, B, 4),

  /*
          yellow
  */
  REG_COLOUR_VALUES(y, Y, 3),

  /*
          magenta
  */
  REG_COLOUR_VALUES(m, M, 5),

  /*
          cyan
  */
  REG_COLOUR_VALUES(c, C, 6),

  /*
          white
  */
  REG_COLOUR_VALUES(w, W, 7)
};

#endif /*#ifdef _WIN32*/

