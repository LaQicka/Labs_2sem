#include "MyList.h"

template<typename T>
MyList<T>::MyList() {
	head = nullptr;
	size = 0;
}

template<typename T>
void MyList<T>::pop() {
	Node<T>* temp = head;
	head = temp->next;
	delete temp;
	size--;
}

template<typename T>
MyList<T>::~MyList() {
	/*while (head->next != nullptr) pop();
	delete head;*/
}

template<typename T>
void MyList<T>::push_back(T data) {
	if (head == nullptr) head = new Node<T>(data);
	else {
		Node<T>* temp = this->head;
		while (temp->next != nullptr) temp = temp->next;
		temp->next = new Node<T>(data);
	}
	size++;
}

template<typename T>
int MyList<T>::get_size() {
	return size;
}

template<typename T>
T& MyList<T>::operator[](const int index) {
	Node<T>* temp = this->head;
	int i = 0;
	while (temp != nullptr) {
		if (i == index) return temp->data;
		temp = temp->next;
		i++;
	}
}