#include <cstdlib>
#include <iostream>

using namespace std;

class Parent {
	int id; //η ταυτότητα του κάθε γονέα
	int* teachers; //δείκτης στον εκάστοτε καθηγητή που θέλει να δει ο γονέας
	Parent* next; //δείκτης προς τον επόμενο 
	Parent* prev; //δείκτης προς τον προηγούμενο
public:
	Parent(int j, int N) { //η constructor της κλάσης Parent - παίρνει ως όρισμα την ταυτότητα του γονέα και τον αριθμό των καθηγητών που θέλει να δει
		int zeros=0;
		id = j;
	    teachers = new int[N]; //δείκτης σε πίνακα με καθηγητές - δεσμεύει δυναμικά τη μνήμη 
	    for(int i=0; i<N; i++) {
	    	if ((teachers[i] = rand()%2) == 0) //Εκχωρείται τυχαία τιμή 0 ή 1 σε κάθε στοιχείου i του πίνακα
				zeros++; //Στο zeros αποθηκεύεται ο αριθμός των μηδενικών του πίνακα
		}
		if(zeros == N) //έλεγχος - αν όλα είναι 0, τότε γίνεται προσθήκη ενός 1 σε μία τυχαία θέση
		   teachers[rand()%N] = 1;
		prev = NULL; 
		next = NULL;
	}
	~Parent() {delete[] teachers;} //η deconstructor της κλάσης Parent - με την καταστροφή της καταστρέφεται και ο δείκτης στον πίνακα με τους καθηγητές που είχε δημιουργηθεί δυναμικά
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
	int visit_teacher(int Teacher) { //Αν ο γονεάς επιθυμεί να ενημερωθεί από τον γονέα, επιστρέφει 1, αλλιώς επιστρέφει 0
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
		int i = rand()%4; //Παίρνει τιμές τυχαία από 0 έως 3 - 25% πιθανότητα να μπερδευτεί ο γονέας
	    if (i==0)
	        return 1; //Επισρέφει 1 στην περίπτωση που ο γονέας έχει μπερδευτεί
		else return 0;
	}
};

class List {
	int size; //μέγεθος λίστας αναμονής
	Parent* next;
public:
	List(int s=0, Parent* ptr=NULL) :size(s), next(ptr) {cout<<"\nAn empty queue has been constructed \n";} //η constructor της κλάσης List
	int size_get() {return size;}
	~List() {cout<<"Queue destroyed\n"<<endl;} //η deconstructor της κλάσης List
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
