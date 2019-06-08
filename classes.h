#include <cstdlib>
#include <iostream>

using namespace std;

class Parent {
	int id; //� ��������� ��� ���� �����
	int* teachers; //������� ���� �������� �������� ��� ����� �� ��� � ������
	Parent* next; //������� ���� ��� ������� 
	Parent* prev; //������� ���� ��� �����������
public:
	Parent(int j, int N) { //� constructor ��� ������ Parent - ������� �� ������ ��� ��������� ��� ����� ��� ��� ������ ��� ��������� ��� ����� �� ���
		int zeros=0;
		id = j;
	    teachers = new int[N]; //������� �� ������ �� ��������� - �������� �������� �� ����� 
	    for(int i=0; i<N; i++) {
	    	if ((teachers[i] = rand()%2) == 0) //���������� ������ ���� 0 � 1 �� ���� ��������� i ��� ������
				zeros++; //��� zeros ������������ � ������� ��� ��������� ��� ������
		}
		if(zeros == N) //������� - �� ��� ����� 0, ���� ������� �������� ���� 1 �� ��� ������ ����
		   teachers[rand()%N] = 1;
		prev = NULL; 
		next = NULL;
	}
	~Parent() {delete[] teachers;} //� deconstructor ��� ������ Parent - �� ��� ���������� ��� ������������� ��� � ������� ���� ������ �� ���� ��������� ��� ���� ������������ ��������
	Parent* next_get() {return next;}
	Parent* prev_get() {return prev;}
	int id_get() {return id;}
	void set_prev(Parent* ptr) {prev=ptr;}
	void set_next(Parent* ptr) {next = ptr;}
	void print(int N) {
		cout << "Parent "<<id<<" has to visit the following teachers: ";
		for (int i=0; i<N; i++)
		    cout<<teachers[i]<<" ";
		cout << "\n";
	}
	void print() {cout<<id<<" ";}
	void set_teachers(int i) {teachers[i]=0;}
	int visit_teacher(int Teacher) { //�� � ������ �������� �� ���������� ��� ��� �����, ���������� 1, ������ ���������� 0
		if (teachers[Teacher] == 1)
		        return 1;
		else return 0;
	}
	int is_done(int N) {
		for (int i=0; i<N;  i++) {
	    	if (teachers[i]==1)
	            return 0;
		}
		return 1;
	}
	int is_confused(void) {
		int i = rand()%4; //������� ����� ������ ��� 0 ��� 3 - 25% ���������� �� ���������� � ������
	    if (i==0)
	        return 1; //��������� 1 ���� ��������� ��� � ������ ���� ����������
		else return 0;
	}
};

class List {
	int size; //������� ������ ��������
	Parent* next;
public:
	List(int s=0, Parent* ptr=NULL) :size(s), next(ptr) {cout<<"\nAn empty queue has been constructed \n";} //� constructor ��� ������ List
	int size_get() {return size;}
	~List() {cout<<"Queue destroyed\n"<<endl;} //� deconstructor ��� ������ List
	void insert_one_end(Parent* ptr);
	void insert_parents(int L, Parent** ptr);
	void parent_leaves_queue(Parent* ptr);
	void wants_teacher(int i, Parent** teachers);
	void choose_parents(Parent** teachers, int N);
	void print() {
		cout<<"\nSize of the queue= "<<size<<endl;
		cout<<"List_pointer= "<<next<<endl;}
	void print(int N);
	void return_chosen_parents(Parent** teachers, int N, int& people_finished);
};
