#pragma once

#include <iostream>
#include <string>
#include <tchar.h>
#include <sstream>

using namespace std;

#if defined(_UNICODE) || defined(UNICODE)
	# define LOCALE std::wcerr.imbue(std::locale("")); \
		 std::wcin.imbue(std::locale("")); \
		 std::wclog.imbue(std::locale("")); \
		 std::wcout.imbue(std::locale(""));
	# define _terr		std::wcerr
	# define _tin		std::wcin
	# define _tlog		std::wclog
	# define _tout		std::wcout
	# define _tstring		std::wstring

	# define _tostringstream	std::wostringstream
	# define _tistringstream	std::wistringstream
	# define _tstringbuf		std::wstringbuf
	# define _tstringstream		std::wstringstream

	#define to_tstring	std::to_wstring

	#define	_tios				std::wios
	#define	_tstreambuf		std::wstreambuf
	#define	_tistream			std::wistream

	#define	_tostream			std::wostream
	#define	_tiostream		std::wiostream

	#define	_tfilebuf			std::wfilebuf
	#define	_tifstream			std::wifstream
	#define	_tofstream		std::wofstream
	#define	_tfstream			std::wfstream
#else
	# define _terr		std::cerr
	# define _tin			std::cin
	# define _tlog		std::clog
	# define _tout		std::cout
	# define _tstring		std::string
	# define _tistringstream	std::istringstream
	# define _tstringbuf			std::stringbuf
	# define _tstringstream		std::stringstream

	#define to_tstring	std::to_string

	#define	_tios				std::ios
	#define	_tstreambuf		std::streambuf
	#define	_tistream			std::istream

	#define	_tostream			std::ostream
	#define	_tiostream		std::iostream

	#define	_tfilebuf			std::filebuf
	#define	_tifstream			std::ifstream
	#define	_tofstream		std::ofstream
	#define	_tfstream			std::fstream
#endif
