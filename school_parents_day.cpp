#include <iostream>
#include <cstdlib>
#include "classes.h"
#include <ctime>

using namespace std;

void set_L(int& L, int& n);
void print_parents_with_teachers(Parent** teachers, int N);

void List::insert_one_end(Parent* ptr) {
	if(next==NULL) {
		next = ptr;
	    ptr->set_prev(next);
	}
	else {
		Parent* temp = next;
		while (temp->next_get() != NULL)
			temp = temp->next_get(); //������� ��� ��������� �����
		temp->set_next(ptr);
		ptr->set_prev(temp);
	}
	ptr->set_next(NULL);
	size++;
}

void List::parent_leaves_queue(Parent* ptr) {
	Parent* temp;
	if (next == ptr) { //�� � ������ ��������� ���� ���� ��� ������
		if(ptr->next_get()==NULL) //��������� ������ ��� �����
			next = NULL;
		else {
			next = ptr->next_get();
			temp = ptr->next_get();
			temp->set_prev(next);
		}
	}
	else if (ptr->next_get()==NULL) { //�� � ������ ��������� ��� ����� ��� ������
		temp = ptr->prev_get();
	    temp->set_next(NULL);
	}
	else { //� ��������� �������� ���� � ����� ���� ���� ����������� �����
		temp = ptr->prev_get(); //��������� ��� � ������ ��� ����� ���� ���� ���� ���� ��� ����� ��� ������
		temp->set_next(ptr->next_get());
		temp = ptr->next_get();
		temp->set_prev(ptr->prev_get());
	}
}

void List::wants_teacher(int i, Parent** teachers) { //������� ��� ����� ��� ����� ����� ��� ����� �� ��������� ��� �������� i
	Parent* temp = next;
	for(int j=0; j<size; j++) { //�� ��� ������ ������, ���� � teacher[i] ����������� �� ������� �� NULL
		if (temp->visit_teacher(i) == 0) //� ������������� ������ �� ����� �� ���������� ��� teacher i
		    temp = temp->next_get();
		else {
			teachers[i] = temp; //o teacher[i] ������� ���� ����� ��� ����������
			temp->set_teachers(i); //���������� ��� temp->teachers[i]
		   	parent_leaves_queue(temp); //� ������ ������ ��� ��� ����
			size--;
			break;
		}
	}
}

void List::print(int N) { //��������� ��� ���������� �������-������ ��� ������
	cout<<"\nQueue has now "<<size<<" parents:\n"<<endl;
	Parent* temp = next;
	for (int i=0; i<size; i++) {
		temp->print(N);
	    temp = temp->next_get();
	}
}

void List::choose_parents(Parent** teachers, int N) {
	for (int i=0; i<N; i++)
		wants_teacher(i,teachers);
}

void List::insert_parents(int L, Parent** ptr) {
	static int whose_turn = 0;
	for (int i=0; i<L; i++) {
		List::insert_one_end(ptr[whose_turn]); //whose_turn=0 ���� ����
		whose_turn++; //����������� ���� ������� ����� ��� �� ���� ���� ����
	} 
}

void List::return_chosen_parents(Parent** teachers, int N, int &people_finished) {
	int check, confused;
	cout<<'\n';
	for(int i=0; i<N; i++) {
		if (teachers[i]!=NULL) { //�� � ��������� ��� ���������� ������ �����
			check = teachers[i]->is_done(N);
	        if(check==1) { //�� � ������ ���� ��������� �� ��� ���������
				people_finished++;
				cout<<"Parent "<<teachers[i]->id_get()<<" has now finished and leaves the queue\n";
				teachers[i]=NULL; //� ������� ��� main ������� ����� ���� �����
			}
			else { //�� � ������ ��� ���� ��������� �� ��� ���������
				confused = teachers[i]->is_confused(); 
				if (confused && size!=0)  //�� � ������ ����������� ��� �������� ��� ����� ������ ���� ����
					cout<<"Parent "<<teachers[i]->id_get()<<" was confused and went at the start of the queue. The other parents are complaining"<<endl;
				cout<<"Parent "<<teachers[i]->id_get()<<" goes back at the end of the queue\n";
				insert_one_end(teachers[i]); //� ������ �������� ��� ����� ��� �����
				teachers[i]=NULL; //� ��������� ������� �� NULL
			}
		}
	}
}

int main(void) {
	int L, N, K, n, people_finished=0;
	cout<<"Give me the number of Teachers: "<<endl;
	cin>>N;
	cout<<"Give me the number of Parents: "<<endl;
    cin >> K;
	Parent** ptr = new Parent*[K];
	for(int i=0; i<K; i++) 
	   ptr[i] = new Parent(i+1,N); //���������� � ������������ ������ Parent ��� ������������ ����
	cout<<"The parents who will visit Teachers today are: \n";
	for (int i=0; i<K; i++) {
		ptr[i]->print(N);
	} 
	Parent** teachers = new Parent*[N]; //���������� ������ � ������� �� ������ - ��������������� ���� ��������� - ���� ���� ��������� ���������� �����, ���� � ������� ������� �� �����
	for (int i=0; i<N; i++) {teachers[i]=NULL;}
	List queue; //���������� ��� �����. �������������� �� ������� �� 0 ��� �� next �� NULL
	srand(time(NULL)); 
	n=K;
	set_L(L,n);
	queue.insert_parents(L,ptr); //������ �������� L ������� ������ ���� ���� 
	queue.print(N); 
	while ( n || (people_finished!=K) ) { //��� ���� ��� �������� ����� ������ ��� ������ �� ����� ���� ���� ��� ����� ��� ����� ��������� ����� ���� �� �
		queue.choose_parents(teachers, N); //�������� ������ ��� �� ����������� ���������
		print_parents_with_teachers(teachers,N); //��������� ���� ������ ��� ������������� ��� ���������
		queue.print(N);
		set_L(L,n);
		queue.insert_parents(L, ptr); //�������� ������� ������� ������ ���� ����
		if(L!=0) //��������� �� ����� ���� �� ���������� ���� ������ �� ����
			queue.print(N);
		queue.return_chosen_parents(teachers,N,people_finished); //���������� ����� ���� ������ ��� ������������ ���� ���� � ��� ����� ��� ����� � ���� ������� ������� ��� ��� ����
	} 
	delete[] teachers;	//����������� ������ ��� �� heap
	for(int i=0; i<K; i++)
		delete ptr[i];
	delete[] ptr;
	cout<<"\nEnd Of School Parents' Day! "<<endl;
return 0;
}

void set_L(int& L, int& n) {
	L = rand()% (n+1); //������� ������� ������ ��� 0 ��� n ��� ���������� ���� ����
		n = n - L;
		cout<<"\n"<<L<<" Parents will now join the queue!";
	}

void print_parents_with_teachers(Parent** teachers, int N) {
	cout<<"\nParents have been chosen from the queue to visit the teachers\n";
		for(int i=0; i<N; i++) {
			if(teachers[i]==NULL)
		        cout<<"Teacher number "<<i+1<<" is not with a parent!\n";
			else { 
				cout<< "Teacher "<<i+1<<" is now with ";
				teachers[i]->print(N);
				}
		}
}
