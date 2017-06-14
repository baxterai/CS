/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: CSreferenceContainerClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Code Structure viewer
 * Project Version: 3l2a 12-June-2017
 *
 *******************************************************************************/

#include "CSreferenceContainerClass.hpp"




CSfunctionContainer::CSfunctionContainer(void)
{
	next = NULL;
	functionObject = NULL;
	fileObjectHoldingFunction = NULL;
}

CSfunctionContainer::~CSfunctionContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(functionObject != NULL)
	{
		delete functionObject;
	}
}

CSfileContainer::CSfileContainer(void)
{
	next = NULL;
	fileObject = NULL;
	aboveLevelObject = NULL;
}

CSfileContainer::~CSfileContainer(void)
{
	if(next != NULL)
	{
		delete next;
	}
	if(fileObject != NULL)
	{
		delete fileObject;
	}
}





//CHECKTHIS;
int CSreferenceContainerClassClass::findEndPositionOfArgument(const string* functionArgumentsRaw, const int startPositionOfArgument, bool* lastArgument)
{
	int endPositionOfArgument = -1;
	int bracketLevel = 1;
	int pos = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	char cPrevious = 'a';
	while(stillFindingEndPositionOfArgument)
	{
		char c = (*functionArgumentsRaw)[pos];
		if(bracketLevel == 1)
		{//only detect base level function reference commas (not commas for embedded function reference arguments)
			if((c == CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_FUNCTION_ARGUMENT_DELIMITER) || (c == CHAR_CLOSE_BRACKET))
			{
				endPositionOfArgument = pos;
				stillFindingEndPositionOfArgument = false;
				if(c == CHAR_CLOSE_BRACKET)
				{
					*lastArgument = true;
				}
			}
		}
		if(c == CLASS_TYPE_OPEN_TAG)
		{
			bracketLevel++;
		}
		else if(c == CLASS_TYPE_CLOSE_TAG)
		{
			if(cPrevious != string(CS_GENERATE_CONST_FUNCTION_ARGUMENTS_TEXT_OBJECT_REFERENCE_DELIMITER)[0])
			{
				bracketLevel--;
			}
		}
		pos++;
		cPrevious = c;
	}
	return endPositionOfArgument;

}

/*
//limitation; only supports a single level of class type assignment (ie, multiple <> tags not supported in a given variable type, eg <><>)
int findEndPositionOfArgument(string* functionArgumentsRaw, int startPositionOfArgument)
{
	#ifdef CS_DEBUG_HTML_DOCUMENTATION
	//cout << "functionArgumentsRaw = " <<* functionArgumentsRaw << endl;
	//cout << "startPositionOfArgument = " << startPositionOfArgument << endl;
	#endif

	int startPositionOfArgumentTemp = startPositionOfArgument;
	bool stillFindingEndPositionOfArgument = true;
	int endPositionOfArgument = -1;
	while(stillFindingEndPositionOfArgument)
	{
		endPositionOfArgument = functionArgumentsRaw->find(CHAR_COMMA, startPositionOfArgumentTemp);	//find next comma
		#ifdef CS_DEBUG_HTML_DOCUMENTATION
		//cout << "endPositionOfArgument = " << endPositionOfArgument << endl;
		#endif

		if(endPositionOfArgument == CPP_STRING_FIND_RESULT_FAIL_VALUE)
		{
			stillFindingEndPositionOfArgument = false;
		}
		else
		{
			int nextPositionOfClassTypeOpenTag = functionArgumentsRaw->find(CLASS_TYPE_OPEN_TAG, startPositionOfArgumentTemp);	//find next comma
			int nextPositionOfClassTypeCloseTag = functionArgumentsRaw->find(CLASS_TYPE_CLOSE_TAG, startPositionOfArgumentTemp);	//find next comma
			if(nextPositionOfClassTypeOpenTag == CPP_STRING_FIND_RESULT_FAIL_VALUE)
			{
				stillFindingEndPositionOfArgument = false;
			}
			else
			{
				if(nextPositionOfClassTypeOpenTag < endPositionOfArgument)
				{
					bool foundCloseTag = false;

					if(nextPositionOfClassTypeCloseTag != CPP_STRING_FIND_RESULT_FAIL_VALUE)
					{
						if(nextPositionOfClassTypeCloseTag < endPositionOfArgument)
						{
							foundCloseTag = true;
						}
					}
					if(!foundCloseTag)
					{
						startPositionOfArgumentTemp = endPositionOfArgument+1;
						#ifdef CS_DEBUG_HTML_DOCUMENTATION
						//cout << "startPositionOfArgumentTemp = " << startPositionOfArgumentTemp << endl;
						#endif
					}
					else
					{
						stillFindingEndPositionOfArgument = false;
					}
				}
				else
				{
					stillFindingEndPositionOfArgument = false;
				}
			}
		}
	}
	return endPositionOfArgument;
}
*/

