# 📚 Online Book Reader

A console-based C++ application that allows users to read books, manage reading sessions, and for administrators to manage the library catalog.

---

## Features

**For Readers (Customers)**
- Browse the full book catalog
- Open books and navigate page by page
- Create and resume reading sessions per book
- View all personal reading sessions

**For Administrators**
- Add new books with custom page content
- Remove books from the library
- View the current book catalog

---

## Architecture

The project follows a **layered service architecture** with clear separation of concerns:

```
App (Entry Point)
├── UI Layer          → ConsoleUI / UI (abstract interface)
├── Auth Layer        → AuthenticationService
├── Data Layer        → DatastoreService
├── Domain Services   → LibraryService, SessionService
└── Models            → User (Admin/Customer), Book, Session
```

### Class Overview

| Class | Responsibility |
|---|---|
| `App` | Main controller — orchestrates login, menus, and navigation |
| `ConsoleUI` | Concrete implementation of the `UI` interface for terminal I/O |
| `UI` | Abstract interface for all user interaction (swappable) |
| `AuthenticationService` | Handles login and logout using `DatastoreService` |
| `DatastoreService` | Static in-memory store for user accounts |
| `LibraryService` | Manages the book collection and reading (page navigation) |
| `SessionService` | Manages reading sessions (create, resume, list, delete) |
| `User` | Abstract base class; extended by `Admin` and `Customer` |
| `Book` | Represents a book with name, author, and pages |
| `Session` | Tracks a user's reading position in a book |

---

## Project Structure

```
.
├── main.cpp            # App, ConsoleUI, DatastoreService, AuthenticationService,
│                       # LibraryService, SessionService
├── User.h              # User (abstract), Admin, Customer
├── Book.h              # Book class
├── Session.h           # Session class
└── UI.h                # UI abstract interface
```

---

## Getting Started

### Prerequisites
- A C++11 (or later) compatible compiler (e.g., `g++`, `clang++`)

### Build & Run

```bash
make && ./app
```

### Default Accounts

| Username | Password | Role |
|---|---|---|
| `mohamedAmr26` | `examplePassword1` | Customer |
| `ahmedSasa57` | `examplePassword2` | Customer |
| `seriousAdmin` | `adminPassword1` | Admin |

---

## Usage

### Customer Flow
1. Log in with a customer account
2. Select **Book List** to browse available books
3. Choose a book → select an existing session or create a new one
4. Navigate pages using the **Previous / Next / Save & Exit** controls
5. Return to the main menu to continue from where you left off via **My Reading Sessions**

### Admin Flow
1. Log in with an admin account
2. Select **Modify Library**
3. Choose to **Add**, **Remove**, or **View** books

---

## Design Notes

- **UI is abstracted** via the `UI` interface, making it straightforward to swap `ConsoleUI` for a GUI or web frontend without changing the core logic.
- **Services are stateless singletons** using static methods and static storage — suitable for a single-process, in-memory application.
- **Sessions persist within a run** — data is held in memory and resets on exit (no file/database persistence).
- **Admin removing a book** also cleans up all associated sessions via `SessionService::remove_sessions_for_book`.
