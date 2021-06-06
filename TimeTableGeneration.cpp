#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<string.h>
#include<conio.h>
#include<windows.h>
using namespace std;

//-----------Class Time Table ADT and Nodes---------------//
struct classptr
{
	int count = 0;
	char names[60][15]; //60 students in class, first name only.
	char rollNo[60][10];
};
struct node2
{
	string subject;
	classptr*ptr = nullptr;									// store the address of next or previous class
};
struct nodelab1
{
	string labname;
	node2 course[4];
	nodelab1*up = nullptr;
};
struct nodeclass1
{
	string room;
	node2 course[8];
	nodeclass1*down = nullptr;
};
struct node
{
	string day;
	node*next = nullptr;
	nodeclass1*down = nullptr;      //for classes
	nodelab1*up = nullptr;     //for labs
};
class TTADT
{
	node*first;
	string classtiming[8];
	string labtiming[4];
public:
	int index1 = 0;
	int index2 = 0;
	int indexlab = 0;
	TTADT(){ first = nullptr; }
	//-----------Part-A:
	void load(char filename[]);
	//-----------Part-B:
	void loadStudentInfo(char filename[]);
	~TTADT();
	//----------Part-C:
	void clashes(char rollno[], int clashIndex, node*curr, nodeclass1*currclass, ofstream& obj);
	void saveClashes();
	void printStudentTimeTable(char rollno[]);
	void printStudentCourses(char rollno[]);
	void printCourse(char day[], char time[], char classroom[]);
	void printCourseTimings(char coursename[], char section[]);
	//---------Searching:
	nodeclass1*searchCourseNameLocation(string&);
	nodelab1*searchCourseNameLocationLab(string&, string&);
	nodeclass1*search2ndCourseNameLocation(string&);
	node*search2ndSubjectDayLocation(nodeclass1*);
	node*searchDay(string&);
	//---------Inserting:
	void insert(istream&);
	void insertclases(istream&);
	void insertlabs(istream&);
	//---------Printing:
	void print();
	void printStudentInfo();
	void printStudentLabs(char rollno[]);
	void printStudentLabsTimeTable(char rollno[]);
	bool printLabCourseTimings(char coursename[], char section[]);
	void printDays();

};
//---------------Global Helping Functions-------------------//
string cat(string str1, string str2)
{
	string str3 = " (";
	string str4 = ")";
	//cout << str1 << " " << str2;
	string str5 = str1 + str3 + str2 + str4;
	return str5;
}

//---------------Search Helping Functions-------------------//
nodeclass1*TTADT::searchCourseNameLocation(string& str)
{
	node*curr = first;
	while (curr)
	{
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			for (int i = 0; i < 8; i++)
			{
				if (currclass->course[i].subject.compare(str) == 0)
				{
					index1 = i;
					return currclass;
				}
			}
			currclass = currclass->down;
		}
		curr = curr->next;
	}
	return 0;
}
nodelab1*TTADT::searchCourseNameLocationLab(string& str1, string & str2)
{
	node*curr = first;
	while (curr)
	{
		string lab = "Lab";
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			for (int i = 0; i < 4; i++)
			{
				int a = currlab->course[i].subject.find(str1);
				int b = currlab->course[i].subject.find(str2);
				int c = currlab->course[i].subject.find(lab);
				if (a != -1 && b != -1 && c != -1)
				{
					//cout <<"			"<< currlab->course[i].subject.find(str1) << endl;
					indexlab = i;
					return currlab;
				}
			}
			currlab = currlab->up;
		}
		curr = curr->next;
	}
	return 0;
}
nodeclass1*TTADT::search2ndCourseNameLocation(string& str)        //return the address of 2nd subject 
{
	int count = 0;
	node*curr = first;
	while (curr)
	{
		nodeclass1*curr1 = curr->down;
		while (curr1)
		{
			for (int i = 0; i < 8; i++)
			{
				if (str.compare(curr1->course[i].subject) == 0)
				{
					count++;
					if (count == 2)
					{
						index2 = i;
						return curr1;
					}
				}
			}
			curr1 = curr1->down;
		}
		curr = curr->next;
	}
	return 0;
}
node*TTADT::search2ndSubjectDayLocation(nodeclass1*found1)      //return the address of 2nd subject day 
{
	node*curr = first;
	while (curr)
	{
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			if (currclass->down == found1)
			{
				return curr;
			}
			else
			{
				currclass = currclass->down;
			}
		}
		curr = curr->next;
	}
	return 0;
}
node*TTADT::searchDay(string& str)
{
	node*curr = first, *find = 0;
	int flag = 0;
	while (curr)
	{
		if (str.compare(curr->day) == 0)
		{
			flag = 1;
			find = curr;
		}
		curr = curr->next;
	}
	if (flag)
	{
		return find;
	}
	return 0;
}

