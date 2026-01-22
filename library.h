#include <iostream>
#include <vector>
#include <algorithm>


#ifndef LIBRARY_H
#define LIBRARY_H

class Library{
    private:
    // change to set later
        vector<Book> Books;
    public:
        Library(){}
        bool insert_book(const Book& new_book){
            auto it = find(Books.begin(), Books.end(), new_book);
            if (it == Books.end()){
                Books.push_back(new_book);
                return true;
            }
            return false;
        }
        bool remove_book(int pos){
            if (pos >= 0 && pos < no_books()){
                Books.erase(Books.begin()+pos);
                return true;
            }
            return false;
        }
        const Book& get_book(int pos) const{
            if (pos >= 0 && pos < no_books())
                return Books[pos];
            throw out_of_range("Book index out of range");
        }
        const Book& get_book(string book_name) const{
            for (const auto& book : Books) {
                if (book.get_book_name() == book_name) {
                    return book;
                }
            }
            throw out_of_range("Book not found");
        }
        int no_books() const{
            return Books.size();
        }
        vector<string> list_books() const{
            vector<string> book_names;
            for (const auto& book : Books) {
                book_names.push_back(book.get_book_name());
            }
            return book_names;
        }
};
#endif