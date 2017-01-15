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
 * File Name: CSoperations.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3j1a 14-January-2017
 *
 *******************************************************************************/


#ifndef HEADER_CS_OPERATIONS
#define HEADER_CS_OPERATIONS

#include "LDreferenceClass.h"
#include "CSglobalDefs.h"
#include "CSreferenceContainerClass.h"

bool getIncludeFileNamesFromCorHfile(CSfileContainer* firstReferenceInIncludeFileListContainer, constEffective CSfileContainer* topLevelReferenceContainer, CSfile* aboveLevelObject, const string topLevelReferenceName, const int level);
	#ifdef CS_GENERATE_CODE_GENERIC
	bool fileIsHeader(string parseFileName);
	#endif
	bool findFileObjectInFileObjectContainerList(constEffective CSfileContainer* firstReferenceContainerInLevel, const string fileReferenceName, constEffective CSfile** fileReferenceFound);
	bool getFunctionNamesFromFunctionDeclarationsInHfile(CSfunction* firstFunctionInFunctionList, const string topLevelFileName, const int level);
	void getFunctionObjectNamesFromFunctionsInCfile(const CSfile* firstFileInIncludeFileList, CSfunction* firstFunctionInFunctionList, const CSfile* aboveLevelObject, const string topLevelFileName, const int level);
		bool searchFunctionStringForFunctionReferencesRecursive(const CSfile* firstFileInIncludeFileList, const CSfileContainer* firstFileNameInLayerContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
			bool searchFunctionStringForFunctionReferences(const CSfile* firstFileInIncludeFileList, const CSfile* fileNameContainingFunctionReferencesToSearchFor, CSfunction** currentReferenceInFunctionReferenceList, CSfunction** currentReferenceInFunctionReferenceListRepeats, string* functionContentsString);
				void identifyFunctionReferenceArguments(CSfunction* currentReferenceInFunctionReferenceList, string* functionContentsString, int indexToFunctionObject);
					string removePrependedWhiteSpace(const string argument);
				#ifdef CS_IDENTIFY_FUNCTION_DECLARATION_ARGUMENTS
				void identifyFunctionDeclarationArguments(CSfunction* currentReferenceInFunctionReferenceList, const string* functionNameFull);
				#endif

/*
CSfile* findReferenceInIncludeFileList(CSfile* firstReferenceInAboveLevelBelowList, string referenceName, CSfile* foundReference, bool* referenceFound);
*/

string generateSourceFileNameFromHeaderFileName(const string headerFileName, const string sourceFileNameExtension);

void attachFunctionReferenceTargets(CSfileContainer* firstObjectInAboveLevelBelowListContainer);

string extractFunctionArgumentName(string* argumentText, const int indexOfStartOfVariableName);

#endif