//---------------Insert Helping Functions-------------------//
void TTADT::insert(istream& obj)
{
	node*temp = new node;
	string read;
	getline(obj, read, ',');
	temp->day = read;
	//cout <<"\n\n-------------------------"<< temp->day << "---------------------------------------\n\n";
	if (!first)
	{
		first = temp;
	}
	else
	{
		node*curr = first;
		while (curr->next)
		{
			curr = curr->next;
		}
		curr->next = temp;
	}
}
void TTADT::insertclases(istream& obj)
{
	string read;
	nodeclass1*tempclass = new nodeclass1;
	getline(obj, read, ',');					// reading room
	tempclass->room = read;
	//cout <<"\n\n-----------------------------------"<< tempclass->room << "-------------------------------\n\n";
	for (int i = 0; i < 8; i++)
	{
		getline(obj, read, ',');
		if (tempclass->course)
		{
			tempclass->course[i].subject = read;
			//cout << tempclass->course[i].subject << endl;
		}
		else
		{
			cout << read << "Not Registered\n";
		}
		//cout<<tempclass->course[i].subject<< "		";
	}
	if (!first->next)
	{
		if (!first->down)
		{
			first->down = tempclass;
		}
		else
		{
			nodeclass1*curr1 = first->down;
			while (curr1->down)
			{
				curr1 = curr1->down;
			}
			curr1->down = tempclass;

		}
	}
	else
	{
		node*curr = first;
		while (curr->next)
		{
			curr = curr->next;
		}
		if (!curr->down)
		{
			curr->down = tempclass;
		}
		else
		{
			nodeclass1*curr1 = curr->down;
			while (curr1->down)
			{
				curr1 = curr1->down;
			}
			curr1->down = tempclass;
		}
	}
}
void TTADT::insertlabs(istream& obj)
{
	string read;
	nodelab1*templab = new nodelab1;
	getline(obj, read, ',');					// reading room
	templab->labname = read;
	//cout << "\n\n-----------------------------------" << templab->labname << "-------------------------------\n\n";
	for (int i = 0; i < 4; i++)
	{
		getline(obj, read, ',');
		if (templab->course)
		{
			templab->course[i].subject = read;
			//cout << templab->course[i].subject << endl;
		}
		else
		{
			cout << read << "Not registered in Lab\n";
		}
		//cout << templab->course[i].subject << "		";
	}
	if (!first->next)
	{
		if (!first->up)
		{
			first->up = templab;
		}
		else
		{
			nodelab1*currlab = first->up;
			while (currlab->up)
			{
				currlab = currlab->up;
			}
			currlab->up = templab;

		}
	}
	else
	{
		node*curr = first;
		while (curr->next)
		{
			curr = curr->next;
		}
		if (!curr->up)
		{
			curr->up = templab;
		}
		else
		{
			nodelab1*currlab = curr->up;
			while (currlab->up)
			{
				currlab = currlab->up;
			}
			currlab->up = templab;
		}
	}
}

