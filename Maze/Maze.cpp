// Tries to solve a maze using a recursive solution
// Shows path through the maze if one exists

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <string>
using namespace std;

const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 81;

// ASCII characters to use for display
// https://theasciicode.com.ar/extended-ascii-code/block-graphic-character-ascii-code-219.html
const char WALL = (char)219;
const char WALLINFILE = '0';
const char START = 'S';
const char DEST = 'D';
const char EMPTY = ' ';
const char PATH = '*';
const char DEAD = 'X';

/*
* getFile will get a filename from the user. It then attmpts
* to open the file, read its contents, and store the maze
* structure into the maze array, based on the contents of the file.
* A WALLINFILE character in the file represents a maze wall, while
* an EMPTY character represents a free pathway. The file should also
* contain a single START character and a single DEST character.
* The function stops reading and exits the program if > MAXCOLS-1
* characters are found on one row, or if > MAXROWS rows are
* found in the file.  If the function is successful, then
* startX and startY will be set to the row and column number of
* where the START character is located in the maze.
*/
void getMaze(char maze[][MAXCOLS], int& startX, int& startY);

/*
* display is used to draw the maze on the screen.
*/
void display(const char maze[][MAXCOLS]);

// TODO: Declare the remaining functions you need

//this function takes the maze, coordinates of the starting position, and a integer passed by reference used to count the number of recursions as parameters
//This recursive function solves the maze by first checking if the current spot is valid or the destination and then picks a direction in a sequence and then calls itself to continue
//returns true or false based on if the function finds the destination spot in the maze
bool pathFinder(char maze[][MAXCOLS], int startX, int startY, int& count);


// This function takes the maze array and will go through every element in the array and initialize them to EMPTY while putting a null zero at the very last column of each row
void initArray(char maze[][MAXCOLS]);

/* This function will go through every element of the array checking for if it contains a DEAD character and replacing them with EMPTY.
* this puts the start character in it's inital position.
*startX The x-coordinate of the starting point in the maze.
*startY The y-coordinate of the starting point in the maze.
* maze is the maze array
*/
void cleanUp(char maze[][MAXCOLS], int startX, int startY);

int main()
{
	char maze[MAXROWS][MAXCOLS];

	// TODO: Write the main function

	//declare variables for starting positions 
	char userInput;

	int startRow = 0;
	int startCol = 0;
	int calls = 0;

	bool solutionFound = false;

	// Function call to initialize the array
	initArray(maze);

	// Function call to getMaze to begin program by asking for file input
	getMaze(maze, startRow, startCol);

	// Function call to show the maze before it is solved
	display(maze);

	// ask user to solve maze by pressing enter and stores it in userInput
	cout << "\nPress ENTER to solve maze\n";
	cin.get(userInput);

	// If the input wasn't an ENTER then it will start looking for an enter in the streambuffer and then solve the maze
	if (userInput != '\n')
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	// Function call to check for a solution and find one for the maze
	solutionFound = pathFinder(maze, startRow, startCol, calls);

	// Function call to cleanup the maze so that it only shows a single correct path
	cleanUp(maze, startRow, startCol);


	// Function call to show the maze after it is solved
	display(maze);


	// Display a message indicating whether or not a solution was found
	if (solutionFound)
	{
		cout << "\n\t\tSolution Found! -- " << calls << " recursive calls\n";
	}
	else
	{
		cout << "\n\t\tNo solution to this maze -- " << calls << " recursive calls\n";
	}
	cout << "\nPress ENTER to exit\n";
	cin.get();

	return 0;
}

// TODO: Write the code for the remaining functions that you need
//this is the function that sets each row of the array to 80 spaces that end with a null zero
void initArray(char maze[][MAXCOLS])
{
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			maze[i][j] = EMPTY;
		}
		maze[i][MAXCOLS - 1] = '\0';
	}
}

void display(const char maze[][MAXCOLS])
{
	// system("cls") clears the screen; only works on Windows machines
	// You will probably need to comment this out if working on
	// a non-Windows machine
	system("cls");

	// TODO: Write the display function

	// This for loop goes through every element in the array and displays it to the screen
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int j = 0; j < MAXCOLS; j++)
		{
			cout << maze[i][j];
		}
		// At the end of every row it goes to the next line
		cout << endl;
	}
}


