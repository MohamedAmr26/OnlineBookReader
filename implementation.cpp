        static map<string, pair<string, bool> > accounts;
        static map<string, vector<Session> > user_sessions;
        static void load_accounts(const map<string, pair<string, bool> >& accounts) {
            User::accounts = accounts;
        }            
        bool login(string in_username, string in_password){
            if (accounts.count(in_username) && accounts[in_username].first == in_password){
                username = in_username;
                password = in_password;
                isAdmin = accounts[in_username].second;
                sessions = user_sessions[username];
                return true;
            }
            return false;
        }
        bool is_admin() const{
            return isAdmin;
        }
        string get_username() const{
            return username;
        }
        static bool is_authorized(const User& user){
            return user.is_admin();
        }
        int sessionsSize() const{
            return sessions.size();
        }
        const vector<Session>& get_sessions() const;
        vector<const Session*> get_sessions_for(string book_name) const;

        void insert_session(const Session& session);

        Session& get_session(int id);
        void logout(){
            user_sessions[username] = sessions;
            sessions.clear();
            username = "";
            password = "";
            isAdmin = false;
        }
class UserService {
    private:
        static User* user;
    public:
        static void syncUser() {
            user = AuthenticationService::user;
        }
        static string get_username() {
            return user->get_username();
        }
        static int sessionsSize() {
            return user->sessionsSize();
        }
        static const vector<Session>& get_user_book_sessions() {
            return user->get_sessions();
        }
        static void insert_session(const Session& session) {
            user->insert_session(session);
        }
        static Session& get_session(int id) {
            return user->get_session(id);
        }
        static vector<pair<int, string>> list_sessions() {
            vector<pair<int, string>> session_pairs;
            for (const auto& session : user->get_sessions()) {
                session_pairs.push_back({session.get_id(), session.toString()});
            }
            return session_pairs;
        }
};
User* UserService::user = nullptr;
        static vector<string> get_sessions_for(string book_name) {
            vector<string> session_strings;
            for (const auto& session : UserService::get_user_book_sessions()) {
                if (session.get_book_name() == book_name) {
                    session_strings.push_back(session.toString());
                }
            }
            return session_strings;
        }
        static vector<int> get_session_ids_for(string book_name) {
            vector<int> session_ids;
            for (const auto& session : UserService::get_user_book_sessions()) {
                if (session.get_book_name() == book_name) {
                    session_ids.push_back(session.get_id());
                }
            }
            return session_ids;
        }