//---------------Print Helping Functions--------------------//
void TTADT::print()
{
	node*curr = first;
	while (curr)
	{
		cout << "\n\n\n-------------------------------------" << curr->day << "--------------------------------------------------------\n\n\n";
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			cout << endl << currclass->room << "::::::::";
			for (int i = 0; i < 8; i++)
			{
				cout << currclass->course[i].subject << "		";
			}
			currclass = currclass->down;
		}
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			cout << endl << currlab->labname << "::::::::";
			for (int i = 0; i < 4; i++)
			{
				cout << currlab->course[i].subject << "		";
			}
			currlab = currlab->up;
		}
		curr = curr->next;
	}
}
void TTADT::printStudentInfo()
{
	node*curr = first;
	while (curr)
	{
		cout << "\n\n##############################################" << curr->day << "##########################################################\n\n";
		nodeclass1*currClass = curr->down;
		while (currClass)
		{
			cout << "\n******************************************" << currClass->room << "*************************************************\n";
			for (int i = 0; i < 8; i++)
			{
				cout << "\n::::::::::::::::::::::::::::" << currClass->course[i].subject << "::::::::::::::::::::::::::::::::::::\n";
				if (currClass->course[i].ptr != nullptr)
				{
					cout << "\n----------------------Total No of students Registered are " << currClass->course[i].ptr->count << " -----------------------\n";
					int count = currClass->course[i].ptr->count;
					//cout << "\n::::::::::::::::::::::::::::" << currClass->course[i].subject << "(" << count << ")" << "::::::::::::::::::::::::::::::::::::\n";
					for (int k = 0; k < count; k++)
					{
						cout << k + 1 << "		" << currClass->course[i].ptr->names[k] << "     " << currClass->course[i].ptr->rollNo[k] << endl;
					}
				}
				else
				{
					cout << "\n-----------No Student Regisstered in this Course----------------\n";
				}
			}
			currClass = currClass->down;
		}
		curr = curr->next;
	}
}
void TTADT::printStudentLabs(char rollno[])
{
	cout << "\n\n--------------------You Have Been Registered in Following Labs------------------------\n\n";
	int flag = 0, flag1 = 0;
	node*curr = first;
	while (curr)
	{
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			for (int i = 0; i < 4; i++)
			{
				if (currlab->course[i].ptr)
				{
					int count = currlab->course[i].ptr->count;
					for (int k = 0; k < count; k++)
					{
						if (strcmp(currlab->course[i].ptr->rollNo[k], rollno) == 0)
						{
							flag = 1;
							k = count;
						}
					}
					if (flag)
					{
						cout << currlab->course[i].subject << endl;
						flag = 0;
						flag1 = 1;
					}
				}
			}
			currlab = currlab->up;
		}
		curr = curr->next;
	}
	if (!flag1)
	{
		cout << "\n\n---------------------------------No Record Found!--------------------------\n\n";
	}
}
void TTADT::printStudentLabsTimeTable(char rollno[])
{
	cout << "\n\n-------------------------------Your labs Time Table is-----------------------------------\n\n";
	int flag = 0;
	node*curr = first;
	while (curr)
	{
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			for (int i = 0; i < 4; i++)
			{
				if (currlab->course[i].ptr)
				{
					int count = currlab->course[i].ptr->count;
					for (int k = 0; k < count; k++)
					{
						if (strcmp(currlab->course[i].ptr->rollNo[k], rollno) == 0)
						{
							cout << "\n\n" << currlab->course[i].subject << ":" << endl;
							cout << curr->day << " " << labtiming[i] << " , " << currlab->labname << "\n\n";
							flag = 1;
						}
					}
				}
			}
			currlab = currlab->up;
		}
		curr = curr->next;
	}
	if (!flag)
	{
		cout << "\n\n---------------------------------No Record Found!--------------------------\n\n";
	}
}
bool TTADT::printLabCourseTimings(char coursename[], char section[])
{
	string s1 = coursename, s2 = section;
	nodelab1*foundlab = searchCourseNameLocationLab(s1, s2);
	//cout << foundlab->labname<<foundlab->course[indexlab].subject;
	node*curr = first, *found = 0;
	while (curr)
	{
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			if (currlab == foundlab)
			{
				found = curr;
			}
			currlab = currlab->up;
		}
		curr = curr->next;
	}
	if (found)
	{
		cout << found->day << " " << labtiming[indexlab] << "\n\n";
		return true;
	}
	return false;
}
void TTADT::printDays()
{
	node*curr = first;
	while (curr)
	{
		cout << curr->day << "      ";
		curr = curr->next;
	}
}

