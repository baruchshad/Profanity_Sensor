/*
	Name: Profanity Filter
	Author: Baruch Shadrouz
	Date: 4/20/18
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <list>
#include <queue>
#include <set>

using namespace std;

ifstream infile;
ofstream outfile;

/*
* Function: Makes sure the string is in lowercase for comparison purposes
* Called: searchReplace
* Calls: N/A
*/
void paperStats(int count, char filterLevel, string fileName)
{
	int wordCount = 0;
	string stats, word;

	infile.open((fileName + ".txt").c_str());
	if (!infile.is_open())
	{
		cout << "ERROR: Unable to open file" << endl;
		system("pause");
		exit(0);
	}

	stats = fileName + "_stats.txt";
	outfile.open(stats.c_str());
	if (!outfile.is_open()) {
		cout << "\nERROR: Unable to create file..." << endl;
		system("pause");
		exit(0);
	}
	cout << "\nPaper stats are being processed..." << endl;
	infile >> word;
	while (!infile.eof())
	{
		wordCount++;
		infile >> word;
	}
	outfile << "Original File Name: " << fileName << endl;
	outfile << "Copied file name: " << (fileName + "_copy.txt") << endl;
	outfile << "Filtered file name: " << (fileName + "_filtered.txt") << endl;
	outfile << "Number of Profanity words: " << count << endl;
	outfile << "Filter strictness level: ";
	if (tolower(filterLevel) == 'p')
		outfile << "Partial" << endl;
	else
		outfile << "Full" << endl;
	outfile << "Number of words in Paper: " << wordCount << endl;
	outfile << "Ratio of Regular to Pofanity: " << (float)wordCount / count << endl;

	outfile.close();
	infile.close();
	cout << "Paper stats successfuly processed." << endl;
}
/*
* Function: Makes sure the string is in lowercase for comparison purposes
* Called: searchReplace
* Calls: N/A
*/
string stringLower(string word)
{
	int l;

	for (int i = 0; i < word.length(); i++)
	{
		l = word[i];
		word[i] = tolower(l);
	}
	return word;
}

/*
* Function: Finds the next word in the file and outputs all white space
* Called By: searchReplace
* Calls: n/a
*/
string findNextWord()
{
	queue<char> tempWord;
	static char tempChar;
	string word;

	if (tempChar == NULL)
		infile.get(tempChar);

	while (!isalpha(tempChar) && !infile.eof())
	{
		outfile << tempChar;
		infile.get(tempChar);
	}

	while (isalpha(tempChar) && !infile.eof())
	{
		tempWord.push(tempChar);
		infile.get(tempChar);
	}

	while (!tempWord.empty())
	{
		word = word + tempWord.front();
		tempWord.pop();
	}
	return word;
}

/*
* Function: Copies input file to new file (for safety purposes)
* Called By: Main
* Calls: n/a
*/
void copy(string fileName)
{
	string line, outputfile;
	
	infile.open((fileName + ".txt").c_str());
	if (!infile.is_open())
	{
		cout << "ERROR: Unable to open file" << endl;
		system("pause");
		exit(0);
	}

	outputfile = fileName + "_copy.txt";
	outfile.open(outputfile.c_str());
	if (!outfile.is_open()) {
		cout << "\nERROR: Unable to create file..." << endl;
		system("pause");
		exit(0);
	}

	cout << "\nYour file is being copied for backup purposes..." << endl;
	while (!infile.eof()) {
		getline(infile, line);
		outfile << line << endl;
	}
	cout << "File copied successfully." << endl;
	outfile.close();
	infile.close();
}

/*
* Function: Searches for Profanity words and replaces them with '*' 
* Called By: main
* Calls: copy, findNextWord
*/
char searchReplace(set<string> profanityWords, string fileName)
{
	string filterFileName, searchWord;
	int profanityWordSize;
	char choice;

	infile.open((fileName + ".txt").c_str());
	if (!infile.is_open())
	{
		cout << "ERROR: Unable to open file" << endl;
		system("pause");
		exit(0);
	}

	filterFileName = fileName + "_filtered.txt";
	outfile.open(filterFileName.c_str());
	if (!outfile.is_open()) {
		cout << "\nERROR: Unable to create file..." << endl;
		system("pause");
		exit(0);
	}

	cout << "\nChoose your Filtering Strictness level: 'F' for full or 'P' for partial" << endl;
	cin >> choice;
	cout << "Your File is now being filtered..." << endl;
	set<string>::iterator profanityIter;
	infile >> searchWord;

	while (!infile.eof())
	{
		profanityIter = profanityWords.find(stringLower(searchWord));
		if (profanityIter != profanityWords.end())
		{
			switch (tolower(choice)) {
				case 'f':
					searchWord = "****";
					break;
				case 'p':
					profanityWordSize = searchWord.size();
					for (int i = 1; i < profanityWordSize - 1; i++)
					{
						searchWord[i] = '*';
					}
					break;
			}
			outfile << searchWord;	
		}
		else {
			outfile << searchWord;
		}
		searchWord = findNextWord();
	}
	outfile.close();
	infile.close();
	cout << "File filtered succesfully." << endl;
	return choice;
}

/*
* Function: Counts the number of profanity words in the paper
* Called By: main
* Calls: n/a
*/
void profanityWordCount(int &count, set<string> profanityWords, string &fileName)
{
	string word;
	bool openfile = true;
	int tryagain;

	count = 0;
	cout << "\nPlease enter the name of the file you want to filter:" << endl;
	getline(cin, fileName);
	infile.open((fileName + ".txt").c_str());
	if (!infile.is_open()) 
	{
		cout << "ERROR: Unable to open file" << endl;
		system("pause");
		exit(0);
	}

	set<string>::iterator profanityIter;
	infile >> word;
	while (!infile.eof())
	{
		profanityIter = profanityWords.find(stringLower(word));
		if (profanityIter != profanityWords.end())
		{
			count++;
		}
		infile >> word;
	}
	infile.close();
	cout << "Profanity word count complete." << endl;
}

/*
* Function: Fills list with Profanity words database 
* Called By: main
* Calls:
*/
void fillWordList(set<string> &wordset)
{
	string word, fileName;
	list<string> wordList;
	
	/*
	cout << "\nPlease enter the name of the file of Profanity words Database:" << endl;
	getline(cin, fileName);
	infile.open((fileName + ".txt").c_str());
	*/
	fileName = "Profanity words.txt";
	infile.open(fileName.c_str());
	if (!infile.is_open()) 
	{
		cout << "ERROR: Unable to open file" << endl;
		system("pause");
		exit(0);
	}

	infile >> word;
	while (!infile.eof())
	{
		wordset.insert(word);
		infile >> word;
	}
	infile.close();
	cout << "Database of words filled." << endl;
}

int main()
{
	set<string> proWordData;
	string paperFileName;
	int count;
	char filterLevel;

	fillWordList(proWordData);
	profanityWordCount(count, proWordData, paperFileName);
	copy(paperFileName);
	filterLevel = searchReplace(proWordData, paperFileName);
	paperStats(count, filterLevel, paperFileName);

	system("PAUSE");
	exit(0);
}
