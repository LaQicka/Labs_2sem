#include "MyList.h"

template<typename T>
MyList<T>::MyList() {
	head = nullptr;
	size = 0;
}

template<typename T>
void MyList<T>::pop_back() {
	remove(size - 1);
}

template<typename T>
MyList<T>::~MyList() {
	while (size) pop_front();
	delete head;
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
T& MyList<T>::operator[](const int index) {
	int i = 0;
	Node<T>* temp = this->head;
	while (temp != nullptr) {
		if (i == index) return temp->data;
		else {
			temp = temp->next;
			i++;
		}
	}
}

template<typename T>
void MyList<T>::insert(int index, T data) {
	if (index == 0) push_front(data);
	else {
		Node<T>* temp = this->head;
		for (int i = 0; i < index - 1; i++)temp = temp->next;
		Node<T>* newNode = new Node<T>(data, temp->next);
		temp->next = newNode;
		size++;
	}
}

template<typename T>
void MyList<T>::remove(int index) {
	if (index == 0)pop_front();
	else {
		Node<T>* temp = this->head;
		for (int i = 0; i < index - 1; i++)temp = temp->next;
		Node<T>* t = temp->next;
		temp->next = t->next;
		delete t;
		size--;
	}
}

template<typename T>
void MyList<T>::push_front(T data) {
	Node<T>* temp = new Node<T>(data, head);
	head = temp;
	size++;
}

template<typename T>
void MyList<T>::pop_front() {
	Node<T>* temp = head;
	head = head->next;
	delete temp;
	size--;
}

template<typename T>
void MyList<T>::clear() {
	while (size)pop_front();
}