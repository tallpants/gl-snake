#include <iostream>

struct coordinate {
	int x;
	int y;
};

class Snake {
	struct node {
		coordinate data;
		node* next;
		node* prev;
	};

	node* head;
	node* tail;

public:
	int size;

	Snake() {
		size = 0;
		head = nullptr;
		tail = nullptr;
	}

	void push_front(coordinate c) {
		if (size == 0) {
			head = new node;
			head->data.x = c.x;
			head->data.y = c.y;
			head->next = nullptr;
			head->prev = nullptr;
			tail = head;
			size++;
		}
		else {
			node* temp = new node;
			temp->data.x = c.x;
			temp->data.y = c.y;
			temp->next = head;
			temp->prev = nullptr;
			head->prev = temp;
			head = temp;
			size++;
		}
	}

	void pop_back() {
		tail = tail->prev;
		tail->next = nullptr;
		size--;
	}

	coordinate operator[] (int index) {
		if (index == 0) {
			coordinate c = { head->data.x, head->data.y };
			return c;
		}
		
		int i = 0;
		node* temp = head;
		while (i != index) {
			temp = temp->next;
			i++;
		}

		coordinate c = { temp->data.x, temp->data.y };
		return c;
	}
};

void main() {
	Snake s;
	coordinate a = { 10, 20 };
	coordinate b = { 30, 40 };
	coordinate c = { 50, 60 };

	s.push_front(a);
	s.push_front(b);
	s.push_front(c);

	for (int i = 0; i < s.size; i++) {
		std::cout << s[i].x << " " << s[i].y << std::endl;
	}
	
	std::cout << "Popping once" << std::endl << std::endl;
	s.pop_back();

	for (int i = 0; i < s.size; i++) {
		std::cout << s[i].x << " " << s[i].y << std::endl;
	}

	std::cin.get();
}