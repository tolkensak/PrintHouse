# <img src="icon.png" alt="PrintHouse" width="28"> PrintHouse

> **Print shop management system — orders, queues, and workshops.**

PrintHouse is a desktop application for managing printing shop operations. It handles print jobs, queues, orders, and workshop coordination.

**Tech Stack:** C++, MFC, MySQL  
**Built with:** Visual Studio 2003 (VS71), Visual Studio 2008 (VS90)  
**Developed:** 2008–2009

<br />

## Features

- **Order Management** — create, edit, and track print orders
- **Print Queue** — manage job queue and priorities
- **Workshop Coordination** — assign jobs to workshops
- **Search & Filters** — find orders by multiple criteria
- **User Authentication** — login system

<br />

## Screenshots

### Orders Line

![Screenshot: Orders Line](screenshots/orders-line.png "Orders Line")

### Login

![Screenshot: Login](screenshots/login.png "Login")

### Edit Order

![Screenshot: Edit Order](screenshots/edit-order.png "Edit Order")

### Search Filters

![Screenshot: Search Filters](screenshots/search-filters.png "Search Filters")

### Workshops

![Screenshot: Workshops](screenshots/workshops.png "Workshops")

<br />

## Project Structure
```
/
├── 71/          — Visual Studio 2003 project files
├── 90/          — Visual Studio 2008 project files
├── db/          — Database schema and scripts
├── src/         — Source code
└── screenshots/ — Application screenshots
```

<br />

## Requirements

- **Windows**
- **Microsoft Visual Studio** (2003 or 2008)
- **MySQL** (database server)
- **MFC** (included with Visual Studio)

<br />

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/tolkensak/PrintHouse.git
   ```
2. Create the MySQL database using files in db/
3. Open the project in Visual Studio (use 71/ or 90/ folder)
4. Build and run

<br />

## License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
