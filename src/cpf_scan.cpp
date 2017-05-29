#include <cprintf/internal/cpf_scan.h>
#include <cprintf/internal/cpf_srch.h>
#include <cprintf/internal/cpf_tconf.h>

#include <algorithm>
#include <cstdio>
#include <functional> //std::function
#include <stdlib.h>   /* atoi */

#define _CPF_TOKEN_PREFIX "$"

namespace _cprintf_
{

	/*
	  these apply in situations where a user may wish to write a string
	  of characters where the first is one which may mistakably be
	  interpreted as part of the attribute specified...

	  i.e cprintf("$rred")

	  in the above case does one wish to apply red to foreground or red to both
	  foregound and background?

	  to resolve this issue the following is used:

	  cprintf("$r`red");
	*/

#define _CPF_TOKEN_PREFIX "$"

	/*
	these apply in situations where a user may wish to write a string
	of characters where the first is one which may mistakably be
	interpreted as part of the attribute specified...
	i.e cprintf("$rred")
	in the above case does one wish to apply red to foreground or red to both
	foregound and background?
	to resolve this issue the following is used:
	cprintf("$r`red");
	*/

	const std::initializer_list<_cprintf_::unicode_string_t> _cprintf_::attr_esc_seqs = {
		L"`$", L"`r", L"`g",
		L"`b", L"`y", L"`m",
		L"`c", L"`w", L"`R",
		L"`G", L"`B", L"`Y",
		L"`M", L"`C", L"`W",
		L"`.", L"``", L"`?",
		L"`!", L"`~", L"`|",
		L"`f", L"`#", L"`l", /*...$bld -> $b`ld*/
	};

	const std::initializer_list<wchar_t> _cprintf_::std_fmt_specs = {
		'c', 'd', 'e',
		'E', 'f', 'F',
		'g', 'G', 'i',
		'o', 's', 'u',
		'x', 'X', 'a',
		'A', 'p', 'n',
		'b', 'S'
	};

	/*
	These are characters that terminate a format specifier:
	Characters typically found at the end of a more complex FS
	%#x
	%.6i
	%05.2f
	*/
	const std::initializer_list<wchar_t> _cprintf_::ext_fmtspec_terms = {
		'd', 'f', 's',
		'e', 'o', 'x',
		'X', 'i', 'u',
		'S'
	};

	/*
	These are characters that typically contained within i.e. in the middle of more
	complex format specifiers:
	%.6i
	%05.2f
	*/
	const std::initializer_list<wchar_t> _cprintf_::inter_fmt_specs = {
		'+', '-', '.',
		'*', '#', 'l'
	};

	const std::initializer_list<wchar_t> _cprintf_::escape_characters = {
		'\a', '\b', '\f',
		'\n', '\r', '\t',
		'\v', '\\', '\"',
		'\0'
	};

	auto col_ids = {
		'r', 'g', 'b',
		'y', 'm', 'c',
		'w', 'R', 'G',
		'B', 'Y', 'M',
		'C', 'W'
	};

	/*characters that may precede colour identifiers (r, g, b...)
	and '#' in the case of '*'..*/
	auto col_id_prefs = { '.' };

	/*specialised character identifiers*/
	auto schar_ids =
	{
		'!', '~', 'f',
		'b', '&', '#',
		'.', '?',	'|',
		'^'
	};


	/*
	  parse-predicate storage type
	*/
	typedef std::map<
		wchar_t,
		std::function<bool(unicode_string_t const&, std::int32_t const&)> >
		ppred_t;

	bool pred_isdigit(unicode_string_t const& s, std::int32_t const& p)
	{
		return isdigit(s[p]) ? true : false;
	}

	bool pred_scrn_clr(unicode_string_t const& s, std::int32_t const& p)
	{
		return s[p] == '!';
	}

