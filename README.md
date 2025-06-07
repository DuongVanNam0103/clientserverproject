# Chat Application 

day la bai tap em lam mot ung dung chat client-server don gian duoc viet bang ngon ngu C, cho phep nguoi dung dang ki max client la 100 user dang ki, su dung phuong thuc TCP, dang nhap va gui tin nhan he thong su dung co so du lieu SQLite de luu tru thong tin nguoi dung va tin nhan. database se tu dong tao hai bang de luu thong tin nguoi dang ki va lich su tin nhan.
## Table of Contents

1. [Introduction]
2. [Installation]
3. [Usage]
4. [Project Structure]


## Introduction

ung dung gam 2 phan chinh client va server kem theo mot file database:
- **Client**: gui cac yeu cau dang ky, dang nhap, va gui tin nhan den server.
- **Server**: xu ly cac yeu cau tu client, quan ly nguoi dung va luu tru tin nhan vao co so du lieu SQLite.
- **database**: thuc hien nhiem vu tao bang de luu tru username va password, mot bang de luu tru tin nhan.

he thong nay su dung giao thuc TCP de giao tiep giua client va server.
2. [Installation]
## Installation

sudo apt-get install libsqlite3-dev

brew install sqlite

** System Requirements

- **gcc** or any C compiler.
- **Linux/MacOS/Windows** (with socket and SQLite support).
- **SQLite** (for the database).

### Steps to Install


##Compile the source code:
 make
#run server:  

./bin/server  

#run client:  

./bin/client  

#delete bin and build  

 make clean


3. [Usage](#usage)

 Register: dang ki mot user moi. Enter your username and password.

 Login: dang nhap user da dang ki.

 Send Message: gui tin nhan cho nguoi chi dinh.

4. [Project Structure]  

chat-app/  

│
├── bin/               
│   ├── client
│   └── server
│
├── build/             
│   ├── client.o
│   ├── database.o
│   └── server.o
│
├── data/             
│   └── chat.db
│
├── include/          
│   └── database.h
│
├── src/               
│   ├── client.c       
│   ├── database.c     
│   └── server.c      
│
├── Makefile           
└── README.md          

Makefile           
README.md          
bin/: luu file thuc thi (client, server).

build/: luu file bien dich (.o) compiled from source code in the src/ folder.

data/: luu file (chat.db) storing user information and messages.

include/: chua cac file thu vien dung chung (.h), including database.h which declares database handling functions.

src/:   chua cac file Files like client.c, server.c, and database.c 
Makefile: em su dung Makefile de bien dich chuong trinh.

README.md: file mo ta README.md.
