#include "otoolstuff.h"
#include "stringutils.h"
#include "rbridge.h"
#include <fstream>
#include <boost/filesystem.hpp>
#include "boost/nowide/fstream.hpp"
#include "utils.h"
#include <iostream>

std::string _system(std::string cmd)
{
	const char *root, *relativePath;

	if (!rbridge_requestTempFileName("log", &root, &relativePath))
		throw std::runtime_error("Cannot open output file for separate R/System cmd!");

	std::string path = std::string(root) + "/" + relativePath;

	cmd += " > " + path + " 2>&1 ";

#ifdef WIN32
	cmd = '"' + cmd + '"'; // See: https://stackoverflow.com/questions/2642551/windows-c-system-call-with-spaces-in-command
#endif

	system(cmd.c_str());

	boost::nowide::ifstream readLog(path);
	std::stringstream out;

	if(readLog)
	{
		out << readLog.rdbuf();
		readLog.close();
	}

	return out.str();
}

// see https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define xstr(s) str(s)
#define str(s)  #s

void _moduleLibraryFixer(const std::string & moduleLibraryPath, bool printStuff)
{
	using namespace boost;

	filesystem::path	modLibpath	= Utils::osPath(moduleLibraryPath),
						rcppPath	= Utils::osPath(moduleLibraryPath + "/Rcpp");

	//I still think it is a bad plan to have Rcpp installed doubly, so lets remove it from the module directory if its there
	if(exists(rcppPath))
		remove_all(rcppPath);

#ifdef __APPLE__
	std::cout << "This is a mac so we are trying to fix the otool mess..." << std::endl;

	typedef filesystem::recursive_directory_iterator	recIt;

	for(recIt dir(modLibpath); dir != recIt(); dir++)
	{
		filesystem::path path = dir->path();
	

		//We only want files that have dylib or so as extension and don't have dSYM anywhere in the path (because those are some kind of debugsymbols)
		if(	! (	filesystem::is_regular_file(path)							&&
				(path.extension() == ".dylib" || path.extension() == ".so")	&&
				path.string().find("dSYM") == std::string::npos				))
			continue;

		if(printStuff)
			std::cout << "	Now checking and fixing otool paths for file '" << path.string() << "'." << std::endl;

		std::string libDir		= stringUtils::replaceBy(path.string(), " ", "\\ "),
					otoolCmd	= "otool -L " + libDir,
					otoolOut	= _system(otoolCmd);
		auto		otoolLines	= stringUtils::splitString(otoolOut, '\n');

		/*std::cout << "jaspRCPP_postProcessLocalPackageInstall used otool -L on " << libDir << " and found this output:" << std::endl;

		for(const auto & line : otoolLines)
			std::cout << line << std::endl;*/

		//ok otoolLines[1] represents the "id" of the lib but we do not need to change it because it probably points directly back to itself. The other lines however we should change

		for(size_t i=2; i<otoolLines.size(); i++)
		{
			std::string line = otoolLines[i];
			line = line.substr(0, line.find_first_of('('));
			stringUtils::trim(line);
			line = stringUtils::replaceBy(line, " ", "\\ ");

			//For all the libs of R we have the following startsWith we can check:
			const std::string libStart = "/Library/Frameworks/R.framework/Versions/";

			//For the JAGS stuff we need to replace like so:
			const std::map<std::string, std::string> replaceThese =
			{
				{	"/usr/local/lib/libjags.4.dylib",	"@executable_path/JAGS/libjags.4.dylib"														},
				{	"/usr/lib/libc++abi.dylib",			"@executable_path/../Frameworks/R.framework/Versions/3.6/Resources/lib/libc++abi.1.dylib"	},
				{	"/usr/lib/libc++.1.dylib",			"@executable_path/../Frameworks/R.framework/Versions/3.6/Resources/lib/libc++.1.dylib"		}
			};
			//This ought to be sort of mirrored in jasp-required-files/MacOS/Frameworks/create-framework.py

			auto install_name_tool_cmd = [&](const std::string & replaceThisLine, const std::string & withThisLine)
			{
				const std::string cmd = "install_name_tool -change " + replaceThisLine + " " + withThisLine + " " + libDir;

				if(printStuff)
					std::cout << cmd << std::endl;

				_system(cmd);
			};

			if(replaceThese.count(line) > 0)
			{
				install_name_tool_cmd(line, replaceThese.at(line));
			}
			if(stringUtils::startsWith(line, libStart))
			{
				install_name_tool_cmd(line, stringUtils::replaceBy("@executable_path/../Frameworks/R.framework/Versions/" + line.substr(libStart.size()), " ", "\\ "));
			}
			else if(stringUtils::startsWith(line, "/opt/") || stringUtils::startsWith(line, "/usr/local/"))
			{
				std::string baseName	= line.substr(line.find_last_of('/') == std::string::npos ? 0 : line.find_last_of('/') + 1),
							newLine		= stringUtils::replaceBy("@executable_path/../Frameworks/R.framework/Versions/"  xstr(CURRENT_R_VERSION) "/Resources/lib/" + baseName, " ", "\\ ");

				install_name_tool_cmd(line, newLine);
			}
		}
	}

#else
	std::cout << "This isn't a mac so we aren't trying to fix the otool mess..." << std::endl;
#endif
}