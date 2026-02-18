#include "Session.h"
#include "Book.h"
#include "User.h"

string Session::toString() const {
    return "ID: " + to_string(id) + ", Book: " + book->get_book_name() + ", User: " + user->get_username() + ", Page: " + to_string(page+1);
}
