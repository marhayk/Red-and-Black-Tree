#pragma once
#include <iostream>
#include <fstream>
#include<string>
#include <windows.h>

using namespace std;

struct Node
{
	int data;
	char color;
	Node *left;
	Node *right;
};

class BRtree
{
private:
	Node * root;
	int count;
	bool done;
	bool childIsRed;
	bool childAndParentAreRed;
	bool uncleIsRed;
	char last, beforeLast, beforeBeforeLast;
	bool immediateParent;

	int replace;
	bool stop;
	bool found;
	bool stopPull;
	bool singleBlackNodeDeleted;
	bool fixed;

	void recinorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recinorder(temp->left);
			cout << temp->data << "   ";
			recinorder(temp->right);
		}
	}

	void recpreorder(Node *temp, int min, int mid, int max, int y)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			if (temp->color == 'r')
			{
				goTo(mid, y, 4);
			}

			else
			{
				goTo(mid, y, 10);
			}
			cout << temp->data;
			cout << " (" << temp->color << ") ";
			recpreorder(temp->left, min, (min + mid) / 2, mid, y + 1);
			recpreorder(temp->right, mid, (mid + max) / 2, max, y + 1);
		}
	}
	void recpostorder(Node *temp)
	{
		if (temp == NULL)
		{
			return;
		}
		else
		{
			recpostorder(temp->left);
			recpostorder(temp->right);
			delete temp;
		}
	}

	void addrec(int item, Node *& trav)
	{
		if (trav == NULL)
		{
			trav = new Node;
			trav->data = item;
			trav->color = 'r';
			trav->left = NULL;
			trav->right = NULL;
			childIsRed = true;
			immediateParent = true;
		}
		else if (item > trav->data)
		{
			beforeLast = last;
			last = 'R';
			addrec(item, trav->right);
			if (immediateParent && trav->color == 'r')
			{
				childAndParentAreRed = true;
				return;
			}
			else
			{
				immediateParent = false;
			}
		}
		else
		{
			beforeLast = last;
			last = 'L';
			addrec(item, trav->left);
			if (immediateParent && trav->color == 'r')
			{
				childAndParentAreRed = true;
				return;
			}
			else
			{
				immediateParent = false;
			}
		}

		if (childAndParentAreRed)
		{
			if (beforeLast == 'L')
			{
				if (trav->right == NULL || trav->right->color == 'b')
				{
					if (last == 'R') // case 2
					{
						Node *temp = trav->left;
						trav->left = temp->right;
						temp->right = NULL;
						trav->left->left = temp;

						temp = trav->left; // case 2 ->case 3
						char tempColor;
						tempColor = trav->color;
						trav->color = temp->color;
						temp->color = tempColor;
						trav->left = temp->right;
						temp->right = trav;
						trav = temp;
						childAndParentAreRed = false;
					}
					else if (last == 'L') // case 3
					{
						Node *temp = trav->left;
						char tempColor;
						tempColor = trav->color;
						trav->color = temp->color;
						temp->color = tempColor;
						trav->left = temp->right;
						temp->right = trav;
						trav = temp;
						childAndParentAreRed = false;
					}
				}
				else // case 1
				{
					trav->left->color = trav->color;
					trav->right->color = trav->color;
					trav->color = 'r';
					childAndParentAreRed = false;
					beforeLast = 'R';
				}

				if (trav->color == 'r')
				{
					immediateParent = true;
					childAndParentAreRed = false;
					childIsRed = true;
				}
				show();
			}
			else if (beforeLast == 'R')
			{
				if (trav->left == NULL || trav->left->color == 'b')
				{
					if (last == 'L') // case 2
					{
						Node *temp = trav->right;
						trav->right = temp->left;
						temp->left = NULL;
						trav->right->right = temp;

						temp = trav->right; // case 2 -> case 3
						char tempColor;
						tempColor = trav->color;
						trav->color = temp->color;
						temp->color = tempColor;
						trav->right = temp->left;
						temp->left = trav;
						trav = temp;
						childAndParentAreRed = false;
					}
					else if (last == 'R') // case 3
					{
						Node *temp = trav->right;
						char tempColor;
						tempColor = trav->color;
						trav->color = temp->color;
						temp->color = tempColor;
						trav->right = temp->left;
						temp->left = trav;
						trav = temp;
						childAndParentAreRed = false;
					}
				}
				else //case 1
				{
					trav->left->color = trav->color;
					trav->right->color = trav->color;
					trav->color = 'r';
					childAndParentAreRed = false;
					beforeLast = 'L';
				}

				if (trav->color == 'r')
				{
					immediateParent = true;
					childAndParentAreRed = false;
					childIsRed = true;
				}
				show();
			}

		}
	}

	bool case5(Node *trav, char direction)
	{
		if (direction == 'L')
		{
			if ((trav->right == NULL || trav->right->color == 'b') && (trav->right->right == NULL || trav->right->right->color == 'b') && (trav->right->left != NULL && trav->right->left->color == 'r'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (direction == 'R')
		{
			if ((trav->left == NULL || trav->left->color == 'b') && (trav->left->left == NULL || trav->left->left->color == 'b') && (trav->left->right != NULL && trav->left->right->color == 'r'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool case6(Node *trav, char direction)
	{
		if (direction == 'L')
		{
			if ((trav->right == NULL || trav->right->color == 'b') && (trav->right->right != NULL && trav->right->right->color == 'r') && (trav->right->right->right == NULL || trav->right->right->right->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (direction == 'R')
		{
			if ((trav->left == NULL && trav->left->color == 'b') && (trav->left->left != NULL && trav->left->left->color == 'r') && (trav->left->left->left == NULL || trav->left->left->left->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool case2(Node *trav, char direction)
	{
		if (direction == 'L')
		{
			if ((trav == NULL || trav->color == 'b') && (trav->right != NULL && trav->right->color == 'r') && (trav->right->left == NULL || trav->right->left->color == 'b') && (trav->right->right == NULL || trav->right->right->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (direction == 'R')
		{
			if ((trav == NULL || trav->color == 'b') && (trav->left != NULL && trav->left->color == 'r') && (trav->left->right == NULL || trav->left->right->color == 'b') && (trav->left->left == NULL || trav->left->left->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool case4(Node *trav, char direction)
	{
		if (direction == 'L')
		{
			if ((trav != NULL && trav->color == 'r') && (trav->right == NULL || trav->right->color == 'b') && (trav->right->left == NULL || trav->right->left->color == 'b') && (trav->right->right == NULL || trav->right->right->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (direction == 'R')
		{
			if ((trav != NULL && trav->color == 'r') && (trav->left == NULL || trav->left->color == 'b') && (trav->left->right == NULL || trav->left->right->color == 'b') && (trav->left->left == NULL || trav->left->left->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool case3(Node *trav, char direction)
	{
		if (direction == 'L')
		{
			if ((trav == NULL || trav->color == 'b') && (trav->right == NULL || trav->right->color == 'b') && (trav->right->left == NULL || trav->right->left->color == 'b') && (trav->right->right == NULL || trav->right->right->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (direction == 'R')
		{
			if ((trav == NULL || trav->color == 'b') && (trav->left == NULL || trav->left->color == 'b') && (trav->left->right == NULL || trav->left->right->color == 'b') && (trav->left->left == NULL || trav->left->left->color == 'b'))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	void delrec(int item, Node *&trav)
	{
		if (trav == NULL)
		{
			return;
		}
		else if (item > trav->data)
		{
			last = 'R';
			delrec(item, trav->right);
			if (singleBlackNodeDeleted)
			{
				if (case5(trav, last) && !fixed)
				{
					Node *temp1 = trav->left;
					Node *temp2 = trav->left->right;
					trav->left->right = temp2->left;
					trav->left = temp2;
					trav->left->left = temp1;
					trav->left->color = 'b';
					trav->left->left->color = 'r';
					//cout << "case5" << endl;
					show();

					trav->color = 'b'; //case 6 after case 5
					trav->left->color = 'r';
					trav->left->left->color = 'b';
					Node *temp3 = trav->left;
					Node *temp4 = trav->left->right;
					trav->left->right = trav;
					trav->left = temp4;
					trav = temp3;
					fixed = true;
					show();
				}
				else if (case6(trav, last) && !fixed)
				{
					trav->color = 'b';
					trav->left->left->color = 'b';
					trav->left->color = 'r';
					Node *temp1 = trav->left;
					Node *temp2 = trav->left->right;
					trav->left->right = trav;
					trav->left = temp2;
					trav = temp1;
					fixed = true;
					show();
					//cout << "case6" << endl;
				}
				else if (case4(trav, last) && !fixed)
				{
					trav->color = 'b';
					trav->left->color = 'r';
					fixed = true;
					//cout << "case4" << endl;
				}
				else if (case2(trav, last) && !fixed)
				{
					trav->color = 'r';
					trav->left->color = 'b';
					Node *temp1 = trav->left;
					Node *temp2 = trav->left->right;
					trav->left->right = trav;
					trav->left = temp2;
					trav = temp1;
					show();

					if (case4(trav->right, last) && !fixed)
					{
						trav->right->color = 'b';
						trav->right->left->color = 'r';
						fixed = true;
					}
					else if (case5(trav->right, last) && !fixed)
					{
						Node *temp3 = trav->right->left;
						Node *temp4 = trav->right->left->right;
						trav->right->left->right = temp4->left;
						trav->right->left = temp4;
						trav->right->left->left = temp3;
						trav->right->left->color = 'b';
						trav->right->left->left->color = 'r';

						show();
						trav->right->color = 'b'; //case 6 after case 5
						trav->right->left->color = 'r';
						trav->right->left->left->color = 'b';
						Node *temp5 = trav->right->left;
						Node *temp6 = trav->right->left->right;
						trav->right->left->right = trav->right;
						trav->right->left = temp6;
						trav->right = temp5;
						show();
						fixed = true;
					}
					//cout << "case2" << endl;
				}
				else if (case3(trav, last) && !fixed)
				{
					trav->left->color = 'r';
					show();
					//cout << "case3" << endl;
				}

			}
		}
		else if (item<trav->data)
		{
			last = 'L';
			delrec(item, trav->left);
			if (singleBlackNodeDeleted)
			{
				if (case5(trav, last) && !fixed)
				{
					Node *temp1 = trav->right;
					Node *temp2 = trav->right->left;
					trav->right->left = temp2->right;
					trav->right = temp2;
					trav->right->right = temp1;
					trav->right->color = 'b';
					trav->right->right->color = 'r';
					//cout << "case5" << endl;
					show();

					trav->color = 'b'; //case 6 after case 5
					trav->right->color = 'r';
					trav->right->right->color = 'b';
					Node *temp3 = trav->right;
					Node *temp4 = trav->right->left;
					trav->right->left = trav;
					trav->right = temp4;
					trav = temp3;
					fixed = true;
					show();
				}
				else if (case6(trav, last) && !fixed)
				{
					trav->color = 'b';
					trav->right->right->color = 'b';
					trav->right->color = 'r';
					Node *temp1 = trav->right;
					Node *temp2 = trav->right->left;
					trav->right->left = trav;
					trav->right = temp2;
					trav = temp1;
					fixed = true;
					show();
					//cout << "case6" << endl;
				}
				else if (case4(trav, last) && !fixed)
				{
					trav->color = 'b';
					trav->right->color = 'r';
					fixed = true;
					//cout << "case4" << endl;
				}
				else if (case2(trav, last) && !fixed)
				{
					trav->color = 'r';
					trav->right->color = 'b';
					Node *temp1 = trav->right;
					Node *temp2 = trav->right->left;
					trav->right->left = trav;
					trav->right = temp2;
					trav = temp1;
					show();

					if (case4(trav->left, last) && !fixed)
					{
						trav->left->color = 'b';
						trav->left->right->color = 'r';
						fixed = true;
					}
					else if (case5(trav->left, last) && !fixed)
					{
						Node *temp3 = trav->left->right;
						Node *temp4 = trav->left->right->left;
						trav->left->right->left = temp4->right;
						trav->left->right = temp4;
						trav->left->right->right = temp3;
						trav->left->right->color = 'b';
						trav->left->right->right->color = 'r';

						show();
						trav->left->color = 'b'; //case 6 after case 5
						trav->left->right->color = 'r';
						trav->left->right->right->color = 'b';
						Node *temp5 = trav->left->right;
						Node *temp6 = trav->left->right->left;
						trav->left->right->left = trav->left;
						trav->left->right = temp6;
						trav->left = temp5;
						show();
						fixed = true;
					}
					//cout << "case2" << endl;
				}
				else if (case3(trav, last) && !fixed)
				{
					trav->right->color = 'r';
					show();
					//cout << "case3" << endl;
				}

			}
		}
		else
		{
			found = true;
			if (trav->left == NULL && trav->right == NULL)
			{
				if (trav->color == 'r')
				{
					delete trav;
					trav = NULL;
				}
				else
				{
					delete trav;
					trav = NULL;
					singleBlackNodeDeleted = true;
				}
			}
			else if (trav->left != NULL && trav->right != NULL)
			{
				Node *trav2 = trav->left;
				while (trav2->right != NULL)
				{
					trav2 = trav2->right;
				}
				trav->data = trav2->data;
				delrec(trav->data, trav->left);
			}
			else if (trav->left == NULL && trav->right != NULL)
			{
				Node *trav2 = trav->right;
				trav->data = trav2->data;
				delrec(trav->data, trav->right);
			}
			else if (trav->right == NULL && trav->left != NULL)
			{
				Node *trav2 = trav->left;
				trav->data = trav2->data;
				delrec(trav->data, trav->left);
			}
		}
	}

	void goTo(int x, int y, int col)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos;
		pos.X = x; //Column
		pos.Y = y; //Row
		SetConsoleCursorPosition(hConsole, pos);
		SetConsoleTextAttribute(hConsole, col);
	}


public:
	BRtree()
	{
		root = NULL;
		count = 0;
	}

	~BRtree()
	{
		clear();
		//delete[] nums;
	}
	void add(int item)
	{
		childIsRed = false;
		childAndParentAreRed = false;
		uncleIsRed = false;
		immediateParent = false;
		last = beforeLast = 'L';
		//stopPull = false;
		addrec(item, root);
		root->color = 'b';
		count++;
	}


	void show()
	{
		system("cls");
		int minX = 0;
		int midX = 40;
		int maxX = 80;
		int y = 0;
		recpreorder(root, minX, midX, maxX, y);
		//system("pause");
	}

	void inorder()
	{
		recinorder(root);
		cout << endl;
		system("pause");
	}

	void clear()
	{
		recpostorder(root);
		root = NULL;
		count = 0;
	}


	Node* search(int item, Node **&parentPtr)
	{
		Node *trav = root;
		parentPtr = &root;
		Node *search_node = new Node;
		search_node->data = item;
		search_node->left = NULL;
		search_node->right = NULL;

		while (trav != NULL)
		{
			if (search_node->data > trav->data)
			{
				parentPtr = &(trav->right);
				trav = trav->right;
			}
			else if (search_node->data < trav->data)
			{
				parentPtr = &(trav->left);
				trav = trav->left;
			}
			else
			{
				return trav;
			}
		}
		return NULL;
	}

	void deleteItem(int item)
	{
		stop = false;
		found = false;
		last = beforeLast = beforeBeforeLast = 'L';
		singleBlackNodeDeleted = false;
		fixed = false;
		delrec(item, root);
		root->color = 'b';
		if (!found)
		{
			cout << "No such item found" << endl;
		}
	}
};