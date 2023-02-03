#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <iterator>

using namespace std;

// TODO: Complementary work needed: Don't use for_each to print, try copying to
// the outputstream
//------------DONE------------------

// TODO: Complementary work needed: Capture only the things you need in a lambda
//------------In the current version we don't see a problem like this. If it still exists, please be so kind to specify the location-------------

// TODO: Complementary work needed: You can just copy the map into vector of pairs
// and not flip it
//-----------DONE-------------------

// COMMENT: To add to your map you can just use map[i]++ to add a new word or increase
// the one already existing
//----------DONE-------------------

// TODO: Complementary work neeed: Do not repeat similar code, frequency and table are 
// the same except for sticky manipulators
//-------------DONE--------------------


// TODO: Complementary work needed: Use reference to constant for input parameters 
// of class type.  
//-------------DONE (except for the cases when the input parameter changes in a function)------------




//--print prints all the words in text, separated with a space, to std::cout.
//refactored 19-12-22. instead of using for-each we just copy to the outputstream
void print(std::vector<string> const& text) {
	cout << "Printing the text: " << endl;
	copy(text.cbegin(), text.cend(), std::ostream_iterator<string>(std::cout, " "));
	cout << endl << "----------------------------------" << endl << endl;
}

//refactored 19-12-2022. simplified the addition to the map.
//choose map because it contains unique keys so it's easy to calculate occurences
void calculateFrequencies(std::vector<string> const& text, std::map<std::string, int>& wordsMap)
{
	std::for_each(text.cbegin(), text.cend(), [&](const auto word) {
		wordsMap[word]++;
		});
}

//added 19-12-22. new universal function both for table and frequency
//simplified usage of the map and the vector
//--frequency prints a frequency table that is sorted in decreasing order on the number of occurrences and right aligned
//--table prints a frequency table where the words are sorted in lexicographic order (sorted A to Z) and left aligned in the table.
void frequencyAndTable(std::vector<string> const& text, std::string const direction)
{
	//create a map and call a function that will calculate word frequencies in text and store then into this map
	std::map<std::string, int> wordsMap;
	calculateFrequencies(text, wordsMap);

	//transfer word frequencies to a vector of pairs to be able to sort them in certain order
	map<string, int> ::iterator it;
	std::vector<std::pair<string, int>> wordsVectorForFreq;

	//finding the word of max length
	int maxlength{};
	for (it = wordsMap.begin(); it != wordsMap.end(); ++it)
	{
		if (maxlength < it->first.size()) {
			maxlength = it->first.size();
		}
	}

	//copy map to a vector to be able to sort it in a different way
	copy(wordsMap.begin(), wordsMap.end(), back_inserter(wordsVectorForFreq));

	//sort and print
	if (direction == "frequency")
	{
		std::sort(wordsVectorForFreq.rbegin(), wordsVectorForFreq.rend(), [](auto& left, auto& right) {
			return left.second < right.second;
			});
		cout << "Print sorted pairs in frequency order (frequency):" << endl;
		auto printwords = [maxlength](const pair<string, int>& s) {
			std::cout << std::setw(maxlength) << right << s.first << "  " << s.second << endl;
		};
		std::for_each(wordsVectorForFreq.cbegin(), wordsVectorForFreq.cend(), printwords);
		cout << "----------------------------------" << endl << endl;
	}
	else if (direction == "table") {
		std::sort(wordsVectorForFreq.rbegin(), wordsVectorForFreq.rend(), [](auto& left, auto& right) {
			return left.first >= right.first;
			});
		cout << "Print sorted pairs in lexicographic order (table):" << endl;
		auto printwords = [maxlength](const pair<string, int>& s) {
			std::cout << std::setw(maxlength) << left << s.first << "  " << s.second << endl;
		};
		std::for_each(wordsVectorForFreq.cbegin(), wordsVectorForFreq.cend(), printwords);
		cout << "----------------------------------" << endl << endl;
	}
}

//--substitute = <old>+<new> replaces all occurances of <old> in text with <new>, where <old> and <new> are arbitrary strings specified by the user.
void substitute(std::vector<string>& text, string const parameter)
{
	if (parameter.find('+') != std::string::npos)
	{
		std::string oldWord = parameter.substr(0, parameter.find('+'));
		std::string newWord = parameter.substr(parameter.find('+') + 1, parameter.size() - 1);
		cout << "Replacing words in the text with parameters: " << oldWord << "-->" << newWord << endl;
		std::replace(text.begin(), text.end(), oldWord, newWord);
		print(text);
	}
	else
	{
		cerr << "Wrong usage of substitute option. Correct pattern: --substitute=<old>+<new>" << endl << endl;
		cout << "----------------------------------" << endl << endl;
	}
}

//--remove = <word> removes all occurances of the word <word> in text, where <word> is an arbitrary string specified by the user.
void remove(std::vector<string>& text, string const parameter)
{
	cout << "Removing words from the text with parameter: " << parameter << endl;
	text.erase(std::remove(text.begin(), text.end(), parameter), text.end());
	print(text);
}

//function to parse parameters list
void analyze(std::string const argument, std::vector<string>& text)
{
	std::string flag{};
	std::string parameter{};

	if (argument.find('=') == std::string::npos)
	{
		flag = argument;
	}
	else
	{
		flag = argument.substr(0, argument.find('='));
		parameter = argument.substr(argument.find('=') + 1, argument.size() - 1);
	}

	if (flag == "--print") { print(text); }
	else
		if (flag == "--frequency") { frequencyAndTable(text, "frequency"); }
		else
			if (flag == "--table") { frequencyAndTable(text, "table"); }
			else
				if (flag == "--substitute") { substitute(text, parameter); }
				else
					if (flag == "--remove") { remove(text, parameter); }

}


int main(int argc, char** argv)
{

	if (argc < 3) {
		cerr << argc << endl;
		cerr << "Invalid number of arguments was provided. Please correct and run the program again providing at least three arguments: " << endl <<
			"1. The name of executable file;" << endl <<
			"2. The name of file to be edited;" << endl <<
			"3. At least one parameter (editing option)." << endl;
		return -1;
	}
	else
	{
		//variables for file name, arguments storage and file content
		string fileName{};
		std::vector<string> arguments;
		std::vector<string> filetext;

		try {
			//get file name, arguments and file body into variables (using vectors seems quite convinient because they provide flexible options of working with list of values of different types
			fileName = argv[1];
			arguments.assign(argv + 2, argv + argc);
			std::ifstream ifstr(fileName);
			std::vector<std::string> text{ std::istream_iterator<std::string> {ifstr},
				 std::istream_iterator<std::string>{}
			};
			ifstr.close();

			//starting processing the arguments
			for (auto argument : arguments) {
				analyze(argument, text);
			}
		}
		catch (const std::exception& e) {
			cerr << "ERROR: Incorrect argument list. Try again." << endl;
			return -1;
		}
	}
}
