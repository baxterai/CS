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
 * File Name: CSreferenceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3f7b 16-August-2015
 *
 *******************************************************************************/

#include "CSreferenceClass.h"




CSfunctionReference::CSfunctionReference(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printedFunctionConnections = false;
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FILE_CODE_STRUCTURE_DIAGRAMS
	singleFileOnlyPrinted = false;
	#endif
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	filePrintXrecordForSingleFileOnly = 0;
	filePrintYrecordForSingleFileOnly = 0;
	HTMLgenerated = false;

	next = NULL;
	previous = NULL;
	firstReferenceContainerInAboveReferenceList = NULL;	//new CSfunctionReferenceContainer()  - no longer possible with forward declaration of CSfunctionReferenceContainer
	hasHadFunctionReferencesParsed = false;
	printedTrace = false;
	printedTraceReset = false;

	//function reference only
	nameFull = "";
	firstReferenceInFunctionReferenceList = NULL;
	isFunctionReference = false;
	isFunctionReferenceReference = false;
	numTopLevelFunctionsInFileAlreadyPrinted = 0;
	#ifdef CS_HTML_DOCUMENTATION_RECORD_FUNCTION_INDENTATION
	functionReferenceIndentation = 0;
	#endif
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	string functionText;
	firstReferenceInFunctionReferenceListRepeats = NULL;
	functionReferenceCharacterIndex = 0;
	#endif
	#ifdef CS_GENERATE_CPP_CLASSES 
	functionTextRaw = "";
	headerFunctionNameFullUpdated = "";
	sourceFunctionNameFullUpdated = "";
	#endif
	
	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}
}

