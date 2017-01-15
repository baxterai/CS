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
 * File Name: CSexecflow.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3e1a 27-August-2014
 *
 *******************************************************************************/

//issues, need to flip vertically

#include "CSexecflow.h"
#include "CSoperations.h"
#include "CSdraw.h"
#include "LDsvg.h"
#include "LDopengl.h"
#include "XMLrulesClass.h"
#include "LDparser.h"
#include "LDsprite.h"
#include "RTpixelMaps.h"
#include "CSgenerateHTMLdocumentation.h"
#ifdef CS_GENERATE_CPP_CLASSES
#include "CSgenerateObjectOrientedCode.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <map>		//required for use with generateHTMLdocumentationFunctionReferenceList
#include <utility>
using namespace std;

#ifndef LINUX
	#include <windows.h>
#endif




void generateCodeStructure(string topLevelFileName, int width, int height, string outputLDRfileName, string outputSVGfileName, string outputPPMfileName, string outputHTMLfileName, bool useOutputLDRfile, bool useOutputPPMfile, bool useOutputHTMLfile, int generateHTMLdocumentationMode, bool display, bool outputFunctionsConnectivity, bool traceFunctionUpwards, string bottomLevelFunctionNameToTraceUpwards, bool outputFileConnections, string topLevelFunctionName, bool generateOOcode)
{
	bool result = true;

	bool htmlDocumentationGenerationPreventsDisplay = false;

	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	cout << "useOutputHTMLfile = " << int(useOutputHTMLfile) << endl;
	cout << "outputHTMLfileName = " << outputHTMLfileName << endl;
	cout << "generateHTMLdocumentationMode = " << generateHTMLdocumentationMode << endl;
	cout << "traceFunctionUpwards = " << traceFunctionUpwards << endl;
	#endif

	if(display)
	{
		initiateOpenGL(width, height, 0, 0, false);
	}

	char * outputFileNameLDRcharstar = const_cast<char*>(outputLDRfileName.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputPPMfileName.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGfileName.c_str());

	//1. for every f
		//2. for every include f listed
			//3. add to ref list
	//for every reference
		//for every reference, and merge all

	CSfileReference * topLevelReferenceInList = new CSfileReference();
	topLevelReferenceInList->name = topLevelFileName;
	CSfileReference * firstReferenceInTopLevelBelowList = new CSfileReference();
	topLevelReferenceInList->firstReferenceInBelowList = firstReferenceInTopLevelBelowList;

	//cout << "start getIncludeFileNamesFromCorHfile()" << endl;
	bool hFileFound = getIncludeFileNamesFromCorHfile(firstReferenceInTopLevelBelowList, topLevelReferenceInList, topLevelFileName, 0);
	//cout << "finish getIncludeFileNamesFromCorHfile()" << endl;
	if(!hFileFound)
	{
		cout << "generateCodeStructure() error: !hFileFound: " << topLevelFileName << endl;
	}

	initiateMaxXatParticularY();
	Reference * firstReferenceInPrintList = new Reference();

	setCurrentDirectory(tempFolderCharStar);

	XMLparserTag * firstTagInSVGFile = new XMLparserTag();
	XMLparserTag * currentTagInSVGFile = firstTagInSVGFile;


	#ifdef CS_SUPPORT_PREDEFINED_GRID
	bool usePredefinedGrid = false;
	bool tempResult = true;
	XMLparserTag * firstTagInRulesTag = parseTagDownALevel(CSfirstTagInXMLfile, RULES_XML_TAG_rules, &tempResult);
	XMLparserTag * firstTagInGridTag = NULL;
	if(tempResult)
	{
		XMLparserTag * currentTag = firstTagInRulesTag;
		while(currentTag->nextTag != NULL)
		{
			if(currentTag->name == RULES_XML_TAG_grid)
			{
				firstTagInGridTag = parseTagDownALevel(currentTag, RULES_XML_TAG_grid, &tempResult);
				if(tempResult)
				{
					usePredefinedGrid = true;
				}
			}
			currentTag = currentTag->nextTag;
		}
		if(!usePredefinedGrid)
		{
			cout << "error: CS_SUPPORT_PREDEFINED_GRID: !foundGridTag - check CSrules.xml" << endl;
		}
	}
	#endif

	Reference * currentReferenceInPrintList = createFileReferenceListBoxes(firstReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, outputFunctionsConnectivity, traceFunctionUpwards, firstTagInGridTag, usePredefinedGrid);
	if(outputFileConnections)
	{
		currentReferenceInPrintList = createFileReferenceListConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, firstReferenceInTopLevelBelowList, &currentTagInSVGFile, traceFunctionUpwards);
	}
	if(outputFunctionsConnectivity)
	{
		CSfunctionReference * currentReferenceInTopLevelBelowList = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
		bool topLevelFunctionNameFound = false;
		CSfunctionReference * topLevelFunctionReference = NULL;
		while(currentReferenceInTopLevelBelowList->next != NULL)
		{
			//cout << "currentReferenceInTopLevelBelowList->name = " << currentReferenceInTopLevelBelowList->name << endl;
			if(currentReferenceInTopLevelBelowList->name == topLevelFunctionName)
			{
				topLevelFunctionReference = currentReferenceInTopLevelBelowList;
				topLevelFunctionNameFound = true;
			}
			currentReferenceInTopLevelBelowList = currentReferenceInTopLevelBelowList->next;
		}
		if(topLevelFunctionNameFound)
		{
			//CSfunctionReference * topLevelFunctionReference = firstReferenceInTopLevelBelowList->firstReferenceInFunctionList;
			topLevelFunctionReference->printX = firstReferenceInTopLevelBelowList->printX;
			topLevelFunctionReference->printY = firstReferenceInTopLevelBelowList->printY;
			topLevelFunctionReference->col = firstReferenceInTopLevelBelowList->col;

			firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] = firstReferenceInTopLevelBelowList->maxFunctionPrintXAtAParticularY[0] + 1;
			topLevelFunctionReference->printed = true;
			if(!(firstReferenceInTopLevelBelowList->printed))
			{
				cout << "error" << endl;
				exit(0);
			}

			CSfunctionReference * currentReferenceInFunctionReferenceList = topLevelFunctionReference->firstReferenceInFunctionReferenceList;
			while(currentReferenceInFunctionReferenceList->next != NULL)
			{
				currentReferenceInPrintList = createFunctionReferenceListBoxesAndConnections(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, topLevelFunctionReference, firstReferenceInTopLevelBelowList, 0, currentReferenceInFunctionReferenceList->name, &currentTagInSVGFile, traceFunctionUpwards, false, NULL, usePredefinedGrid);
				currentReferenceInFunctionReferenceList = currentReferenceInFunctionReferenceList->next;
			}
			resetPrintedFunctionConnections(firstReferenceInTopLevelBelowList, topLevelFunctionReference, false, NULL);

			if(traceFunctionUpwards && (bottomLevelFunctionNameToTraceUpwards != ""))
			{
				bool foundBottomLevelFunctionRef = false;
				CSfileReference * fileReferenceHoldingFunction = NULL;
				CSfunctionReference * bottomLevelFunctionToTraceUpwards = findPrintedFunctionReferenceWithName(bottomLevelFunctionNameToTraceUpwards, NULL, firstReferenceInTopLevelBelowList, &foundBottomLevelFunctionRef, &fileReferenceHoldingFunction);
				
				if(foundBottomLevelFunctionRef)
				{
					string fileNameHoldingFunction = fileReferenceHoldingFunction->name;

					if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
					{
						if(!useOutputHTMLfile)
						{//still use single html file for function, but automatically generate html file name (based on function name)
							useOutputHTMLfile = true;
							outputHTMLfileName = bottomLevelFunctionNameToTraceUpwards + HTML_EXTENSION;	//automatically generate html file name (based on function name)
						}
					}
					string HTMLdocumentationFunctionNOTUSED = "";
					generateHTMLdocumentationForFunction(currentReferenceInPrintList, firstReferenceInTopLevelBelowList, bottomLevelFunctionToTraceUpwards, fileNameHoldingFunction, &currentTagInSVGFile, generateHTMLdocumentationMode, &HTMLdocumentationFunctionNOTUSED, &outputSVGfileName, useOutputHTMLfile, outputHTMLfileName, traceFunctionUpwards);
				}
				else
				{
					cout << "error: foundBottomLevelFunctionRef " << bottomLevelFunctionNameToTraceUpwards << " cannot be found" << endl;
					exit(0);
				}
			}
			else
			{
				if(generateHTMLdocumentationMode == CS_GENERATE_HTML_DOCUMENTATION_MODE_ON)
				{//generate documentation for all functions...
					generateHTMLdocumentationFunctions(firstTagInSVGFile, firstReferenceInTopLevelBelowList, generateHTMLdocumentationMode, useOutputHTMLfile, traceFunctionUpwards, usePredefinedGrid, outputHTMLfileName);
					htmlDocumentationGenerationPreventsDisplay = true;	//cannot display in OpenGL/save to file, as LD vector graphics references have been deleted
				}
			}
		}
		else
		{
			cout << "error: !topLevelFunctionNameFound" << endl;
			exit(0);
		}
	}
	
	#ifdef CS_GENERATE_CPP_CLASSES
	if(generateOOcode)
	{
		if(!generateCPPclasses(firstReferenceInTopLevelBelowList))
		{
			result = false;
		}
	}
	#endif
	
	if(!htmlDocumentationGenerationPreventsDisplay)
	{//do not display if generating html (unless tracing single file)

		if(!writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXX, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MINY, CS_CODE_STRUCTURE_FUNCTION_DIAGRAM_MAXY))
		{
			result = false;
		}
		delete firstTagInSVGFile;

		if(useOutputLDRfile || display)
		{
			writeReferencesToFile(outputFileNameLDRcharstar, firstReferenceInPrintList);
		}

		if(display)
		{
			//re-parse, then re-write to create a collapsed referencelist file...
			//method1:
			char * topLevelSceneFileName = outputFileNameLDRcharstar;
			char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
			Reference * initialReferenceInSceneFile = new Reference();
			Reference * topLevelReferenceInSceneFile = new Reference(topLevelSceneFileName, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileName, initialReferenceInSceneFile, topLevelReferenceInSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileName << " does not exist in the directory" << endl;
				exit(0);
			}
			write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFile);


			unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

			//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
			setViewPort3Dortho(-100.0, 2000, 2000.0, -100.0, 1.0, -1.0);

			//now reparse file
			Reference * initialReferenceInCollapsedSceneFile = new Reference();
			Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
			{//file does not exist
				cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
				exit(0);
			}

			drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
			drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				//due to opengl code bug, need to execute this function twice.

			if(useOutputPPMfile)
			{
				generatePixmapFromRGBmap(displayFileNamePPMcharstar, width, height, rgbMap);
			}

			delete rgbMap;

		}
		else
		{
			//must use an external program to view the .ldr file (Eg LDView)
		}
	}

}