//limitation; can't match argument types (only number of arguments)
bool CSreferenceContainerClassClass::findFunctionReferenceTarget(const CSfunction* functionReference, constEffective CSfile* currentFileObject, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments)
{
	string name = functionReference->name;
	int numArguments = this->countArgumentList(functionReference->firstFunctionArgumentInFunction);
	//cout << "findFunctionReferenceTarget{}:" << endl;
	//cout << "name = " << name << endl;
	//cout << "numArguments = " << numArguments << endl;

	bool foundPrintedReferenceWithName = false;

	constEffective CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
	while(currentFunctionObject->next != NULL)
	{
		bool conditions = false;
		if(currentFunctionObject->name == name)
		{
			if(countArguments)
			{
				if(this->countArgumentList(currentFunctionObject->firstFunctionArgumentInFunction) == numArguments)
				{
					conditions = true;
				}
			}
			else
			{
				conditions = true;
			}
		}
		if(conditions)
		{
			*functionReferenceTarget = currentFunctionObject;
			foundPrintedReferenceWithName = true;
			*fileObjectHoldingFunction = currentFileObject;
		}
		currentFunctionObject = currentFunctionObject->next;
	}

	if(currentFileObject->firstFileInBelowListContainer != NULL)
	{
		if(this->findFunctionReferenceTargetRecurse(functionReference, currentFileObject->firstFileInBelowListContainer, fileObjectHoldingFunction, functionReferenceTarget, countArguments))
		{
			foundPrintedReferenceWithName = true;
		}
	}

	//cout << "end findFunctionReferenceTarget{}:" << endl;

	return foundPrintedReferenceWithName;
}

bool CSreferenceContainerClassClass::findFunctionReferenceTargetRecurse(const CSfunction* functionReference, constEffective CSfileContainer* firstObjectInAboveLevelBelowListContainer, constEffective CSfile** fileObjectHoldingFunction, constEffective CSfunction** functionReferenceTarget, const bool countArguments)
{
	bool foundPrintedReferenceWithName = false;

	string name = functionReference->name;
	int numArguments = this->countArgumentList(functionReference->firstFunctionArgumentInFunction);

	constEffective CSfileContainer* currentFileObjectContainer = firstObjectInAboveLevelBelowListContainer;

	while(currentFileObjectContainer->next != NULL)
	{
		constEffective CSfile* currentFileObject = currentFileObjectContainer->fileObject;

		constEffective CSfunction* currentFunctionObject = currentFileObject->firstFunctionInFunctionList;
		while(currentFunctionObject->next != NULL)
		{
			bool conditions = false;
			if(currentFunctionObject->name == name)
			{
				if(countArguments)
				{
					if(this->countArgumentList(currentFunctionObject->firstFunctionArgumentInFunction) == numArguments)
					{
						conditions = true;
					}
				}
				else
				{
					conditions = true;
				}
			}
			if(conditions)
			{
				*functionReferenceTarget = currentFunctionObject;
				foundPrintedReferenceWithName = true;
				*fileObjectHoldingFunction = currentFileObject;
			}
			currentFunctionObject = currentFunctionObject->next;
		}

		if(currentFileObject->firstFileInBelowListContainer != NULL)
		{
			if(this->findFunctionReferenceTargetRecurse(functionReference, currentFileObject->firstFileInBelowListContainer, fileObjectHoldingFunction, functionReferenceTarget, countArguments))
			{
				foundPrintedReferenceWithName = true;
			}
		}

		currentFileObjectContainer = currentFileObjectContainer->next;
	}

	return foundPrintedReferenceWithName;
}

int CSreferenceContainerClassClass::countArgumentList(const CSfunctionArgument* firstFunctionArgumentInFunction)
{
	int count = 0;
	const CSfunctionArgument* currentFunctionArgumentInFunction = firstFunctionArgumentInFunction;
	while(currentFunctionArgumentInFunction->next != NULL)
	{
		count++;
		currentFunctionArgumentInFunction = currentFunctionArgumentInFunction->next;
	}
	return count;
}