CSfunctionReference::~CSfunctionReference(void)
{
	/*
	if(firstReferenceInBelowListContainer != NULL)
	{
		delete firstReferenceInBelowListContainer;
	}
	*/
	if(next != NULL)
	{
		delete next;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
	if(firstReferenceInFunctionReferenceList != NULL)
	{
		delete firstReferenceInFunctionReferenceList;	//added 3e5a
	}
	#ifdef CS_HTML_DOCUMENTATION_GENERATE_FUNCTION_REFERENCE_LIST
	if(firstReferenceInFunctionReferenceListRepeats != NULL)
	{
		delete firstReferenceInFunctionReferenceListRepeats;	//added 3e5a
	}
	#endif
}

CSfileReference::CSfileReference(void)
{
	//file/function reference [shared]
	id = 0;
	level = 0;
	name = "";
	col = 0;
	printed = false;
	printedFileConnections = false;
	printX = 0;
	printY = 0;
	printXIndex = 0;
	printYIndex = 0;
	printTextX = 0;
	printTextY = 0;
	HTMLgenerated = false;

	firstReferenceContainerInAboveReferenceList = NULL;	//new CSfileReferenceContainer()  - no longer possible with forward declaration of CSfileReferenceContainer
	printedTrace = false;
	printedTraceReset = false;

	//file reference only
	firstReferenceInBelowListContainer = NULL;
	firstReferenceInFunctionList = NULL;

	for(int i=0; i<MAX_INCLUDE_DEPTH_FUNCTION; i++)
	{
		maxFunctionPrintXAtAParticularY[i] = 0;
	}
	
	#ifdef CS_GENERATE_CPP_CLASSES
	printedOO = false;
	sourceFileNameOrig = "";
	sourceFileTextOrig = "";
	headerFileTextOrig = "";
	headerFileName = "";
	sourceFileName = "";
	sourceFileText = "";
	headerFileText = "";
	#endif	
}

CSfileReference::~CSfileReference(void)
{
	if(firstReferenceInBelowListContainer != NULL)
	{
		delete firstReferenceInBelowListContainer;
	}
	if(firstReferenceContainerInAboveReferenceList != NULL)
	{
		delete firstReferenceContainerInAboveReferenceList;
	}
}



void printCSFileReference(CSfileReference* ref)
{
	cout << "col = " << ref->col << endl;
	cout << "id = " << ref->id << endl;
	cout << "printed = " << ref->printed << endl;
	cout << "printX = " << ref->printX << endl;
	cout << "printY = " << ref->printY << endl;
	cout << "printXIndex = " << ref->printXIndex << endl;
	cout << "printYIndex = " << ref->printYIndex << endl;
	cout << "printTextX = " << ref->printTextX << endl;
	cout << "printTextY = " << ref->printTextY << endl;
	cout << "level = " << ref->level << endl;
	cout << "name = " << ref->name << endl;
}

void printCSFunctionReference(CSfunctionReference* ref)
{
	cout << "col = " << ref->col << endl;
	cout << "id = " << ref->id << endl;
	cout << "printed = " << ref->printed << endl;
	cout << "printX = " << ref->printX << endl;
	cout << "printY = " << ref->printY << endl;
	cout << "printXIndex = " << ref->printXIndex << endl;
	cout << "printYIndex = " << ref->printYIndex << endl;
	cout << "printTextX = " << ref->printTextX << endl;
	cout << "printTextY = " << ref->printTextY << endl;
	cout << "level = " << ref->level << endl;
	cout << "name = " << ref->name << endl;
	cout << "nameFull = " << ref->nameFull << endl;
}



/*
//these functions are not being used:

void addReferenceToReferenceList(CSReference* reference, CSReference* firstReferenceInList)
{
	CSReference* currentReferenceInIncludeFileList = firstReferenceInList;

	bool foundMatch = false;
	while(currentReferenceInIncludeFileList->next != NULL)
	{
		if(reference->name == currentReferenceInIncludeFileList->name)
		{
			foundMatch = true;
		}

		if(currentReferenceInIncludeFileList->next->next == NULL)
		{
			if(!foundMatch)
			{
				currentReferenceInIncludeFileList->next = reference;
				currentReferenceInIncludeFileList->next->next = NULL;
				currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
			}
			else
			{
				currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
			}
		}
		else
		{
			currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
		}
	}
}


void mergeIncludeFileReferences(CSReference* firstReferenceInIncludeFileList, CSReference* firstReferenceInTopLevelBelowList)
{
	CSReference* currentReferenceInIncludeFileList = firstReferenceInIncludeFileList;

	while(currentReferenceInIncludeFileList->next != NULL)
	{
		findAndMergeAllIndenticalFileReferences(currentReferenceInIncludeFileList, firstReferenceInTopLevelBelowList);

		if(currentReferenceInIncludeFileList->firstReferenceInBelowListContainer != NULL)
		{
			mergeIncludeFileReferences(currentReferenceInIncludeFileList->firstReferenceInBelowListContainer, firstReferenceInTopLevelBelowList);
		}
		currentReferenceInIncludeFileList = currentReferenceInIncludeFileList->next;
	}
}


void findAndMergeAllIndenticalFileReferences(CSReference* reference, CSReference* firstReferenceInAboveLevelBelowList)
{
	CSReference* currentReference = firstReferenceInAboveLevelBelowList;

	while(currentReference->next != NULL)
	{
		#ifdef CS_DEBUG
		if(currentReference->name != "")
		{
			for(int i= 0; i<currentReference->level; i++)
			{
				cout << "\t";
			}
			cout << "currentReference->name = " << currentReference->name << endl;
			//cout << "currentReference->firstReferenceInBelowListContainer->name = " << currentReference->firstReferenceInBelowListContainer->name << endl;
		}
		#endif

		if(currentReference->name == reference->name)
		{//identical file reference found;

			if(currentReference->id != reference->id)
			{

				for(int i= 0; i<reference->level; i++)
				{
					cout << "\t";
				}
				cout << "reference->name = " << reference->name << endl;
				for(int i= 0; i<currentReference->level; i++)
				{
					cout << "\t";
				}
				cout << "currentReference->name = " << currentReference->name << endl;

				if(reference->level < currentReference->level)
				{
					reference->level = currentReference->level;
				}

				if(reference->firstReferenceInAboveList != NULL)
				{
					cout << "reference->firstReferenceInAboveList->name = " << reference->firstReferenceInAboveList->name << endl;
					cout << "currentReference->currentReference->name = " << currentReference->firstReferenceInAboveList->name << endl;

					mergeReferenceLists(reference->firstReferenceInAboveList, currentReference->firstReferenceInAboveList);
				}

				currentReference->previous->next = currentReference->next;
				delete currentReference;
			}
		}

		if(currentReference->firstReferenceInBelowListContainer != NULL)
		{

			findAndMergeAllIndenticalFileReferences(reference, currentReference->firstReferenceInBelowListContainer);
		}
		currentReference = currentReference->next;
	}



}


void mergeReferenceLists(CSReference* firstReferenceInList, CSReference* firstReferenceInListToMerge)
{
	CSReference* currentReferenceInReferenceListToMerge = firstReferenceInListToMerge;

	while(currentReferenceInReferenceListToMerge->next != NULL)
	{
		bool foundMatch = false;


		CSReference* currentReference = firstReferenceInList;

		while(currentReference->next != NULL)
		{
			if(currentReference->name == currentReferenceInReferenceListToMerge->name)
			{
				foundMatch = true;
			}
			currentReference = currentReference->next;
		}

		if(!foundMatch)
		{
			CSReference* currentReference = firstReferenceInList;
			while(currentReference->next != NULL)
			{
				if(currentReference->next->next == NULL)
				{
					currentReference->next = currentReferenceInReferenceListToMerge;
					currentReference->next->next = NULL;
					currentReference->next->previous = currentReference;
					currentReference = currentReference->next;
				}
				else
				{
					currentReference = currentReference->next;
				}
			}
		}

		currentReferenceInReferenceListToMerge = currentReferenceInReferenceListToMerge->next;
	}
}

*/



