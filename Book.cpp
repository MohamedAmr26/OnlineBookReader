#include <iostream>
#include <vector>

using namespace std;

#include "User.h"
#include "Book.h"

void Book::add_page(string page_context, const User &user)
{
    if (!User::is_authorized(user))
        return;
    pages.push_back(page_context);
}

void Book::remove_page(int pos, const User &user)
{
    if (!User::is_authorized(user))
        return;
    if (pos >= 0 && pos < pages.size())
        pages.erase(pages.begin() + pos);
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

void Book::change_book_name(string new_name, const User &user)
{
    if (!User::is_authorized(user))
        return;
    name = new_name;
}

void Book::change_book_author(string new_author, const User &user)
{
    if (!User::is_authorized(user))
        return;
    author = new_author;
}
