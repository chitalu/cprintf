#include "colour_repr.h"

extern "C" const _cpf_types::string_vector _cpf_colour_tokens= {
	/*default*/
	"!",

	/*dim text colour no background*/
	"r", "g", "b", "y", "m", "c", "w", "0",

	/*bright text colour no background*/
	"r!", "g!", "b!", "y!", "m!", "c!", "w!", "0!",

	/*dim text and background colour*/
	"rr", "rb", "rg", "ry", "rm", "rc", "rw", "r0",/*red*/
	"gg", "gb", "gr", "gy", "gm", "gc", "gw", "g0",/*green*/
	"bb", "br", "bg", "by", "bm", "bc", "bw", "b0",/*blue*/
	"yy", "yb", "yg", "yr", "ym", "yc", "yw", "y0",/*yellow*/
	"mm", "mr", "mg", "my", "mb", "mc", "mw", "m0",/*magenta*/
	"cc", "cr", "cg", "cy", "cm", "cb", "cw", "c0",/*cyan*/
	"ww", "wr", "wg", "wy", "wm", "wc", "wb", "w0",/*white*/
	"00", "0r", "0g", "0y", "0m", "0c", "0b", "0w",/*black*/

	/*bright text colour and dim background colour*/
	"r!r", "r!b", "r!g", "r!y", "r!m", "r!c", "r!w", "r!0", 	/*red*/
	"g!g", "g!b", "g!r", "g!y", "g!m", "g!c", "g!w", "g!0",	/*green*/
	"b!b", "b!r", "b!g", "b!y", "b!m", "b!c", "b!w", "b!0",	/*blue*/
	"y!y", "y!b", "y!g", "y!r", "y!m", "y!c", "y!w", "y!0",	/*yellow*/
	"m!m", "m!r", "m!g", "m!y", "m!b", "m!c", "m!w", "m!0",	/*magenta*/
	"c!c", "c!r", "c!g", "c!y", "c!m", "c!b", "c!w", "c!0",	/*cyan*/
	"w!w", "w!r", "w!g", "w!y", "w!m", "w!c", "w!b", "w!0",	/*white*/
	"0!0", "0!r", "0!g", "0!y", "0!m", "0!c", "0!b", "0!w",/*black*/

	/*dim text colour and bright background colour*/
	"rr!", "rb!", "rg!", "ry!", "rm!", "rc!", "rw!", "r0!",/*red*/
	"gg!", "gb!", "gr!", "gy!", "gm!", "gc!", "gw!", "g0!",/*green*/
	"bb!", "br!", "bg!", "by!", "bm!", "bc!", "bw!", "b0!",/*blue*/
	"yy!", "yb!", "yg!", "yr!", "ym!", "yc!", "yw!", "y0!",	/*yellow*/
	"mm!", "mr!", "mg!", "my!", "mb!", "mc!", "mw!", "m0!",	/*magenta*/
	"cc!", "cr!", "cg!", "cy!", "cm!", "cb!", "cw!", "c0!",	/*cyan*/
	"ww!", "wr!", "wg!", "wy!", "wm!", "wc!", "wb!", "w0!",	/*white*/
	"00!", "0r!", "0g!", "0y!", "0m!", "0c!", "0b!", "0w!",/*black*/

	/*bright text and background colour*/
	"r!r!", "r!b!", "r!g!", "r!y!", "r!m!", "r!c!", "r!w!", "r!0!",/*red*/
	"g!g!", "g!b!", "g!r!", "g!y!", "g!m!", "g!c!", "g!w!", "g!0!",/*green*/
	"b!b!", "b!r!", "b!g!", "b!y!", "b!m!", "b!c!", "b!w!", "b!0!",/*blue*/
	"y!y!", "y!b!", "y!g!", "y!r!", "y!m!", "y!c!", "y!w!", "y!0!",/*yellow*/
	"m!m!", "m!r!", "m!g!", "m!y!", "m!b!", "m!c!", "m!w!", "m!0!",/*magenta*/
	"c!c!", "c!r!", "c!g!", "c!y!", "c!m!", "c!b!", "c!w!", "c!0!",/*cyan*/
	"w!w!", "w!r!", "w!g!", "w!y!", "w!m!", "w!c!", "w!b!", "w!0!",/*white*/
	"0!0!", "0!r!", "0!g!", "0!y!", "0!m!", "0!c!", "0!b!", "0!w!",/*black*/
};

