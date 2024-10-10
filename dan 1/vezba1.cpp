#include <iostream>

struct Node {
	Node() {}
	Node(int e, Node* n) : elem(e), next(n) {}
	int elem = 0;
	Node* next = nullptr;

	// implementirao sam unutar strukture
	struct iterator{
		iterator() {};
		iterator(Node* x) : p(x) {}
		Node* p = nullptr;

		// svi kao member funkcije
		bool operator== (iterator& it) {
			return p == it.p;
		}
		bool operator!= (iterator& it) {
			return p != it.p;
		}

		int& operator * () {
			return p->elem;
		}

		// prefiksni operator
		void operator++ () {
			p = p->next;
		}

		void operator++ (int) {
			p = p->next;
		}
	};

	iterator begin() {
		return iterator(this);
	}

	iterator end() {
		return iterator(nullptr);
	}
};

void add(Node& n, int x) {
	n.next = new Node(x, n.next);
}

int main() {
	Node head;
	add(head, 1);
	add(head, 2);
	add(head, 3);
	add(head, 4);
	add(head, 5);

	for (Node* p = head.next; p != nullptr; p = p->next) {
		std::cout << p->elem << " ";
	}

	std::cout << std::endl;

	for (int x : head) {
		std::cout << x << " ";
	}

	std::cout << std::endl;

	for (int& x : head) {
		x = 7;
	}

	for (const int& x : head) {
		std::cout << x << " ";
	}

	std::cout << std::endl;

	return 0;

}
