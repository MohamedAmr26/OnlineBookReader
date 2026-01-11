        Session(const Book& b, int p, string d) 
            : book(b), page(p), date(d) {}

        Book get_book() const {
            return book;
        }

        int get_page() const {
            return page;
        }

        string get_date() const {
            return date;
        }

        void set_page(int p) {
            page = p;
        }

        void set_date(string d) {
            date = d;
        }