#ifdef _WIN32

const std::map<const _cpf_types::_string_type_, _cpf_types::colour> _cpf_colour_token_vals{
	/*default*/
	{ "!", [&]()->_cpf_types::colour
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		auto a = csbi.wAttributes;
		return static_cast<_cpf_types::colour>(a % 16);
	}()
	},

	/*black*/

	{ "0", (_cpf_Rf) },
	{ "0!", (_cpf_Rf | _cpf_fgi) },

	{ "0r", (_cpf_Rf | _cpf_Rb) },
	{ "0b", (_cpf_Rf | _cpf_Bb) },
	{ "0g", (_cpf_Rf | _cpf_Gb) },
	{ "0y", (_cpf_Rf | _cpf_Rb | _cpf_Gb) },
	{ "0m", (_cpf_Rf | _cpf_Rb | _cpf_Bb) },
	{ "0c", (_cpf_Rf | _cpf_Gb | _cpf_Bb) },
	{ "0w", (_cpf_Rf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "0!r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ "0!b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ "0!g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ "0!y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "0!m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "0!c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "0!w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "0r!", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ "0b!", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ "0g!", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ "0y!", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "0m!", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "0c!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "0w!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "0!r!", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "0!b!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "0!g!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "0!y!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "0!m!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "0!c!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "0!w!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*red*/
	{ "r", (_cpf_Rf) },
	{ "r!", (_cpf_Rf | _cpf_fgi) },

	{ "rr", (_cpf_Rf | _cpf_Rb) },
	{ "rb", (_cpf_Rf | _cpf_Bb) },
	{ "rg", (_cpf_Rf | _cpf_Gb) },
	{ "ry", (_cpf_Rf | _cpf_Rb | _cpf_Gb) },
	{ "rm", (_cpf_Rf | _cpf_Rb | _cpf_Bb) },
	{ "rc", (_cpf_Rf | _cpf_Gb | _cpf_Bb) },
	{ "rw", (_cpf_Rf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "r!r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ "r!b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ "r!g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ "r!y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "r!m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "r!c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "r!w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "rr!", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ "rb!", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ "rg!", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ "ry!", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "rm!", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "rc!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "rw!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "r!r!", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "r!b!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "r!g!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "r!y!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "r!m!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "r!c!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "r!w!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*green*/

	{ "g", (_cpf_Gf) },
	{ "g!", (_cpf_Gf | _cpf_fgi) },

	{ "gr", (_cpf_Gf | _cpf_Rb) },
	{ "gb", (_cpf_Gf | _cpf_Bb) },
	{ "gg", (_cpf_Gf | _cpf_Gb) },
	{ "gy", (_cpf_Gf | _cpf_Rb | _cpf_Gb) },
	{ "gm", (_cpf_Gf | _cpf_Rb | _cpf_Bb) },
	{ "gc", (_cpf_Gf | _cpf_Gb | _cpf_Bb) },
	{ "gw", (_cpf_Gf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "g!r", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb) },
	{ "g!b", ((_cpf_Gf | _cpf_fgi) | _cpf_Bb) },
	{ "g!g", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb) },
	{ "g!y", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "g!m", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "g!c", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "g!w", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "gr!", ((_cpf_Gf) | (_cpf_Rb | _cpf_bgi)) },
	{ "gb!", ((_cpf_Gf) | (_cpf_Bb | _cpf_bgi)) },
	{ "gg!", (_cpf_Gf | (_cpf_Gb | _cpf_bgi)) },
	{ "gy!", (_cpf_Gf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "gm!", (_cpf_Gf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "gc!", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "gw!", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "g!r!", ((_cpf_Gf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "g!b!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "g!g!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "g!y!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "g!m!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "g!c!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "g!w!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*blue*/

	{ "b", (_cpf_Bf) },
	{ "b!", (_cpf_Bf | _cpf_fgi) },

	{ "br", (_cpf_Bf | _cpf_Rb) },
	{ "bb", (_cpf_Bf | _cpf_Bb) },
	{ "bg", (_cpf_Bf | _cpf_Gb) },
	{ "by", (_cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "bm", (_cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "bc", (_cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "bw", (_cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "b!r", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "b!b", ((_cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "b!g", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "b!y", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "b!m", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "b!c", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "b!w", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "br!", ((_cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "bb!", ((_cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "bg!", (_cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "by!", (_cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "bm!", (_cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "bc!", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "bw!", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "b!r!", ((_cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "b!b!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "b!g!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "b!y!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "b!m!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "b!c!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "b!w!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*yellow*/

	{ "y", (_cpf_Rf | _cpf_Gf) },
	{ "y!", ((_cpf_Rf | _cpf_Gf) | _cpf_fgi) },

	{ "yr", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb) },
	{ "yb", ((_cpf_Rf | _cpf_Gf) | _cpf_Bb) },
	{ "yg", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb) },
	{ "yy", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Gb) },
	{ "ym", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Bb) },
	{ "yc", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb) },
	{ "yw", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "y!r", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb) },
	{ "y!b", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Bb) },
	{ "y!g", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb) },
	{ "y!y", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "y!m", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "y!c", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "y!w", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "yr!", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Rb | _cpf_bgi)) },
	{ "yb!", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Bb | _cpf_bgi)) },
	{ "yg!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_bgi)) },
	{ "yy!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "ym!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "yc!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "yw!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "y!r!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "y!b!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "y!g!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "y!y!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "y!m!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "y!c!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "y!w!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*magenta*/

	{ "m", (_cpf_Rf | _cpf_Bf) },
	{ "m!", (_cpf_Rf | _cpf_Bf | _cpf_fgi) },

	{ "mr", (_cpf_Rf | _cpf_Bf | _cpf_Rb) },
	{ "mb", (_cpf_Rf | _cpf_Bf | _cpf_Bb) },
	{ "mg", (_cpf_Rf | _cpf_Bf | _cpf_Gb) },
	{ "my", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "mm", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "mc", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "mw", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "m!r", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "m!b", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "m!g", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "m!y", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "m!m", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "m!c", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "m!w", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "mr!", ((_cpf_Rf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "mb!", ((_cpf_Rf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "mg!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "my!", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "mm!", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "mc!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "mw!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "m!r!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "m!b!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "m!g!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "m!y!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "m!m!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "m!c!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "m!w!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*cyan*/

	{ "c", (_cpf_Gf | _cpf_Bf) },
	{ "c!", (_cpf_Gf | _cpf_Bf | _cpf_fgi) },

	{ "cr", (_cpf_Gf | _cpf_Bf | _cpf_Rb) },
	{ "cb", (_cpf_Gf | _cpf_Bf | _cpf_Bb) },
	{ "cg", (_cpf_Gf | _cpf_Bf | _cpf_Gb) },
	{ "cy", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "cm", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "cc", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "cw", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "c!r", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "c!b", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "c!g", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "c!y", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "c!m", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "c!c", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "c!w", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "cr!", ((_cpf_Gf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "cb!", ((_cpf_Gf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "cg!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "cy!", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "cm!", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "cc!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "cw!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "c!r!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "c!b!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "c!g!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "c!y!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "c!m!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "c!c!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "c!w!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	/*white*/

	{ "w", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) },
	{ "w!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) },

	{ "wr", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "wb", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "wg", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "wy", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "wm", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "wc", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "ww", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "w!r", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb) },
	{ "w!b", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Bb) },
	{ "w!g", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb) },
	{ "w!y", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "w!m", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "w!c", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "w!w", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "wr!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Rb | _cpf_bgi)) },
	{ "wb!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Bb | _cpf_bgi)) },
	{ "wg!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "wy!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "wm!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "wc!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "ww!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "w!r!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "w!b!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "w!g!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "w!y!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "w!m!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "w!c!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "w!w!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) }

};

#else /*#ifdef _WIN32*/
//http://www.linuxhomenetworking.com/forums/showthread.php/1095-Linux-console-Colors-And-Other-Trick-s
//http://stackoverflow.com/questions/3506504/c-code-changes-terminal-text-color-how-to-restore-defaults-linux
//http://linuxgazette.net/issue65/padala.html

extern const std::map<const _cpf_types::_string_type_, _cpf_types::colour> _cpf_colour_token_vals{
	/*<ESC>[{attr};{fg};{bg}m*/
	{	"0",	"\x1B[0;0;0m" },

	{	"r",	"\x1B[0;0;31m" },
	{	"rr",	"\x1B[0;31;41m" },
	{	"rg",	"\x1B[0;31;42m" },
	{	"rb",	"\x1B[0;31;44m" },
	{	"ry",	"\x1B[0;31;43m" },
	{	"rm",	"\x1B[0;31;45m" },
	{	"rc",	"\x1B[0;31;46m" },
	{	"rw",	"\x1B[0;31;47m" },

	{	"r!",	"\x1B[0;0;91m" },
	{	"r!r",	"\x1B[0;91;41m" },
	{	"r!g",	"\x1B[0;91;42m" },
	{	"r!b",	"\x1B[0;91;44m" },
	{	"r!y",	"\x1B[0;91;43m" },
	{	"r!m",	"\x1B[0;91;45m" },
	{	"r!c",	"\x1B[0;91;46m" },
	{	"r!w",	"\x1B[0;91;47m" },

	{	"rr!",	"\x1B[0;31;101m" },
	{	"rg!",	"\x1B[0;31;102m" },
	{	"rb!",	"\x1B[0;31;104m" },
	{	"ry!",	"\x1B[0;31;103m" },
	{	"rm!",	"\x1B[0;31;105m" },
	{	"rc!",	"\x1B[0;31;106m" },
	{	"rw!",	"\x1B[0;31;107m" },

	{	"r!r!",	"\x1B[0;91;101m" },
	{	"r!g!",	"\x1B[0;91;102m" },
	{	"r!b!",	"\x1B[0;31;104m" },
	{	"r!y!",	"\x1B[0;91;103m" },
	{	"r!m!",	"\x1B[0;91;105m" },
	{	"r!c!",	"\x1B[0;91;106m" },
	{	"r!w!",	"\x1B[0;91;107m" },

    {	"g",	"\x1B[0;0;32m" },
    {	"b",	"\x1B[0;0;34m" },
	{	"y",	"\x1B[0;0;33m" },
	{	"m",	"\x1B[0;0;35m" },
	{	"c",	"\x1B[0;0;36m" },
	{	"w",	"\x1B[0;0;37m" },
    
    {	"g!",	"\x1B[0;0;92m" },
    {	"b!",	"\x1B[0;0;94m" },
    {	"y!",	"\x1B[0;0;93m" },
	{	"m!",	"\x1B[0;0;95m" },
	{	"c!",	"\x1B[0;0;96m" },
	{	"w!",	"\x1B[0;0;97m" },

    /*default*/
	{ "!", "\x1B[0m"}
};
	/*black*/

	/*{ "0",  },
	{ "0!",  },

	{ "0r",  },
	{ "0b",  },
	{ "0g",  },
	{ "0y",  },
	{ "0m",  },
	{ "0c",  },
	{ "0w",  },*/
    /*
	{ "0!r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ "0!b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ "0!g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ "0!y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "0!m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "0!c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "0!w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "0r!", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ "0b!", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ "0g!", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ "0y!", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "0m!", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "0c!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "0w!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "0!r!", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "0!b!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "0!g!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "0!y!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "0!m!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "0!c!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "0!w!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//red
	{ "r", (_cpf_Rf) },
	{ "r!", (_cpf_Rf | _cpf_fgi) },

	{ "rr", (_cpf_Rf | _cpf_Rb) },
	{ "rb", (_cpf_Rf | _cpf_Bb) },
	{ "rg", (_cpf_Rf | _cpf_Gb) },
	{ "ry", (_cpf_Rf | _cpf_Rb | _cpf_Gb) },
	{ "rm", (_cpf_Rf | _cpf_Rb | _cpf_Bb) },
	{ "rc", (_cpf_Rf | _cpf_Gb | _cpf_Bb) },
	{ "rw", (_cpf_Rf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "r!r", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb) },
	{ "r!b", ((_cpf_Rf | _cpf_fgi) | _cpf_Bb) },
	{ "r!g", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb) },
	{ "r!y", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "r!m", ((_cpf_Rf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "r!c", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "r!w", ((_cpf_Rf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "rr!", ((_cpf_Rf) | (_cpf_Rb | _cpf_bgi)) },
	{ "rb!", ((_cpf_Rf) | (_cpf_Bb | _cpf_bgi)) },
	{ "rg!", (_cpf_Rf | (_cpf_Gb | _cpf_bgi)) },
	{ "ry!", (_cpf_Rf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "rm!", (_cpf_Rf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "rc!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "rw!", (_cpf_Rf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "r!r!", ((_cpf_Rf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "r!b!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "r!g!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "r!y!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "r!m!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "r!c!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "r!w!", ((_cpf_Rf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//green

	{ "g", (_cpf_Gf) },
	{ "g!", (_cpf_Gf | _cpf_fgi) },

	{ "gr", (_cpf_Gf | _cpf_Rb) },
	{ "gb", (_cpf_Gf | _cpf_Bb) },
	{ "gg", (_cpf_Gf | _cpf_Gb) },
	{ "gy", (_cpf_Gf | _cpf_Rb | _cpf_Gb) },
	{ "gm", (_cpf_Gf | _cpf_Rb | _cpf_Bb) },
	{ "gc", (_cpf_Gf | _cpf_Gb | _cpf_Bb) },
	{ "gw", (_cpf_Gf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "g!r", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb) },
	{ "g!b", ((_cpf_Gf | _cpf_fgi) | _cpf_Bb) },
	{ "g!g", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb) },
	{ "g!y", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "g!m", ((_cpf_Gf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "g!c", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "g!w", ((_cpf_Gf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "gr!", ((_cpf_Gf) | (_cpf_Rb | _cpf_bgi)) },
	{ "gb!", ((_cpf_Gf) | (_cpf_Bb | _cpf_bgi)) },
	{ "gg!", (_cpf_Gf | (_cpf_Gb | _cpf_bgi)) },
	{ "gy!", (_cpf_Gf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "gm!", (_cpf_Gf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "gc!", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "gw!", (_cpf_Gf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "g!r!", ((_cpf_Gf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "g!b!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "g!g!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "g!y!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "g!m!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "g!c!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "g!w!", ((_cpf_Gf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//blue

	{ "b", (_cpf_Bf) },
	{ "b!", (_cpf_Bf | _cpf_fgi) },

	{ "br", (_cpf_Bf | _cpf_Rb) },
	{ "bb", (_cpf_Bf | _cpf_Bb) },
	{ "bg", (_cpf_Bf | _cpf_Gb) },
	{ "by", (_cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "bm", (_cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "bc", (_cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "bw", (_cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "b!r", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "b!b", ((_cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "b!g", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "b!y", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "b!m", ((_cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "b!c", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "b!w", ((_cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "br!", ((_cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "bb!", ((_cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "bg!", (_cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "by!", (_cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "bm!", (_cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "bc!", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "bw!", (_cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "b!r!", ((_cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "b!b!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "b!g!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "b!y!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "b!m!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "b!c!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "b!w!", ((_cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//yellow

	{ "y", (_cpf_Rf | _cpf_Gf) },
	{ "y!", ((_cpf_Rf | _cpf_Gf) | _cpf_fgi) },

	{ "yr", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb) },
	{ "yb", ((_cpf_Rf | _cpf_Gf) | _cpf_Bb) },
	{ "yg", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb) },
	{ "yy", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Gb) },
	{ "ym", ((_cpf_Rf | _cpf_Gf) | _cpf_Rb | _cpf_Bb) },
	{ "yc", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb) },
	{ "yw", ((_cpf_Rf | _cpf_Gf) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "y!r", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb) },
	{ "y!b", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Bb) },
	{ "y!g", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb) },
	{ "y!y", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "y!m", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "y!c", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "y!w", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "yr!", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Rb | _cpf_bgi)) },
	{ "yb!", (((_cpf_Rf | _cpf_Gf)) | (_cpf_Bb | _cpf_bgi)) },
	{ "yg!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_bgi)) },
	{ "yy!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "ym!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "yc!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "yw!", ((_cpf_Rf | _cpf_Gf) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "y!r!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "y!b!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "y!g!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "y!y!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "y!m!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "y!c!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "y!w!", (((_cpf_Rf | _cpf_Gf) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//magenta

	{ "m", (_cpf_Rf | _cpf_Bf) },
	{ "m!", (_cpf_Rf | _cpf_Bf | _cpf_fgi) },

	{ "mr", (_cpf_Rf | _cpf_Bf | _cpf_Rb) },
	{ "mb", (_cpf_Rf | _cpf_Bf | _cpf_Bb) },
	{ "mg", (_cpf_Rf | _cpf_Bf | _cpf_Gb) },
	{ "my", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "mm", (_cpf_Rf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "mc", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "mw", (_cpf_Rf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "m!r", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "m!b", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "m!g", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "m!y", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "m!m", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "m!c", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "m!w", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "mr!", ((_cpf_Rf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "mb!", ((_cpf_Rf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "mg!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "my!", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "mm!", (_cpf_Rf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "mc!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "mw!", (_cpf_Rf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "m!r!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "m!b!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "m!g!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "m!y!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "m!m!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "m!c!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "m!w!", ((_cpf_Rf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//cyan

	{ "c", (_cpf_Gf | _cpf_Bf) },
	{ "c!", (_cpf_Gf | _cpf_Bf | _cpf_fgi) },

	{ "cr", (_cpf_Gf | _cpf_Bf | _cpf_Rb) },
	{ "cb", (_cpf_Gf | _cpf_Bf | _cpf_Bb) },
	{ "cg", (_cpf_Gf | _cpf_Bf | _cpf_Gb) },
	{ "cy", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Gb) },
	{ "cm", (_cpf_Gf | _cpf_Bf | _cpf_Rb | _cpf_Bb) },
	{ "cc", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb) },
	{ "cw", (_cpf_Gf | _cpf_Bf | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "c!r", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "c!b", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "c!g", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "c!y", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "c!m", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "c!c", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "c!w", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "cr!", ((_cpf_Gf | _cpf_Bf) | (_cpf_Rb | _cpf_bgi)) },
	{ "cb!", ((_cpf_Gf | _cpf_Bf) | (_cpf_Bb | _cpf_bgi)) },
	{ "cg!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_bgi)) },
	{ "cy!", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "cm!", (_cpf_Gf | _cpf_Bf | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "cc!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "cw!", (_cpf_Gf | _cpf_Bf | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "c!r!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "c!b!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "c!g!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "c!y!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "c!m!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "c!c!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "c!w!", ((_cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	//white

	{ "w", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) },
	{ "w!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) },

	{ "wr", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb) },
	{ "wb", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Bb) },
	{ "wg", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb) },
	{ "wy", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "wm", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "wc", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "ww", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "w!r", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb) },
	{ "w!b", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Bb) },
	{ "w!g", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb) },
	{ "w!y", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Gb) },
	{ "w!m", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Rb | _cpf_Bb) },
	{ "w!c", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb) },
	{ "w!w", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | _cpf_Gb | _cpf_Bb | _cpf_Rb) },

	{ "wr!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Rb | _cpf_bgi)) },
	{ "wb!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi)) | (_cpf_Bb | _cpf_bgi)) },
	{ "wg!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "wy!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "wm!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "wc!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "ww!", ((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) },

	{ "w!r!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_bgi | _cpf_Rb)) },
	{ "w!b!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Bb | _cpf_bgi)) },
	{ "w!g!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_bgi)) },
	{ "w!y!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Gb | _cpf_bgi)) },
	{ "w!m!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Rb | _cpf_Bb | _cpf_bgi)) },
	{ "w!c!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_bgi)) },
	{ "w!w!", (((_cpf_Rf | _cpf_Gf | _cpf_Bf | _cpf_fgi) | _cpf_fgi) | (_cpf_Gb | _cpf_Bb | _cpf_Rb | _cpf_bgi)) }
*/
//};

#endif /*#ifdef _WIN32*/