//-----------------------Part-A-----------------------------//
void TTADT::load(char filename[])
{
	// Reading Monday-Friday
	ifstream obj;
	obj.open(filename);
	string read;
	getline(obj, read, ',');
	for (int i = 0; i < 8; i++)
	{
		getline(obj, read, ',');
		classtiming[i] = read;
		//cout << classtiming[i] << "		";
	}
	cout << endl;
	getline(obj, read, ',');
	for (int i = 0; i < 4; i++)
	{
		getline(obj, read, ',');
		labtiming[i] = read;
		//cout << labtiming[i] << "		";
	}
	for (int j = 0; j < 5; j++)
	{
		getline(obj, read, ',');
		this->insert(obj);
		for (int i = 0; i < 19; i++)
		{
			getline(obj, read, ',');
			this->insertclases(obj);
		}
		for (int i = 0; i < 8; i++)
		{
			getline(obj, read, ',');
			this->insertlabs(obj);
		}
	}
}

//-----------------------Part-B-----------------------------//
void TTADT::loadStudentInfo(char filename[])
{
	ifstream obj;
	obj.open(filename);
	for (int k = 0; k < 7761; k++)   //7761
	{
		string read1, read2, read3, read4;
		getline(obj, read1, ',');
		getline(obj, read1, ',');
		getline(obj, read2, ',');
		getline(obj, read3, ',');
		getline(obj, read4, ',');
		string read5 = cat(read3, read4);
		nodeclass1*foundclass = searchCourseNameLocation(read5);
		// (foundclass)
		//cout << foundclass->course[index1].subject << endl;
		//cout << foundclass << "       ";
		nodelab1*foundlab = searchCourseNameLocationLab(read3, read4);
		//if (foundlab)
		//cout << foundlab->course[indexlab].subject << endl;
		//cout << foundlab << "       ";
		if (foundclass)
		{
			//cout << "\nCourse mill giya\n";
			if (foundclass->course[index1].ptr == nullptr)
			{
				foundclass->course[index1].ptr = new classptr;
			}
			int j = foundclass->course[index1].ptr->count;
			int i = 0;
			for (; read2[i] != '\0'; i++)
			{
				foundclass->course[index1].ptr->names[j][i] = read2[i];
			}
			foundclass->course[index1].ptr->names[j][i] = '\0';
			foundclass->course[index1].ptr->names[j][i + 1] = '\0';
			int k = 0;
			for (; read1[k] != '\0'; k++)
			{
				foundclass->course[index1].ptr->rollNo[j][k] = read1[k];
			}
			foundclass->course[index1].ptr->rollNo[j][k] = '\0';
			foundclass->course[index1].ptr->rollNo[j][k + 1] = '\0';
			foundclass->course[index1].ptr->count = foundclass->course[index1].ptr->count + 1;
		}
		else if (foundlab && !foundclass)
		{
			//cout <<read5<< "             lab mill gi\n";
			if (foundlab->course[indexlab].ptr == nullptr)
			{
				foundlab->course[indexlab].ptr = new classptr;
			}
			int j = foundlab->course[indexlab].ptr->count;
			int i = 0;
			for (; read2[i] != '\0'; i++)
			{
				foundlab->course[indexlab].ptr->names[j][i] = read2[i];
			}
			foundlab->course[indexlab].ptr->names[j][i] = '\0';
			foundlab->course[indexlab].ptr->names[j][i + 1] = '\0';
			int k = 0;
			for (; read1[k] != '\0'; k++)
			{
				foundlab->course[indexlab].ptr->rollNo[j][k] = read1[k];
			}
			foundlab->course[indexlab].ptr->rollNo[j][k] = '\0';
			foundlab->course[indexlab].ptr->rollNo[j][k + 1] = '\0';
			foundlab->course[indexlab].ptr->count = foundlab->course[indexlab].ptr->count + 1;
		}
		else
		{
			cout << endl << read5 << "		" << "Not found!\n";
		}
	}
}
TTADT::~TTADT()
{
	node* curr = first;
	while (first != nullptr)
	{
		nodeclass1*firstclass = first->down;
		nodeclass1*currclass = firstclass;
		while (firstclass)
		{
			firstclass = firstclass->down;
			//cout << "Deleting " << currclass->room << endl;
			for (int i = 0; i < 8; i++)
			{
				if (currclass->course[i].ptr)
					delete currclass->course[i].ptr;   //deleting classptr ptr
			}
			if (currclass)
			{
				delete currclass;
				currclass = firstclass;
			}
		}
		nodelab1*firstlab = first->up;
		nodelab1*currlab = firstlab;
		while (firstlab)
		{
			firstlab = firstlab->up;
			//cout << "Deleting " << currclass->room << endl;
			for (int i = 0; i < 4; i++)
			{
				if (currlab->course[i].ptr)
					delete currlab->course[i].ptr;     //deleting classptr ptr
			}
			if (currlab)
			{
				delete currlab;
				currlab = firstlab;
			}
		}
		first = first->next;
		//cout << curr->day << endl;
		if (curr)
		{
			delete curr;
			curr = first;
		}
	}
}

