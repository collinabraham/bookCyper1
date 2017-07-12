#include<iostream>	
#include<vector>
#include<fstream>
#include<filesystem>
#include<algorithm>			//std::find 
#include<cstdlib>			//rand()
#include<ctime>				//srand()time...
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 4) {
	cerr << "Error: Incorrect number of command line arguments.\n"
	<< "There are four command line arguments required.\n"
	<< "Proper usage is: bcencode.exe bookfile.txt messagefile.txt codefile.txt"
	<< "Exiting program.";
	return EXIT_FAILURE;
	}

	ifstream inBookFile(argv[1]);
	if (!inBookFile) {
	cerr << "Error: failed to open the book file: <" << argv[1] << ">\n"
	<< "Check filename, path, or it doesn't exist.\n"
	<< "Exiting program.";
	return EXIT_FAILURE;
	}

	ifstream inMsgFile(argv[2]);
	if (!inMsgFile) {
	cerr << "Error: failed to open the message file: <" << argv[2] << ">\n"
	<< "Check filename, path, or it doesn't exist.\n"
	<< "Exiting program.";
	return EXIT_FAILURE;
	}

	ofstream outCodedFile(argv[3]);

	// read messagefile and pushback a vector of chars with its contents
	vector<char> vMsgFile;
	char theChars;
	while (inMsgFile >> noskipws >> theChars)
		vMsgFile.push_back(theChars);

	// read bookfile and pushback a vector of chars with its contents\
	// while we can eat characters.. store them and their exact locations based on line and offset 
	vector<char> vBookFile;
	vector<int> vBookFileLineCount;
	vector<int> vBookFileOffset;
	unsigned lineCounter = 1;	//!!!!!!!!!!!!!!!!RESET THIS TO 0 BEFORE SUBMISSION.. USING 0 TO ACCOMODATE FOR NOTEPAD++ USING 1 AS THE FIRST LINE INSTEAD OF 0!!!!!!!!!!!
	unsigned offset = 0;
	unsigned i = 0;
	while (inBookFile >> noskipws >> theChars)
	{
		vBookFile.push_back(theChars);					// increment the char storage vector
		vBookFileOffset.push_back(offset);				// increment the offset storage vector
		vBookFileLineCount.push_back(lineCounter);		// increment the line counter storage vector
		++offset;										// increment offset counter after approriate information is stored 

		if (theChars == '\n')							// when a newline character is encountered
		{
			++lineCounter;								// increment the lineCounter and reset offset back to 0;
			offset = 0;
		}
	}

	// loop through the messagefile
	// for each character of the messagefile
	// loop through the bookfile to ensure that the element of the message file is within the bookfile 
	// if a matching element is encountered, continue out of that iteration of the for loop
	// if the character never found a match, alert the user and terminate the program 
	for (size_t i = 0; i < vMsgFile.size(); ++i)
	{
		if (find(vBookFile.begin(), vBookFile.end(), vMsgFile[i]) != vBookFile.end())
			continue;
		else
		{
			cerr << "ERROR: Character -> " << vMsgFile[i] << " in the message file \nwas not found within the bookfile!\n" << endl;
			cerr << "The character was found at index: " << i << endl;
			cerr << "The surrounding characters are ..." << vMsgFile[i - 2] << vMsgFile[i - 1] << vMsgFile[i] << vMsgFile[i + 1] << vMsgFile[i + 2] << "..." << endl;
			return EXIT_FAILURE;
		}
	}

	//bookfile tester
	/*while (vBookFileLineCount[i] != 100)
	{
		cout << endl;
		cout << "Char = " << vBookFile[i] << endl;
		cout << "Line# = " << vBookFileLineCount[i] << endl;
		cout << "Offset = " << vBookFileOffset[i] << endl;
		++i;
	}

	for (auto x : vMsgFile)
		cout << x << endl;*/

	//seed a random number based on the time of the computer
	srand((unsigned int)time(0));
	unsigned seedNum = rand() % vBookFile.size();
	unsigned messageIncrementer = 0;												// used as the basis of WHERE we are sitting within the message vector for comparison
	unsigned bookIncrementer(seedNum);												// used as the basis of WHERE we are sitting within the bookfile -> initialized with the rand seed
	vector<int> vMsgFileLines;														// storage for the new line locations
	vector<int> vMsgFileOffsets;													// storage for the new offset locations
	int resetMultiplier = 250;														// an arbitrary integer chosen as a multiplier after resetting "near" the beginning of the bookfile 

	// use the seed to jump into the bookfile vector at a random spot 
	// and begin comparing the values
	// of the bookfile and the message file
	do
	{
		if (vMsgFile[messageIncrementer] != vBookFile[bookIncrementer])				//if the book and the messagefile DON'T match
		{
			if (bookIncrementer >= vBookFile.size())								//check that if we have reached the end of the bookfile 
				bookIncrementer = seedNum * resetMultiplier;						//we go back to near the beginning of bookfile
				//bookIncrementer = 0;
			else
			{
				++bookIncrementer;													//otherwise increment the book check index and jump out of 1 interation of the loop
				continue;
			}


		}

		if (vMsgFile[messageIncrementer] == vBookFile[bookIncrementer])				//if the book and message file DO match
		{
			vMsgFileLines.push_back(vBookFileLineCount[bookIncrementer]);			//copy the corresponding line number into the new message file vector
			vMsgFileOffsets.push_back(vBookFileOffset[bookIncrementer]);			//copy the corresponding offset into the new message file vector
			++messageIncrementer;
			bookIncrementer += (rand() % 10000);									//jump to a random 1-50000 value of characters ahead to keep the 
																					//line numbers from making it obvious it is a book file

			if (bookIncrementer >= vBookFile.size())								//check that if we have reached the end of the bookfile 
				bookIncrementer = seedNum * resetMultiplier;						//we go back near the beginning of bookfile
				//bookIncrementer = 0;
		}

	} while (messageIncrementer != (vMsgFile.size()));							//loop until we reach the end of message file

	//print coded contents to screen 
	i = 0;
	while (i < vMsgFileLines.size())
	{
		cout << vMsgFile[i] << " = " << vMsgFileLines[i] << " " << vMsgFileOffsets[i] << endl;
		++i;
	}

	i = 0;

	//write to outCodedFile
	while (i < vMsgFileLines.size())
	{
		outCodedFile << vMsgFileLines[i] << " " << vMsgFileOffsets[i] << " ";
		++i;
	}
	outCodedFile.close();
}