void getMaze(char maze[][MAXCOLS], int& startX, int& startY)
{
	// TODO: Write the getMaze function
	char line[MAXCOLS];
	char file[FILENAMESIZE];
	char checker;
	size_t length = 0;
	int row = 0;

	//this prompts the user to enter the name of a file that contains a maze and then add the .txt part to the cstring afterward
	printf("Enter the name of a TXT file containing the maze (don't enter the 'txt') \n");
	cin.getline(file, FILENAMESIZE);
	snprintf(file, FILENAMESIZE, "%s.txt", file);

	ifstream inFile(file);

	//this checks to make sure there are no problems with opening the file and displays an error message to the user if there is along with closing the file and exiting the program
	if (!inFile.is_open())
	{
		cerr << "\nERROR: Input file cannot be opened.\n";
		inFile.close();
		exit(1);
	}

	//this runs as long as the amount of rows in the file are less than the max amount of rows in the array and there are still characters in the file to be extracted
	//this also extracts the first character in the line and sets it to the char variable checker
	while (row < MAXROWS && inFile.get(checker))
	{
		/*
		* this checks if checker is not a new line character, if so it ungets that character then uses .get to grab a line of up to MAXCOLS characters, as .get won't return a failbit
		* if it can't find the delimiter in the n-1 or nth position unlike .getline which means amount of character read for each line can be ristricted or controlled more easily,
		* and then after ignoring the left over characters in the buffer, if there are any, till it finds a new line character as .get doesn't throw away the delimiter
		* the character read get put it into the line cstring to be processed, if the value of checker is a new line character then it just copies it into the line cstring
		* in a way that adds the null zero
		*/
		if (checker != '\n')
		{
			inFile.unget();
			inFile.get(line, MAXCOLS);
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else
		{
			snprintf(line, MAXCOLS, "\n");
		}

		//this gets the length of line so it can be processed without going out of the bounds of the character array line, as the length of line isn't always going to be equal to MAXCOLS
		//but the length of line will always be less than or equal to MAXCOLS due to how it is read from the file
		length = strlen(line);

		/*
		* this loops through the line cstring to process it
		* it looks for the characters of WALLINFILE, EMPTY,START,and DEST and then assigns the corresponding character to the corresponding position in the maze array
		* The row and column of the START character are also recorded by setting the variables, startX, and startY to the current row and i (or column) of the maze array
		* whenever a START character is found in the extracted line from the file. Any other character outside of the ones being looked for are set to EMPTY
		*/
		for (int i = 0; i < length; i++)
		{
			if (line[i] == WALLINFILE)
			{
				maze[row][i] = WALL;
			}
			else if (line[i] == EMPTY)
			{
				maze[row][i] = EMPTY;
			}
			else if (line[i] == START)
			{
				maze[row][i] = START;
				startX = row;
				startY = i;
			}
			else if (line[i] == DEST)
			{
				maze[row][i] = DEST;
			}
			else
			{
				maze[row][i] = EMPTY;
			}
		}
		row++;

	}
	//this "eats" up the remaining characters in the file essentially reading them and then throwing them away to make sure the end of the file is always reached but no characters
	//are written to the maze array that are outside of the MAXROWS lines in the array, so the end of file can always be reached but no extra characters are written to the array.
	while (inFile.get(checker))
	{
		checker = EMPTY;
	}

	/*
	* Checks is the end of the file is reached
	* If it wasn't then determines whether it was becaues there were to many characters in a line or the file contained too many lines
	* after that has been decided it displays the corresponding error to the user, closes the file and then exits the program
	*/
	if (!inFile.eof())
	{
		if (row < MAXROWS)
		{
			cerr << "\nERROR: Line " << row << " in input file contains more than " << MAXCOLS << " chars\n";
		}
		else
		{
			cerr << "\nERROR: There are more than " << MAXROWS << " lines in the file\n";
		}
		inFile.close();
		exit(1);
	}
	inFile.close();
}

bool pathFinder(char maze[][MAXCOLS], int startX, int startY, int& count)
{
	//this is the first thing that is done in the function so it increments count every time the function is ran
	count++;
	//checking to make sure you are not out of bounds
	if (startX < 0 || startY < 0 || startX >= MAXROWS || startY >= MAXCOLS)
	{
		return false;
	}

	//short hand versus having multiple longer entries for the same symtax
	char position = maze[startX][startY];

	//checks to make sure the position is not already at the end of the maze
	if (position == DEST)
	{
		return true;
	}

	// checks to make sure you are not at a wall
	// also checking to that the space you are is not the start or empty
	if (position == WALL || (position != EMPTY && position != START))
	{
		return false;
	}

	//after finding if you are the start position or an empty position you move and leave figuring out if its a valid spot for the next call 
	else
	{
		//marks the current spot as PATH
		maze[startX][startY] = PATH;

		// move to the left
		if (pathFinder(maze, startX, startY - 1, count))
		{
			return true;
		}
		//move up 
		if (pathFinder(maze, startX - 1, startY, count))
		{
			return true;
		}
		//move right 
		if (pathFinder(maze, startX, startY + 1, count))
		{
			return true;
		}
		//move down
		if (pathFinder(maze, startX + 1, startY, count))
		{
			return true;
		}

		//assigning a spot as dead
		maze[startX][startY] = DEAD;
		return false;
	}
}

void cleanUp(char maze[][MAXCOLS], int startX, int startY)
{
	//This puts the start character back in original place in the array.
	maze[startX][startY] = START;

	//Finds any characters in the array that are DEAD and replaces them with an EMPTY character.
	for (int i = 0; i < MAXROWS; i++)
	{
		for (int v = 0; v < MAXCOLS; v++)
		{
			if (maze[i][v] == DEAD)
			{
				maze[i][v] = EMPTY;
			}
		}
	}
}