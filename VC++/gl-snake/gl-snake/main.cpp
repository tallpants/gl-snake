#include <iostream>

typedef struct coordinate
{
	int x;
	int y;
};

class Snake
{
	typedef struct node
	{
		coordinate data;
		node *next;
		node *prev;
	};

	node* head;
	node* tail;

public:
	int size;

	Snake()
	{
		size = 0;
		head = nullptr;
		tail = nullptr;
	}

	void push_front(coordinate c)
	{
		// Is the structure empty?
		if (size == 0)
		{
			head = new node;
			head->data.x = c.x;
			head->data.y = c.y;
			head->next = nullptr;
			head->prev = nullptr;
			tail = head;
			size += 1;
		}

		else
		{
			node* temp = new node;
			temp->data.x = c.x;
			temp->data.y = c.y;
			temp->prev = nullptr;
			head->prev = temp;
			head = temp;
			size += 1;
		}
	}

	//coordinate pop_back();

	coordinate operator [] (int index) {
		if (index == 0) {
			coordinate c = { head->data.x, head->data.y };
			return c;
		}
	}
};

void main()
{
	Snake s;
	coordinate a = { 10, 20 };
	coordinate b = { 30, 40 };
	s.push_front(a);
	s.push_front(b);
	std::cout << s[0].x << " " << s[0].y << std::endl;
	std::cin.get();
}