//-----------------------Part-C-----------------------------//
int count1 = 1;
void TTADT::clashes(char rollno[], int clashIndex, node*curr, nodeclass1* currClaSs, ofstream &obj)
{
	if (curr)
	{
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			if (currclass->course[clashIndex].ptr)
			{
				int count = currclass->course[clashIndex].ptr->count;
				for (int i = 0; i < count; i++)
				{
					if (strcmp(rollno, currclass->course[clashIndex].ptr->rollNo[i]) == 0 && currclass->course[clashIndex].subject.compare(currClaSs->course[clashIndex].subject))
					{
						cout << "count#" << count1 << "		" << rollno << "		" << currclass->course[clashIndex].subject;
						cout << "		" << classtiming[clashIndex] << "    \n";
						cout << "		        " << currclass->course[clashIndex].ptr->rollNo[i] << "		";
						cout << currClaSs->course[clashIndex].subject << "			";
						cout << classtiming[clashIndex] << endl << endl << endl;
						obj << rollno << "	" << currclass->course[clashIndex].ptr->names[i] << "		";
						obj << currclass->course[clashIndex].subject << "	";
						obj << currClaSs->course[clashIndex].subject << "	";
						obj << curr->day << "	";
						obj << classtiming[clashIndex] << endl << endl;
						count1++;
					}
				}
			}
			currclass = currclass->down;
		}
		int labclashindex = clashIndex / 2;
		nodelab1*currlab = curr->up;
		while (currlab)
		{
			if (currlab->course[labclashindex].ptr)
			{
				int count = currlab->course[labclashindex].ptr->count;
				for (int i = 0; i < count; i++)
				{

					if (strcmp(rollno, currlab->course[labclashindex].ptr->rollNo[i]) == 0 && currlab->course[labclashindex].subject.compare(currClaSs->course[clashIndex].subject))
					{
						cout << "count#" << count1 << "		" << rollno << "		" << currlab->course[labclashindex].subject;
						cout << "		" << labtiming[labclashindex] << "    \n";
						cout << "		        " << currClaSs->course[clashIndex].ptr->rollNo[i] << "		";
						cout << currClaSs->course[clashIndex].subject << "			";
						cout << classtiming[clashIndex] << endl << endl << endl;
						obj << rollno << "	" << currlab->course[labclashindex].ptr->names[i] << "		";
						obj << currlab->course[labclashindex].subject << "	";
						obj << currClaSs->course[clashIndex].subject << "	";
						obj << curr->day << "	";
						obj << labtiming[labclashindex] << "\n\n";
						count1++;
					}
				}
			}
			currlab = currlab->up;
		}
	}
}
void TTADT::saveClashes()
{
	cout << "\n\n-------------------------------Clashes-----------------------------------\n\n";
	ofstream obj("clashes.text");
	obj.clear();
	node*curr = first;
	while (curr)
	{
		nodeclass1*currclass = curr->down;
		//nodelab1*currlab = curr->up;
		while (currclass)
		{
			for (int i = 0; i < 8; i++)
			{
				if (currclass->course[i].ptr)
				{
					for (int j = 0; j < currclass->course[i].ptr->count; j++)
					{
						clashes(currclass->course[i].ptr->rollNo[j], i, curr, currclass, obj);
					}
				}
			}
			currclass = currclass->down;
		}
		curr = curr->next;
	}
	obj.close();
}
void TTADT::printStudentTimeTable(char rollno[])
{
	cout << "\n\n-------------------------------Your Time Table is-----------------------------------\n\n";
	int flag = 0, flag1 = 0;
	node*curr = first;
	while (curr)
	{
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			for (int i = 0; i < 8; i++)
			{
				if (currclass->course[i].ptr)
				{
					int count = currclass->course[i].ptr->count;
					for (int k = 0; k < count; k++)
					{
						if (strcmp(currclass->course[i].ptr->rollNo[k], rollno) == 0)
						{
							flag = 1;
							k = count;
						}
					}
					if (flag)
					{
						node*nextDay = 0;
						nodeclass1*nextSearch = search2ndCourseNameLocation(currclass->course[i].subject);
						if (nextSearch)
							nextDay = search2ndSubjectDayLocation(nextSearch);
						if (nextDay)
						{
							cout << "\n\n" << currclass->course[i].subject << ":" << endl;
							cout << curr->day << " " << classtiming[i] << " , " << currclass->room << "||";
							cout << nextDay->day << " " << classtiming[i] << " , " << currclass->room << "\n\n";
							flag = 0;
							flag1 = 1;
						}
					}
				}
			}
			currclass = currclass->down;
		}
		curr = curr->next;
	}
	if (!flag1)
	{
		cout << "\n\n---------------------------------No Record Found!--------------------------\n\n";
	}
	printStudentLabsTimeTable(rollno);
}
void TTADT::printStudentCourses(char rollno[])
{
	cout << "\n\n--------------------You Have Been Registered in Following Courses------------------------\n\n";
	int flag = 0, flag1 = 0;
	node*curr = first;
	while (curr)
	{
		nodeclass1*currclass = curr->down;
		while (currclass)
		{
			for (int i = 0; i < 8; i++)
			{
				if (currclass->course[i].ptr)
				{
					int count = currclass->course[i].ptr->count;
					for (int k = 0; k < count; k++)
					{
						if (strcmp(currclass->course[i].ptr->rollNo[k], rollno) == 0)
						{
							flag = 1;
							k = count;
						}
					}
					if (flag)
					{
						cout << currclass->course[i].subject << endl;
						flag = 0;
						flag1 = 1;
					}
				}
			}
			currclass = currclass->down;
		}
		curr = curr->next;
	}
	if (!flag1)
	{
		cout << "\n\n---------------------------------No Record Found!--------------------------\n\n";
	}
	printStudentLabs(rollno);
	cout << "\n\n\n";
}
void TTADT::printCourse(char day[], char time[], char classroom[])
{
	cout << "\n\n--------------------------Your Required Course is---------------------------------\n\n";
	string s1 = day;
	string s2 = time;
	string s3 = classroom;
	node*curr = searchDay(s1);
	if (curr)
	{
		int flagclass = -1, flagclass1 = 0;
		int flaglab = -1, flaglab1 = 0;         // same index1 of time and course#
		for (int i = 0; i < 8; i++)
		{
			if (s2.compare(classtiming[i]) == 0)
			{
				flagclass = i;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (s2.compare(labtiming[i]) == 0)
			{
				flaglab = i;
			}
		}
		if (flagclass >= 0 || flaglab >= 0)
		{
			if (flagclass >= 0)
			{
				nodeclass1*currClass = curr->down;
				while (currClass)
				{
					if (s3.compare(currClass->room) == 0)
					{
						//cout << "The course Studed on " << s1 << " in Room ";
						//cout << s3 << " from " << s2 << " is ";
						cout << currClass->course[flagclass].subject << endl;
						currClass = 0;
						flagclass1 = 1;
					}
					else
					{
						currClass = currClass->down;
					}
				}
			}
			else if (flaglab >= 0)
			{
				nodelab1*currlab = curr->up;
				while (currlab)
				{
					if (s3.compare(currlab->labname) == 0)
					{
						//cout << "The course Studed on " << s1 << " in Room ";
						//cout << s3 << " from " << s2 << " is ";
						cout << currlab->course[flaglab].subject << endl;
						currlab = 0;
						flaglab1 = 1;
					}
					else
					{
						currlab = currlab->up;
					}
				}
			}
			if (!flaglab1 && !flagclass1)
			{
				cout << "\n-------------Room Does Not Exist in Time Table-------------------\n";
			}
		}
		else
		{
			cout << "\n-------------Time Does Not Exist in Time Table-------------------\n";
		}

	}
	else
	{
		cout << "\n-------------Day Does Not Exist in Time Table-------------------\n";
	}
}
void TTADT::printCourseTimings(char coursename[], char section[])
{
	cout << "\n\n------------------------------You Required course timing is------------------------------\n\n";
	string s1 = coursename, s2 = section;
	string s3 = cat(s1, s2);
	nodeclass1*found1 = searchCourseNameLocation(s3);
	node*curr = first, *found = 0, *found2 = 0;;

	if (found1)
	{
		while (curr)
		{
			nodeclass1*currClass = curr->down;
			while (currClass)
			{
				if (currClass->down == found1)
				{
					currClass = 0;
				}
				else
				{
					currClass = currClass->down;
				}
			}
			if (!currClass)
			{
				found = curr;
				curr = 0;
			}
			else
				curr = curr->next;
		}
		nodeclass1*found3 = search2ndCourseNameLocation(s3);
		if (found3)
		{
			curr = first;
			while (curr)
			{
				nodeclass1*currClass = curr->down;
				while (currClass)
				{
					if (currClass->down == found3)
					{
						currClass = 0;
						found2 = curr;
						curr = 0;
					}
					else
					{
						currClass = currClass->down;
					}
				}
				if (curr != 0)
				{
					curr = curr->next;
				}
			}
			if (found && found2)
			{
				cout << found->day << " " << classtiming[index1] << endl;
				cout << found2->day << " " << classtiming[index2] << "\n\n";
			}
		}
	}
	else if (printLabCourseTimings(coursename, section))
	{
		//printLabCourseTimings(coursename, section);
	}
	else
	{
		cout << "\n----------------------You provide wrong information!--------------------------\n";
	}
}

//--------------------------Main()-------------------------//
int main()
{
	system("mode con: lines=1000 cols=150");
	TTADT t1;

	//loding TimeTable File 
	{
		char TTfile[] = "timetable.csv";
		t1.load(TTfile);
		//t1.print();
	}

	//loading StudentInfo file()
	{
		char RDfile[] = "Registration Data.csv";
		t1.loadStudentInfo(RDfile);
		//t1.printStudentInfo();
	}

	//test function printstudenttimetable()
	{
		char rollno[] = { "270" };
		t1.printStudentTimeTable(rollno);
	}

	//test function printstudentCourses()
	{
		char rollno[] = { "20" };
		t1.printStudentCourses(rollno);
	}
	//test Function PrintCourse():
	{
		char _day[] = { "Friday" };
		char _time[] = { "11:00-2:00" };
		char _classroom[] = { "Lab(CS-1)" };
		t1.printCourse(_day, _time, _classroom);
	}

	//test funtion printCouseTiming():
	{
		char _coursename[] = { "Computer Networks Lab" };
		char _section[] = { "CS-B1" };
		t1.printCourseTimings(_coursename, _section);
	}

	//test clashes
	t1.saveClashes();

	cout << endl << endl << endl;
	system("pause");
}