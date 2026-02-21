#include "Session.h"
#include "Book.h"
#include "User.h"

string Session::toString() const {
    return "ID: " + to_string(id) + ", User: " + username + ", Page: " + to_string(page+1);
}
