#include <iostream>
#include <vector>

using namespace std;

#include "User.h"
#include "Book.h"
#include "Session.h"

void Book::add_page(string page_context)
{
    pages.push_back(page_context);
}

void Book::remove_page(int pos)
{
    if (pos >= 0 && pos < pages.size())
        pages.erase(pages.begin() + pos);
}

bool Book::is_out_borders(int pos) const{
    return pos >= 0 && pos < pages.size();
}

string Book::get_page(int num) const
{
    if (num >= 0 && num < pages.size())
        return pages[num];
    return "";
}

int Book::get_no_pages() const
{
    return pages.size();
}

string Book::get_book_name() const
{
    return name;
}

string Book::get_book_author() const
{
    return author;
}

void Book::change_book_name(string new_name)
{
    name = new_name;
}

void Book::change_book_author(string new_author)
{
    author = new_author;
}

void Book::show_page(int p){
    string page = get_page(p);
    cout << page << endl;
    cout << "1-Previous Page" << "            " << "2-Next Page" << "            " << "3-Save&Exit\n";
    ask_controllers();
}
void Book::ask_controllers(){
    int n;
    cin >> n;
    switch (n){
        case 1:
            if (!is_out_borders(current_page+1))
                show_page(++current_page);
            break;
        case 2:
            if (!is_out_borders(current_page-1))
                show_page(--current_page);
            break;
        case 3:
            break;
        default:
            ask_controllers();
            break;
    }
}

void Book::open(Session& session){
    if (session.get_book_name() != name) {
        cout << "This session doesn't relate to this book\n";
        return;
    }
    current_page = session.get_page();
    show_page(current_page);
    
    // save and exit
    session.set_page(current_page);
    session.set_date("14 Jan 2026");
}