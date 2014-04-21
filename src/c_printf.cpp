#include "c_printf.h"

const char* normalize_arg(const	std::string& arg) 
{
	return	arg.c_str();
}

void check_printf(const char* format) 
{
	for	(; *format; ++format) 
	{
		if (*format != '%' || *++format == '%')
		{
			continue;
		}
		throw std::invalid_argument("bad format specifier");
	}
}

meta_format_t _parse_formatter(const char* _formatter)
{
	auto extract_color_repr = [&](std::string fstr, std::size_t start_pos)->std::string
	{
		std::size_t offset, base;

		offset = base = (start_pos + 2);
		while (fstr[offset] != '}')	{	offset++; }
		assert(offset != fstr.npos);

		return fstr.substr(base, (offset - base));
	};
	
	meta_format_t meta;
	std::size_t pos;
	std::string subseq_colored_str, 
				color_repr, 
				formatter, 
				tag;

	formatter = _formatter;
	tag = "${";

	pos = formatter.find(tag, 0);

	if (pos == formatter.npos)
	{
		meta.push_back(std::make_pair(std::string("s"), formatter));
		return meta;
	}
	else
	{
		meta.push_back(std::make_pair(std::string("s"), formatter.substr(0, pos)));
	}

	color_repr = extract_color_repr(formatter, pos);
	/*plus 1 is for ")", to complete the symbolic color tag's size i.e ${s}*/
	pos += (tag.size() + (color_repr.size()) + 1);

	auto _p = formatter.find(tag, pos);
	if (_p != formatter.npos){	_p -= pos;	}

	auto offpos_insert_meta = [&](std::size_t count)->void
	{
		subseq_colored_str = formatter.substr(pos, count);
		meta.push_back(std::make_pair(color_repr, subseq_colored_str));
	};

	offpos_insert_meta(_p);

	while (pos != std::string::npos)
	{
		pos = formatter.find(tag, pos + 1);
		color_repr = extract_color_repr(formatter, pos);
		pos += (tag.size() + (color_repr.size()) + 1);

		_p = formatter.find(tag, pos);

		if (_p >= formatter.npos)
		{
			offpos_insert_meta(formatter.npos);		
			break;
		}

		if (_p != formatter.npos){	_p -= pos;	}

		offpos_insert_meta(_p);
	}

	return meta;
}