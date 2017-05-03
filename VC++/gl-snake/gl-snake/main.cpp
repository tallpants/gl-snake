// Storage container for coordinates of the blocks forming the snake's
// body and the food.
struct coordinate {
	int x;
	int y;
};

// Partial deque implementation that encapsulates the snake's body
class Snake {
	struct node {
		coordinate data;
		node* next;
		node* prev;
	};

	node* head;
	node* tail;

public:
	// Current size of the snake's body
	int size;

	// Default constructor for empty snake
	Snake() {
		size = 0;
		head = nullptr;
		tail = nullptr;
	}

	// Push a block onto the head of the snake
	void push_front(coordinate c) {
		// Is this the first block of the snake?
		if (size == 0) {
			head = new node;
			head->data.x = c.x;
			head->data.y = c.y;
			head->next = nullptr;
			head->prev = nullptr;
			tail = head;
			size++;
		} else {
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

	// Pop the rearmost block off of the snake's body
	void pop_back() {
		tail = tail->prev;
		tail->next = nullptr;
		size--;
	}

	// Access the blocks of the snake using the subscript operator
	coordinate operator[] (int index) {
		// The first block of the snake is pointed to by head
		if (index == 0) {
			coordinate c = { head->data.x, head->data.y };
			return c;
		}

		// Traverse the structure till you get to the right node
		node* temp = head;
		int i = 0;
		while (i != index) {
			temp = temp->next;
			i++;
		}

		coordinate c = { temp->data.x, temp->data.y };
		return c;
	}
};