	bool pred_colour(unicode_string_t const& s, std::int32_t const& p)
	{
		/*a colour char may only be preceded by another or any char contained in
		 * col_id_prefs...*/
		return (std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end()) ||
			(std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) !=
				col_id_prefs.end());
	}

	/*
	  map of predicates for conditional syntax checking based on encountered
	  characters
	  on parsing.

	  the returned value from all predicates signifies whether the encountered
	  syntax constitues valid (true) token specification with the format string
	  or not (false). Where a return value of false would, in [most] but not all
	  cases, result in parsing at a particular position in the format string being
	  recognised as finished.
	  By "position" it is meant the position at which a cprintf syntax token ($)
	  is encountered, incremented by 1 i.e $r, parsing start position in this case
	  would be at format-string index position of character 'r'.

	  note: the argument p accepted by all predicates signifies the positional index
	  preceding the character at the current search position. This "character"
	  is always the [key]
	*/
	const ppred_t parsing_predicates = {
		{ '!', pred_scrn_clr },/*screen wipe + reset position to 0,0 in console*/
		{ '~', pred_scrn_clr },/*screen wipe + retain current position -> !~ -> $!~...*/
		{ ',', pred_isdigit },
		{ 'f', pred_isdigit },
		{ '&', pred_isdigit },
		{ 'r', pred_colour },
		{ 'g', pred_colour },
		/*dim blue shall use a different predicate, see below*/
		{ 'y', pred_colour },
		{ 'm', pred_colour },
		{ 'c', pred_colour },
		{ 'w', pred_colour },
		{ 'R', pred_colour },
		{ 'G', pred_colour },
		{ 'B', pred_colour },
		{ 'Y', pred_colour },
		{ 'M', pred_colour },
		{ 'C', pred_colour },
		{ 'W', pred_colour },
		{ '?',
		  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
			return s[p] == '?' || s[p] == '.';
			} },
		{ '|',
		  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
			return s[p] == '|' || s[p] == '.';
			} },
		//{ '*', /*an asterisk can only be prefixed by colour identifiers*/
		//  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
		//	return std::find(col_ids.begin(), col_ids.end(), s[p]) !=
		//		   col_ids.end(); /*in r, g, b, ...*/
		//	;
		//	} },
		{ '.',
		  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
				// s[p] in r, g, b, ... or is digit or b f & etc
				return (std::find(col_ids.begin(), col_ids.end(), s[p]) !=
						col_ids.end()) ||
					   isdigit(s[p]) || (std::find(schar_ids.begin(), schar_ids.end(),
												   s[p]) != schar_ids.end());
				} },
			{ '#',
			  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
				return (std::find(col_ids.begin(), col_ids.end(), s[p]) !=
						col_ids.end()) ||
					   s[p] == '*';
				} },
			{ 'b',
			  [](unicode_string_t const& s, std::int32_t const& p) -> bool {
					/*note: this covers the colour blue (...$b[*]...) as well as a token
					representing
					a unix bitmap-colour token (...$34b...).
					in effect, what this means is that the blue colour token shall not be
					treated
					in a similar manner as the rest of the colours for reasons of possible
					ambiguity
					in parsing...*/
					return (isdigit(s[p]) ? true : false) ||
						   std::find(col_ids.begin(), col_ids.end(), s[p]) != col_ids.end() ||
						   (std::find(col_id_prefs.begin(), col_id_prefs.end(), s[p]) !=
							col_id_prefs.end());
					} }
	};

	void purge_meta_esc_sequences(format_string_layout_t& meta)
	{
		auto purge_str_esc_sequences = [&](unicode_string_t& src) {
			for (auto& es : attr_esc_seqs)
			{
				auto replacew = es.substr(1);

				/* searches "src" for "es" and replaces it with "replacew" */
				size_t pos = 0;
				while ((pos = search_for(es, src, pos)) !=
					unicode_string_t::npos)
				{
					src.replace(pos, es.length(), replacew);
					pos += replacew.length();
				}
			}
		};

		for (auto& e : meta)
		{
			purge_str_esc_sequences(e.second.second);
		}
	}

	/*
	  parses "src_string" starting from "ssp" (search start position) and returns an
	  offset via "offset_pos"
	  denoting the size of the substring that represents an attribute specifier.
	*/
	void parse_attribute_specifier(unicode_string_t const&      src_string,
		unicode_string_t::size_type& offset_val,
		unicode_string_t::size_type& ssp)
	{
		std::int32_t offset_counter = 0;
		wchar_t         c = src_string[ssp]; // first character after occurrance of "$"
		bool            finished = false,
			checked_if_is_txt_frmt_modifier = false; //$bld $rvs etc..
		ppred_t::const_iterator
			pred_iter; // colour and screen wipe parsing predicates iterator
		while (!finished)
		{
			/*we start with checking for text format specifiers first...
			note: this is only necessary on linux as thats the only platform
			supporting those txt attribute specifiers i.e $rvs $bld, $?bld*/
			auto off3 = (ssp + 3);
			auto off4 = (ssp + 4);
			auto lst_i = (src_string.size() - 1);
			if ((off4 < lst_i || off3 < lst_i) &&
				!checked_if_is_txt_frmt_modifier) // if offset does not exceed searched
												  // string's last character index...
			{
				auto f = [&](unicode_string_t const& s) {
					// faster by using backwards iteration (see initialisation of
					// std_tokens)
					if (std::find(std_tokens.rbegin(),
						std_tokens.rend(),
						s) != std_tokens.rend())
					{
						finished = true;
						return true;
					}
					return false;
				};

				/*order of if conditions is very important here*/
				if (f(src_string.substr(ssp, 4u)) == true)
				{
					offset_counter = 4;
				}
				else if (f(src_string.substr(ssp, 3u)) == true)
				{
					offset_counter = 3;
				}

				checked_if_is_txt_frmt_modifier = true;
			}
			else if ((pred_iter = parsing_predicates.find(c)) !=
				parsing_predicates.end())
			{
				do
				{
					auto off = ssp + (++offset_counter);

					/*on first iteration, take away only 1 from "off" since
					character compared to in predicate will not have any other preceding
					char but "$". All other comparisons involve comparing the 'current'
					char with the one preceding its position to verify syntax correction
					*/
					auto x = (offset_counter) == 1 ? 1 : 2;
					/*if character found does abide by parsing rules...*/
					if (pred_iter->second(src_string, off - x) == false)
					{
						finished = true;
						break;
					}

					c = src_string[off];

				} while ((pred_iter = parsing_predicates.find(c)) !=
					parsing_predicates.end());
			}
			else if (isdigit(c))
			{
				do
				{
					auto off = ssp + (++offset_counter);
					c = src_string[off];
				} while (isdigit(c));
			}
			else
			{
				finished = true;
			}

			/*
			  if possibly enter an infinite loop...
			  note that we simply use an arbitrary value
			*/
			if (offset_counter >= 50)
			{
				finished = true;
				offset_counter = 0;
			}
		}

		offset_val = offset_counter;
	}

	/**/
	unicode_string_t parse_fstr_for_attrib_specs(
		unicode_string_t const&      format_str_,
		unicode_string_t::size_type  search_start_pos_,
		unicode_string_t::size_type& attrib_end_pos)
	{
		auto ssp = search_start_pos_;

		/*offset from search start position i.e token occurance position in format
		 * string*/
		unicode_string_t::size_type offset_pos = 0;

		parse_attribute_specifier(format_str_, offset_pos, ssp);

		/*the parsed attribute(s) string...*/
		unicode_string_t attribute_string = format_str_.substr(ssp, offset_pos);

		if (attribute_string.size() == 0)
			throw CPF_FORMAT_STRING_TOKEN_ERROR; // invalid '$' token encountered on parse

		attrib_end_pos += offset_pos;
		return attribute_string;
	}

	format_string_layout_t parse_format_string(
		const unicode_string_t& src_format)
	{
		format_string_layout_t meta;

		const std::int32_t NUM_C_TAGS = [&]() -> decltype(NUM_C_TAGS) {
			std::int32_t             occurrences = 0;
			unicode_string_t::size_type start = 0;

			while ((start = search_for(L"$", src_format, start)) !=
				unicode_string_t::npos)
			{
				++occurrences;
				start++;
			}
			return occurrences;
		}();

		std::int32_t token_occurance_pos = 0; 
		std::int32_t attrib_endpos_p1 = 0;
		bool            first_iter = true;
		while ((token_occurance_pos = search_for(
			L"$", src_format, attrib_endpos_p1)) != src_format.npos)
		{
			if (first_iter && token_occurance_pos != 0)
			{
				unicode_string_vector_t default_attrib{ L"?" };
				meta.insert(std::make_pair(
					0u, std::make_pair(default_attrib,
						src_format.substr(0u, token_occurance_pos))));
				first_iter = false;
			}

			auto                        tokOccPos_1 = token_occurance_pos + 1;
			unicode_string_t::size_type off = 0;
			auto                        attibs_str =
				parse_fstr_for_attrib_specs(src_format, token_occurance_pos + 1, off);
			attrib_endpos_p1 = tokOccPos_1 + off;

			auto next_prefix_pos =
				search_for(L"$", src_format, attrib_endpos_p1);

			/*vector to hold attributes that are applied to the (sub) string proceeding
			the token "$"'s occurance position.*/
			unicode_string_vector_t subseq_str_attribs;
			unicode_string_t     current_attrib;

			for (auto c = std::begin(attibs_str); c != std::end(attibs_str); ++c)
			{
				if (*c == '.')
				{
					subseq_str_attribs.push_back(current_attrib);
					current_attrib.clear();
					continue;
				}
				current_attrib.append({ *c });
				// reached last character in string...
				if (std::distance(attibs_str.begin(), c) == (attibs_str.size() - 1))
				{
					subseq_str_attribs.push_back(current_attrib);
				}
			}

			/*the sub string to which the encountered attributes shall be applied*/
			auto meta_substr =
				src_format.substr(attrib_endpos_p1, (next_prefix_pos - attrib_endpos_p1));

			meta.insert({ token_occurance_pos, { subseq_str_attribs, meta_substr } });
		}

		if (meta.empty())
		{
			meta.insert(std::make_pair(
				0u, std::make_pair(unicode_string_vector_t({ L"?" }), src_format)));
		}

		purge_meta_esc_sequences(meta);

		return meta;
	}

}