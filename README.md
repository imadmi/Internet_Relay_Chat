# Internet Relay Chat

This project involves creating our own IRC server and testing it with a real IRC client (we choosed LimeChat). The Internet operates on solid standard protocols that enable connected computers to interact seamlessly. Developing an IRC server is a valuable exercise in understanding these protocols and real-time communication. 

## Table of Contents

I. [Introduction](#introduction)

II. [General Rules](#general-rules)

III. [Mandatory Part](#mandatory-part)

- [Requirements](#requirements)
- [For MacOS Only](#for-macos-only)
- [Test Example](#test-example)

IV. [Bonus Part](#bonus-part)

## Chapter I: Introduction

Internet Relay Chat (IRC) is a text-based communication protocol on the Internet, offering real-time messaging, both public and private. Users can exchange direct messages and join group channels. IRC clients connect to IRC servers to join channels, and these servers form a network.

## Chapter II: General Rules

- tHE program should not crash under any circumstances and must not quit unexpectedly.
- A Makefile is required, which should compile your source files without relinking.
- The Makefile should include rules such as `$(NAME)`, `all`, `clean`, `fclean`, and `re`.
- Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.
- The code must comply with the C++ 98 standard and still compile with the flag `-std=c++98`.
- Always prefer C++ features over C functions whenever possible.
- External libraries and Boost libraries are not allowed.

## Chapter III: Mandatory Part

### Requirements

- The server must handle multiple clients simultaneously without hanging.
- Forking is not allowed; all I/O operations must be non-blocking.
- Only one `poll()` (or equivalent) is allowed for handling all operations.
- A reference IRC client must be used during the evaluation, and it should connect to your server without errors.
- Communication between clients and the server should use TCP/IP (v4 or v6).
- The following features must be implemented:
  - Authentication, setting a nickname and username, joining a channel, sending and receiving private messages.
  - Messages sent to a channel must be forwarded to all other clients in the channel.
  - Support for operators and regular users.
  - Implementation of channel operator commands: KICK, INVITE, TOPIC, and MODE.

### For MacOS Only

MacOS specifics should be handled using `fcntl()`. File descriptors must be in non-blocking mode, with the following flag: `fcntl(fd, F_SETFL, O_NONBLOCK)`.

### Test Example

Testing should involve various error and issue scenarios, including partial data and low bandwidth. Use `nc` (netcat) to test your server, breaking the command into parts, and ensure it processes correctly.

## Chapter IV: Bonus Part

Extra features, such as handling file transfers or implementing a bot, can be added to make your IRC server more like an actual IRC server. The bonus part is evaluated only if the mandatory part is perfect.

## To run the server:
``` make && ./ircserv <port> <server_password>```

## To transfer data:
```make bonus && ./fileTransfer/bot <serverFd> <nickName> <Pathfile>```

## The client we used as reference is LimeChat
