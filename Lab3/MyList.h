#pragma once

template<typename T>
class MyList 
{
public:
	MyList();
	~MyList();

private:
	template<typename T>
	struct Node {
		Node* next;
		T data;

		Node(T data = T(), Node* next = nullptr) {
			this->data = data;
			this->next = next;
		}
	};

public:	
	Node<T>* head;
	int size;

	void push_back(T data);
	int get_size();
	T& operator[](const int index);
	void pop();
};