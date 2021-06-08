#include<iostream>
#include<fstream>
#include<conio.h>
#include<Windows.h>
using namespace std;
//80 X 24
void gotoxy(int x, int y)// keeps track of x,y position
{

	COORD pos = { x, y };

	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(output, pos);

}

class maze
{
public:
	char MAZE[80][24];//declaring size of the maze
	int i = 0, j = 0;//initializations
	void displaymaze()//function to display the maze
	{
		for (int j = 0; j < 24; j++)
		{
			for (int i = 0; i < 80; i++)
			{
				cout << MAZE[i][j];
			}cout << endl;
		}
	}
	void setmaze(char x)
	{
		MAZE[i][j] = x;


		if (MAZE[i][j] == '9')
		{
			MAZE[i][j] = char(219);
		}
		if (MAZE[i][j] == '1')
		{
			MAZE[i][j] = ' ';
		}
		i++;

		if (i == 80)
		{
			i = 0;
			j++;
		}

	}
};
class stack//stack class that controls when the mouse needs to change direction
{
public:
	int dir[250];
	int index = 0;
	int i = 0, j = 0;
	int revdir[250];
	int revindex = 0;
	void push(int x)//stack that only holds int values
	{
		dir[index] = x;

		index++;
	}
	int pop()//pops
	{
		index--;
		if (index < 0)
		{
			index = 0;
			dir[index] = 1;
		}
		return dir[index];
	}
	void revpush(int x)
	{
		revdir[revindex] = x;
		revindex++;
	}
	int revpop()
	{
		revindex--;
		if (revindex < 0)
		{
			revindex = 0;
		}
		return revdir[revindex];
	}


};
class mousepath//class that detects if a location has been visited
{
public:
	maze m;
	bool visited[80][24];
	int i = 0, j = 0;

};
class mousemovement
{
public:
	maze m;
	stack s;
	mousepath path;

	int i = 0, j = 0;
	bool ENDmaze(maze m)
	{
		if (m.MAZE[i][j] == 'F')//checks if maze is finished
		{
			system("cls"); cout << "MAZE COMPLETE!!!";
			return true;
		}
		else if (m.MAZE[i][j] == '@')//checks for bombs
		{
			system("cls"); cout << "YOU DIED :(  !!!";
			return true;
		}
		else
			return false;

	}
	void startpoint(maze m)//funtion for initiallizing start point
	{
		int i = j = 0;
		while (m.MAZE[i][j] == char(219))
		{
			gotoxy(i, ++j);
		}
		path.visited[i][j] = true;
	}

	void moveright(maze m)//moves mouse when its not hitting a wall
	{
		if (m.MAZE[i + 1][j] != char(219))
		{


			gotoxy(i++, j);
			m.MAZE[i][j] = '>';
			cout << m.MAZE[i][j];
			gotoxy(i, j);
			m.MAZE[i][j] = '#';
			cout << m.MAZE[i][j];
			path.visited[i][j] = true;
		}

	}
	void moveleft(maze m)//moves mouse when its not hitting a wall
	{
		if (m.MAZE[i - 1][j] != char(219))
		{


			gotoxy(i--, j);
			m.MAZE[i][j] = '<';
			cout << m.MAZE[i][j];
			gotoxy(i, j);
			m.MAZE[i][j] = '#';
			cout << m.MAZE[i][j];
			path.visited[i][j] = true;
		}

	}
	void moveup(maze m)//moves mouse when its not hitting a wall
	{
		if (m.MAZE[i][j - 1] != char(219))
		{


			gotoxy(i, j--);
			m.MAZE[i][j] = '^';
			cout << m.MAZE[i][j];
			gotoxy(i, j);
			m.MAZE[i][j] = '#';
			path.visited[i][j] = true;
		}

	}
	void movedown(maze m)//moves mouse when its not hitting a wall
	{
		if (m.MAZE[i][j + 1] != char(219))
		{


			gotoxy(i, j++);
			m.MAZE[i][j] = 'v';
			cout << m.MAZE[i][j];
			gotoxy(i, j);
			m.MAZE[i][j] = '#';
			cout << m.MAZE[i][j];
			path.visited[i][j] = true;
		}

	}
	bool deadend(maze m)//boolean funtion to check for dead ends
	{
		if (m.MAZE[i + 1][j] == char(219) && m.MAZE[i][j - 1] == char(219) && m.MAZE[i][j + 1] == char(219))//if right is dead end
		{
			cout << "\a";//windows sound when deadend is hit
			return true;
		}
		if (m.MAZE[i + 1][j] == char(219) && m.MAZE[i][j - 1] == char(219) && m.MAZE[i - 1][j] == char(219))// if up is dead end
		{
			cout << "\a";//windows sound when deadend is hit

			return true;
		}
		if (m.MAZE[i + 1][j] == char(219) && m.MAZE[i][j + 1] == char(219) && m.MAZE[i - 1][j] == char(219))//if down is dead end
		{
			cout << "\a";//windows sound when deadend is hit

			return true;
		}
		if (m.MAZE[i - 1][j] == char(219) && m.MAZE[i][j - 1] == char(219) && m.MAZE[i][j + 1] == char(219))//if left is dead end
		{
			cout << "\a";//windows sound when deadend is hit

			return true;
		}
		else
			return false;//returns false if not a dead end

	}
	void movement(maze m)//where the main logic of that maze takes place
	{
		int y = 0;
		stack s;

		while (ENDmaze(m) == 0)//condition if the maze has ended
		{
			Sleep(35);



			if (path.visited[i + 1][j] != true && m.MAZE[i + 1][j] != char(219) && !deadend(m))//right while checking for dead ends
			{
				y = 1;
				s.push(y);//keeps track of current position
				s.revpush(2);//keeps track of previous direction
			}
			else  if (path.visited[i][j - 1] != true && m.MAZE[i][j - 1] != char(219) && !deadend(m))//up while checking for dead ends
			{
				y = 4;
				s.push(y);//keeps track of current position
				s.revpush(3);//keeps track of previous direction
			}

			else if (path.visited[i][j + 1] != true && m.MAZE[i][j + 1] != char(219) && !deadend(m))//down while checking for dead ends
			{
				y = 3;
				s.push(y);//keeps track of current position
				s.revpush(4);//keeps track of previous direction
			}

			else if (path.visited[i - 1][j] != true && m.MAZE[i - 1][j] != char(219) && !deadend(m))//left while checking for dead ends
			{
				y = 2;
				s.push(y);//keeps track of current position
				s.revpush(1);//keeps track of previous direction
			}
			else
			{
				if (deadend(m))//handles if a deadend is hit
				{
					y = s.revpop();
				}
				else if (!deadend(m))//if deadend is not hit and checks if have already visited a given position
				{
					y = s.revpop();
				}
			}

			switch (y)//switch funtion that takes argument 'y' depending on previous arugments
			{
			case 1:
				moveright(m);
				break;
			case 2:moveleft(m);
				break;
			case 3:
				movedown(m);
				break;
			case 4:
				moveup(m);
				break;

			
			}

		}


	}
};

void main()
{
	ifstream file;//file stream declaration
	maze m;//declaring object maze
	mousemovement move;//declaring object of mousemovement class
	file.open("mazecourse.txt");//file pathway

	char c;
	while (file.peek() != -1)//reads until the end of the file
	{
		file >> c;
		m.setmaze(c);//passes characters into the maze to build the structure
	}

	m.displaymaze();//displays maze
	move.startpoint(m);//initializes start point
	move.movement(m);//moves until finished maze


	system("pause>nul");
}
