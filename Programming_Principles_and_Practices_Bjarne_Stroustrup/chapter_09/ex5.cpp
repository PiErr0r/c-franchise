#include "../std_lib_facilities.h"

int cardCnt = 0;

bool isValidISBN(string ISBN) {
	vector<string> parts (4, "");

	int cnt = 0;

	for (char i : ISBN) {
		if (i == '-') {
			++cnt;
			continue;
		}
		parts[cnt] += i;
	}

	if (parts.size() != 4) {
		return false;
	}
	if (parts[cnt].size() != 1) {
		return false;
	}

	for (int i = 0; i < parts.size() - 1; ++i) {
		for (int j = 0; j < parts[i].size(); ++j) {
			char lowerBound = j == 0 ? '1' : '0';
			char tmp = parts[i][j];
			if (tmp < lowerBound || tmp > '9') {
				return false;
			}
		}
	}


	return true;
};

enum class Genre {
	fiction = 1, nonfiction, periodical, biography, children
};

class Book {
private:
	string ISBN;
	string title;
	string author;
	bool available;
	int genre;
public:
	string getISBN() const { return ISBN; };
	string getTitle() const { return title; };
	string getAuthor() const { return author; };
	int getGenre() const { return genre; };
	void checkOut();
	void checkIn();
	bool isAvailable() { return available; };
	Book(string ISBN, string title, string author, Genre genre);
};

Book::Book(string nISBN, string nTitle, string nAuthor, Genre nGenre) 
	: ISBN {nISBN}, title {nTitle}, author {nAuthor}, genre {int(nGenre)}, available {true}
{
	if (!isValidISBN(nISBN)) {
		error("Invalid ISBN");
	}
}

void Book::checkIn() {
	available = false;
}

void Book::checkOut() {
	available = true;
}

bool operator==(const Book& a, const Book& b) {
	return a.getISBN() == b.getISBN();
}

bool operator!=(const Book& a, const Book& b) {
	return !(a==b);
}

ostream& operator<<(ostream& os, const Book& a) {
	return os << a.getISBN() << '\n' 
						<< a.getTitle() << '\n'
						<< a.getAuthor() << '\n'
						<< a.getGenre() << '\n';	
};

class Patron {
private:
	string name;
	int cardNumber;
	double fee;
public:
	string getName() const { return name; };
	int getCardNumber() const { return cardNumber; };
	double getFee() const { return fee; };
	void setFee(double fee);
	Patron(string name);
};

Patron::Patron(string nName)
	: name {nName}, cardNumber {cardCnt += 2}, fee {0}
{};

void Patron::setFee(double nFee) {
	fee += nFee;
};

bool owesFee(Patron& a) {
	return a.getFee() > 0;
};

ostream& operator<<(ostream& os, const Patron a) {
	return os << a.getName() << '\n'
						<< a.getCardNumber() << '\n'
						<< a.getFee() << '\n';
};

class Library {
private:
	vector<Book> books;
	vector<Patron> patrons;
	struct Transaction {
		Book book;
		Patron patron;
	};
	vector<Transaction> transactions;
public:
	void addBook(Book& book);
	void addPatron(Patron& patron);
	void checkOut(Patron& user, Book book); 
	vector<Patron> getOwers();
	Library();
};

vector<Patron> Library::getOwers() {
	vector<Patron> ret;
	for (Patron p : patrons) {
		if (owesFee(p)) {
			ret.push_back(p);
		}
	}
	return ret;
}

Library::Library(){};

void Library::addBook(Book& book) {
	books.push_back(book);
}

void Library::addPatron(Patron& patron) {
	patrons.push_back(patron);
}

void Library::checkOut(Patron& patron, Book book) {
	bool hasBook {false};
	for (Book& b : books) {
		if (b == book) {
			b.checkOut();
			hasBook = true;
			break;
		}
	}
	if (!hasBook) {
		error("The book is not in the Library");
	}

	bool hasUser {false};
	for (Patron& p : patrons) {
		if (p.getName() == patron.getName()) {
			hasUser = true;
			p.setFee(420.69);
			break;
		}
	}
	if (!hasUser) {
		error("The patron is not registered in the Library");
	}

	if (owesFee(patron)) {
		error("The patron owes fee");
	}

	Transaction tr {book, patron};
	transactions.push_back(tr);
}

int main() {
	Book test {"123-321-115-a", "Tristessa", "Jack Kerouac", Genre::nonfiction};
	Patron user {"BRene"};
	Patron pjer {"Pjer"};
	pjer.setFee(1.123);


	Library lib;
	lib.addPatron(user);
	lib.addPatron(pjer);
	lib.addBook(test);
	lib.checkOut(user, test);
	// cout << user;

	for (Patron p: lib.getOwers()) {
		cout << p;
	}
	return 0;
}