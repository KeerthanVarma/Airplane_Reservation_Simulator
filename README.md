# Airplane_Reservation_Simulator


Welcome to the **Domestic Airline Reservation Simulator**, a command-line based flight booking system designed using **C++**, **Object-Oriented Programming**, and **efficient Data Structures**. This project aims to simulate the core functionalities of a domestic airline's reservation and management system with real-time validation, seat visualization, booking persistence, admin control, and more.

---

##  Project Highlights

- **Built from Scratch in Advanced C++**
- **OOP Design** for modularity and scalability
- **Efficient Use of DSA**: maps, vectors, queues, priority queues, unordered maps, etc.
- **Seat Booking with Live Visualization**
- **Aadhaar-based Login System for Passengers**
- **Admin Panel for Monitoring and Control**
- **Real-time Fare Calculation & Refund Engine**
- **Persistent Data Storage using File Handling**
- **Voice Feedback Integration** using simple TTS
- **Boarding Pass Generation**
- **Booking History with Time Stamps**

---

## 🛠 Features in Detail

###  Passenger Side

- **Login via Aadhaar number (with validation)**
- Search flights by **source, destination, and date**
- View available flights with **live seat map (40x6 layout)**
- **Book tickets** with:
  - Name, Age, Gender, Seat Selection
  - Seat confirmation with input correction support
- **Cancel tickets** with dynamic refund calculation:
  - Full refund before 7 days
  - 50% within a week
  - No refund on the day
- **Boarding Pass Generation**
  - Displays passenger name, flight ID, seat, and flight details
- View **booking history** and **past flight info**

---

### 🛫 Admin Panel

- Login with **username/password**
- View all **flights with statistics**
- Check **occupancy and revenue**
- View **all bookings with seat info**
- View **waitlist and boarding queue**
- Data updated **live with every booking or cancellation**

---

## ⚙️ Data Structures & Optimization

This project doesn’t just simulate logic—it’s built with **real performance** in mind. We used:

- `std::map`, `std::unordered_map` — for fast access to flights, bookings, and passenger info
- `std::vector` — for dynamic storage of seats, passengers
- `std::queue`, `std::priority_queue` — for waitlisting and age-priority boarding queues
- `std::set` — to prevent duplicate bookings (Aadhaar-based)
- `std::chrono` and `std::tm` — for real-time booking and refund logic

All operations are optimized to minimize runtime, reduce lookup times, and simulate real-world airline data processing.

---

##  File Structure
/Airline-Reservation-Simulator
├── /include
│ ├── BookingSystem.h
│ ├── Flight.h
│ ├── Passenger.h
│ ├── Admin.h
│ ├── Pricing.h
│ ├── BoardingPass.h
│ ├── LoginManager.h
├── /src
│ ├── BookingSystem.cpp
│ ├── Flight.cpp
│ ├── Passenger.cpp
│ ├── Admin.cpp
│ ├── Pricing.cpp
│ ├── BoardingPass.cpp
│ ├── LoginManager.cpp
├── main.cpp
├── flights.txt
├── bookings.txt
├── README.md

---

##  Data Persistence

- Bookings are stored in `bookings.txt`
- Flight data stored in `flights.txt`
- All data is **retained** between sessions
- No crashes on invalid input — full **input validation** throughout

---

##  Security & Input Handling

- Every input is validated (age, seat, gender, Aadhaar)
- If invalid data is entered, users are **reprompted only for that field**
- Aadhaar input is enforced to be 12-digit only
- **Case-insensitive** city matching for source/destination

---

##  Why This Project is Different

> Many airline simulators exist — but **few are built from scratch** using only core **C++ with DSA**, no external libraries, and a **fully modular OOP structure**.

We aimed to build a system that:
- Is **educationally rich**
- Is **functionally complete**
- Emphasizes **time and space efficiency**
- Supports **scaling and real-world simulation**

You’ll find this project useful whether you’re a student, interviewer, or systems designer.

---

##  Contributing

This project is built solo as a passion project, but contributions and suggestions are welcome. Fork it, build on top of it, or just explore the logic.

---

##  Final Words

This project was built with real effort, debugging, and iterative improvements. It's more than just code — it's a representation of applied OOP, DSA, and software engineering fundamentals. Hope you enjoy it!

---

> **Author**: Keerthan Varma  
> **Institution**: IIT Gandhinagar  
> **Language**: C++17  
> **Platform**: Windows (MinGW)
