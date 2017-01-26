/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
 *
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: CSmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j2b 17-January-2017
 *
 *******************************************************************************/

//issues, need to flip vertically


#include "CSmain.hpp"

#ifndef LINUX
	#include <windows.h>
#endif


static char errmessage[] = "Usage:  CS.exe [options]"
"\n"
"\n\twhere options are any of the following"
"\n"
"\n\t-oldr [string]          : code structure display .ldr output filename (def: codeStructureNet.ldr)"
"\n\t-osvg [string]          : code structure display .svg output filename (def: codeStructureNet.svg)"
"\n\t-oppm [string]          : code structure display .ppm output filename (def: codeStructureNet.ppm)"
"\n\t-ohtml [string]         : code structure display .html output filename (def: codeStructureNet.html) (use single file, else HTML file names are auto generated on a per C file basis)"
"\n\t-oall [string]          : code structure display .svg/.ldr/.ppm default generic output filename (def: codeStructureNet)"
"\n\t-file [string]          : top level source file name (eg, main.c) [compulsory]"
"\n\t-function [string]      : top level function name referenced within file {recommended: not defined in file, declared within include h file} (eg, x for int x()) [compulsory]"
"\n\t-notshow                : do not display output in opengl"
"\n\t-width [int]            : raster graphics width in pixels (def: 1600)"
"\n\t-height [int]           : raster graphics height in pixels (def: 1000)"
"\n\t-enablefunctions        : output function connectivity on top of file connectivity"
"\n\t-disablefileconnections : disable output of file connections"
"\n\t-mode [int]             : mode (1: execution flow, 2: data flow, 3: filter code based upon preprocessor definitions) (def: 1)"
"\n\t-trace                  : trace a function's usage upwards"
"\n\t-tracefunction [string] : bottom level function name to trace upwards / document (eg, y for int y())"
"\n\t-html                   : generate html documentation (user must specify tracefunction, else will document all functions)"
#ifdef CS_GENERATE_CPP_CLASSES
"\n\t-generateoo             : generate object oriented C++ code (must specify tempfolder else input files will be overwritten)"
#endif
#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
"\n\t-generateconst          : generate const function arguments code (must specify tempfolder else input files will be overwritten)"
#endif
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables; CS.exe (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-version                : print version"
"\n"
"\n\tThe program generates vector graphics based upon code structure of a given function in the C/C++ source directory."
"\n"
"\n";

static char knownLimitationsMsg[] = "* CS is designed for non-object oriented C (it does not support C++ classes)"
"\n* all .c and .h files that wish to be parsed/added to tree must be contained in the same directory"
"\n* if the CS program does not stop, there might be loops in the include file structure (eg a.c includes b.c, b.c includes a.c)"
"\n* CS supports c/c++ style commenting"
"\n* function definitions in .c files must not have leading white space, and should be contained on a single line"
"\n* function definitions in .c files must end with a } without any leading white space"
"\n* function declarations for functions that wish to be parsed/added to tree must be contained in .h files, can have preceding white space, but must be contained on a single line"
"\n* CS does not support relative paths in #include"
"\n* CS requires include/header files that wish to be parsed/added to tree to be delimited with \" \" rather than < >"
"\n* CS requires a single space between #include and \"this.h\""
"\n* CS may produce large SVG files (Eg when functions are enabled via enablefunctions) which must be viewed with a viewer capable of dynamic zoom, eg, inkscape"
"\n* CS is not designed to operate with function pointers (and object orientated code)"
"\n* the first include file in the top level source file (eg PROJECTmain.c) must declare the top level function name (eg main)"
"\n* make sure the temp folder is clear of all output files (ie svg/html files of the same name as expected output files)"
"\n* function contents cannot include a reference to themselves in comments (required for HTML generation function reference list and generateoo)"
"\n* function contents cannot include a reference to themselves in cout statements (e.g. cout << \"dothis()\")"
"\n* function contents cannot include a reference to their name in cout statements followed by an equals sign, unless it is referenced at the start of the comment (e.g. cout << \"generateHTMLdocumentationMode = \" ...)"
"\n* CS does not support 2 identical function declarations (with identical arguments) for a single function in a header file (separated by preprocessor definitions)"
"\n* CS does not support 2 function declarations with different arguments for a single function in a header file (separated by preprocessor defintions)"
"\n* CS does not support overloaded functions with the same number of arguments (required for precise referencing)";



int main(const int argc, const char** argv)
{
#ifdef CS_CONVERT_INDENTED_LIST_TO_HTML_LIST
	CSgenerateHTMLdocumentationClass().convertIndentedListToHTMLlist();
#elif defined CS_GENERATE_CLASS_HTML_DOCUMENTATION_FROM_CUSTOM_CSCLASS_FORMAT
	CSgenerateHTMLdocumentationClass().generateClassHTMLdocumentationFromCustomCSclassFormat();
#else

	bool useOutputLDRfile = false;
	string outputLDRfileName = "codeStructureNet.ldr";

	bool useOutputPPMfile = false;
	string outputPPMfileName = "codeStructureNet.ppm";

	bool useOutputSVGFile = false;
	string outputSVGfileName = "codeStructureNet.svg";

	bool useOutputHTMLfile = false;
	string outputHTMLfileName = "codeStructureNet.html";

	bool useOutputAllFile = false;
	string outputAllFileName = "codeStructureNet";

	bool useTopLevelFile = false;
	string topLevelFileName = "main.cpp";

	bool useTopLevelFunction = false;
	string topLevelFunctionName = "main";

	bool useBottomLevelFunctionNameToTraceUpwards = false;
	string bottomLevelFunctionNameToTraceUpwards = "";

	bool printOutput = false;
	bool displayInOpenGLAndOutputScreenshot = true;

	int mode = CS_MODE_OUTPUT_EXECUTION_FLOW;	//1. output execution flow, 2. output data flow, 3. filter code based upon preprocessor definitions

	int generateHTMLdocumentationMode = CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF;

	//#ifdef CS_GENERATE_CPP_CLASSES
	bool generateOOcode = false;
	//#endif
	//#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	bool generateConstFunctionArgumentsCode = false;
	//#endif

	bool passInputReq = true;
	bool outputFunctionsConnectivity = false;
	bool traceFunctionUpwards = false;

	int rasterImageWidth = 1600;
	int rasterImageHeight = 1000;

	bool outputFileConnections = true;

	if(SHAREDvarsClass().argumentExists(argc, argv, "-mode"))
	{
		mode = SHAREDvarsClass().getFloatArgument(argc, argv, "-mode");
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-oldr"))
	{
		outputLDRfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oldr");
		useOutputLDRfile = true;
		printOutput = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-oppm"))
	{
		outputPPMfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oppm");
		useOutputPPMfile = true;
		printOutput = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-osvg"))
	{
		outputSVGfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-osvg");
		useOutputSVGFile = true;
		printOutput = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-ohtml"))
	{
		outputHTMLfileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ohtml");
		useOutputHTMLfile = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-oall"))
	{
		outputAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oall");
		useOutputAllFile = true;
		printOutput = true;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-file"))
	{
		topLevelFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-file");
	}
	else
	{
		passInputReq = false;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-function"))
	{
		topLevelFunctionName = SHAREDvarsClass().getStringArgument(argc, argv, "-function");
	}
	else
	{
		passInputReq = false;
	}




	if(SHAREDvarsClass().argumentExists(argc, argv, "-notshow"))
	{
		displayInOpenGLAndOutputScreenshot = false;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-width"))
	{
		rasterImageWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-width");
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-height"))
	{
		rasterImageHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-height");
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-enablefunctions"))
	{
		outputFunctionsConnectivity = true;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-disablefileconnections"))
	{
		outputFileConnections = false;
	}

	if(SHAREDvarsClass().argumentExists(argc, argv, "-trace"))
	{
		if(outputFunctionsConnectivity)
		{
			traceFunctionUpwards = true;

		}
		else
		{
			cout << "error: -trace declared without -enablefunctions" << endl;
			passInputReq = false;
		}
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-tracefunction"))
	{
		bottomLevelFunctionNameToTraceUpwards = SHAREDvarsClass().getStringArgument(argc, argv, "-tracefunction");
	}


	if(SHAREDvarsClass().argumentExists(argc, argv, "-html"))
	{
		generateHTMLdocumentationMode = CS_GENERATE_HTML_DOCUMENTATION_MODE_ON;
		//SHAREDvarsClass().getFloatArgument(argc, argv, "-html");
	}

	#ifdef CS_GENERATE_CPP_CLASSES
	if(SHAREDvarsClass().argumentExists(argc, argv, "-generateoo"))
	{
		generateOOcode = true;
	}
	#endif
	#ifdef CS_GENERATE_CONST_FUNCTION_ARGUMENTS
	if(SHAREDvarsClass().argumentExists(argc, argv, "-generateconst"))
	{
		generateConstFunctionArgumentsCode = true;
	}
	#endif

	string currentFolder = SHAREDvarsClass().getCurrentDirectory();

	if(SHAREDvarsClass().argumentExists(argc, argv, "-workingfolder"))
	{
		workingFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-workingfolder");
	}
	else
	{
		workingFolder = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-exefolder"))
	{
		exeFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-exefolder");
	}
	else
	{
		exeFolder = currentFolder;
	}
	if(SHAREDvarsClass().argumentExists(argc, argv, "-tempfolder"))
	{
		tempFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-tempfolder");
	}
	else
	{
		tempFolder = currentFolder;
	}

	SHAREDvarsClass().setCurrentDirectory(workingFolder);

	if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
	{
		cout << "CS.exe - Project Version: 3j2b 17-January-2017" << endl;
		exit(1);
	}

	if(!passInputReq)
	{
		printf(errmessage);
		cout << "**** Known Limitations: ****" << endl;
		printf(knownLimitationsMsg);
		cout << "****************************" << endl;
		exit(0);
	}


	if(printOutput)
	{
		if(!useOutputLDRfile)
		{
			if(useOutputAllFile || displayInOpenGLAndOutputScreenshot)		//LDR output is always required when displaying semantic network in OpenGL and outputing screenshot
			{
				useOutputLDRfile = true;
				outputLDRfileName = outputAllFileName + ".ldr";
			}
		}
		if(!useOutputSVGFile)
		{
			useOutputSVGFile = true;	//SVG output is always required when printing/drawing semantic network
			outputSVGfileName = outputAllFileName + ".svg";
		}
		if(!useOutputPPMfile)
		{
			if(useOutputAllFile)
			{
				useOutputPPMfile = true;
				outputPPMfileName = outputAllFileName + ".ppm";
			}
		}
	}
	/*
	if(generateHTMLdocumentationMode != CS_GENERATE_HTML_DOCUMENTATION_MODE_OFF)
	{//only print html when explicitly set to do so
		if(!useOutputHTMLfile)
		{
			if(useOutputAllFile)
			{
				useOutputHTMLfile = true;
				outputHTMLfileName = outputAllFileName + ".html";
			}
		}
	}
	*/



	if(!XMLrulesClassClass().parseCSrulesXMLfile())
	{
		cout << "error: no rules file detected" << endl;
		exit(0);
	}
	LDspriteClass().fillInLDspriteExternVariables();
	CSdrawClass().fillInCSrulesExternVariables();

	if(mode == CS_MODE_OUTPUT_EXECUTION_FLOW)
	{
		CSexecflowClass().generateCodeStructure(topLevelFileName, rasterImageWidth, rasterImageHeight, outputLDRfileName, outputSVGfileName, outputPPMfileName, outputHTMLfileName, useOutputLDRfile, useOutputPPMfile, useOutputHTMLfile, generateHTMLdocumentationMode, displayInOpenGLAndOutputScreenshot, outputFunctionsConnectivity, traceFunctionUpwards, bottomLevelFunctionNameToTraceUpwards, outputFileConnections, topLevelFunctionName, generateOOcode, generateConstFunctionArgumentsCode);
	}
	else if(mode == CS_MODE_OUTPUT_DATA_FLOW)
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
	else if(mode == CS_MODE_FILTER_CODE_USING_PREPROCESSOR_DEFINITIONS)
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
	else
	{
		cout << "error: invalid operation mode" << endl;
		exit(0);
	}
#endif
}



