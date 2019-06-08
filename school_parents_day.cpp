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
			temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
		temp->set_next(ptr);
		ptr->set_prev(temp);
	}
	ptr->set_next(NULL);
	size++;
}

void List::parent_leaves_queue(Parent* ptr) {
	Parent* temp;
	if (next == ptr) { //Αν ο κόμβος βρίσκεται στην αρχή της λίστας
		if(ptr->next_get()==NULL) //Μοναδικός κόμβος στη λίστα
			next = NULL;
		else {
			next = ptr->next_get();
			temp = ptr->next_get();
			temp->set_prev(next);
		}
	}
	else if (ptr->next_get()==NULL) { //Αν ο κόμβος βρίσκεται στο τέλος της λίστας
		temp = ptr->prev_get();
	    temp->set_next(NULL);
	}
	else { //η συνάρτηση καλείται όταν η λίστα έχει εναν τουλάχιστον κόμβο
		temp = ptr->prev_get(); //περίπτωση που ο κόμβος δεν είναι ούτε στην αρχή ούτε στο τέλος της λίστας
		temp->set_next(ptr->next_get());
		temp = ptr->next_get();
		temp->set_prev(ptr->prev_get());
	}
}

void List::wants_teacher(int i, Parent** teachers) { //Αναζητά στη λίστα τον πρώτο γονέα που θέλει να επισκεφτή τον καθηγητή i
	Parent* temp = next;
	for(int j=0; j<size; j++) { //Αν δεν βρεθεί κανείς, τότε ο teacher[i] εξακολουθεί να δείχνει σε NULL
		if (temp->visit_teacher(i) == 0) //ο συγκεκριμένος γονέας δε θέλει να επισκεφτεί τον teacher i
		    temp = temp->next_get();
		else {
			teachers[i] = temp; //o teacher[i] δείχνει στον γονέα που ενημερώνει
			temp->set_teachers(i); //Μηδενισμός του temp->teachers[i]
		   	parent_leaves_queue(temp); //Ο γονέας φεύγει από την ουρά
			size--;
			break;
		}
	}
}

void List::print(int N) { //Εκτυπώνει του υπάρχοντες κόμβους-γονείς της λίστας
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
		List::insert_one_end(ptr[whose_turn]); //whose_turn=0 στην αρχή
		whose_turn++; //αντιστοιχεί στον επόμενο γονέα που θα μπει στην ουρά
	} 
}

void List::return_chosen_parents(Parent** teachers, int N, int &people_finished) {
	int check, confused;
	cout<<'\n';
	for(int i=0; i<N; i++) {
		if (teachers[i]!=NULL) { //Αν ο καθηγητής δεν ενημερώνει κανένα γονεά
			check = teachers[i]->is_done(N);
	        if(check==1) { //Αν ο γονέας έχει τελειώσει με την ενημέρωση
				people_finished++;
				cout<<"Parent "<<teachers[i]->id_get()<<" has now finished and leaves the queue\n";
				teachers[i]=NULL; //ο δείκτης της main δείχνει ακόμα στον γονέα
			}
			else { //Αν ο γονέας δεν έχει τελειώσει με την ενημέρωση
				confused = teachers[i]->is_confused(); 
				if (confused && size!=0)  //Αν ο γονέας μπερδεύτηκε και υπάρχουν και άλλοι γονείς στην ουρά
					cout<<"Parent "<<teachers[i]->id_get()<<" was confused and went at the start of the queue. The other parents are complaining"<<endl;
				cout<<"Parent "<<teachers[i]->id_get()<<" goes back at the end of the queue\n";
				insert_one_end(teachers[i]); //Ο γονέας πηγαίνει στο τέλος της ουράς
				teachers[i]=NULL; //Ο καθηγητής δείχνει σε NULL
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
	   ptr[i] = new Parent(i+1,N); //Δημιουργία Κ αντικειμένων κλάσης Parent και αρχικοποίηση τους
	cout<<"The parents who will visit Teachers today are: \n";
	for (int i=0; i<K; i++) {
		ptr[i]->print(N);
	} 
	Parent** teachers = new Parent*[N]; //Δημιουργία πίνακα Ν δεικτών σε γονείς - αντιπροσωπεύουν τους καθηγητές - όταν ένας καθηγητής εξυπηρετεί γονέα, τότε ο δείκτης δείχνει σε αυτόν
	for (int i=0; i<N; i++) {teachers[i]=NULL;}
	List queue; //Δημιουργία της ουράς. Αρχικοποιείται το μέγεθος με 0 και το next με NULL
	srand(time(NULL)); 
	n=K;
	set_L(L,n);
	queue.insert_parents(L,ptr); //Αρχική εισαγωγή L πλήθους γονέων στην ουρά 
	queue.print(N); 
	while ( n || (people_finished!=K) ) { //Έως ότου δεν υπάρχουν άλλοι γονείς που θέλουν να μπουν στην ουρά και αυτοί που έχουν τελειώσει είναι ίσοι με Κ
		queue.choose_parents(teachers, N); //Επιλέγει γονείς που θα επισκεφτούν δασκάλους
		print_parents_with_teachers(teachers,N); //Εκτυπώνει τους γονείς που ενημερώνονται από δασκάλους
		queue.print(N);
		set_L(L,n);
		queue.insert_parents(L, ptr); //Εισαγωγή τυχαίου πλήθους γονέων στην ουρά
		if(L!=0) //Εκτυπώνει τη λίστα μόνο αν εισάγονται νέοι γονείς σε αυτή
			queue.print(N);
		queue.return_chosen_parents(teachers,N,people_finished); //Επιστρέφει όλους τους γονείς που ενημερώθηκαν στην αρχή ή στο τέλος της ουράς ή τους αφαιρεί εντελώς από την ουρά
	} 
	delete[] teachers;	//Αποδέσμευση μνήμης από το heap
	for(int i=0; i<K; i++)
		delete ptr[i];
	delete[] ptr;
	cout<<"\nEnd Of School Parents' Day! "<<endl;
return 0;
}

void set_L(int& L, int& n) {
	L = rand()% (n+1); //Τυχαίος αριθμός γονέων από 0 έως n που εισάγονται στην ουρά
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
