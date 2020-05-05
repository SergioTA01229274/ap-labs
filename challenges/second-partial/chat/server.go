// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.


//Author: Sergio Ivan Tostado Nieto
//Student ID: A01229274
//Second partial challenge
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
	"bytes"
)

type client chan<- string // an outgoing message channel, send-only channel

var (
	clientsArr []client //array of clients to identify and update channel ADMIN
	kickedClients = make(map[client]bool)
	entering = make(chan client) //clients that get into the channel chat
	leaving  = make(chan client) //clients that leave the channel chat
	messages = make(chan string) // all incoming client messages
	hostName = "irc-server"
	location, _ = time.LoadLocation(timezone)
	//loc = "America/Mexico_City"
	usernames = make(map[client]string) // map to store clients usernames
	connections = make(map[client]net.Conn) //map to store clients TCP connections
	timezone = ""
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			userSending := strings.Split(msg, " ")[0]
			for cli := range clients {
				key, _ := usernames[cli]
				if key == userSending{
					continue
				}
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true
			clientsArr = append(clientsArr, cli)

		case cli := <-leaving:
			delete(clients, cli)
			delete(usernames, cli)
			delete(connections, cli)
			delete(kickedClients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	ch <- hostName + " > Welcome to the Simple IRC Server"
	username := make([]byte, 1024)
	conn.Read(username)
	usernames[ch] = string(username)
	connections[ch] = conn
	kickedClients[ch] = false
	ch <- hostName + " > Your user [" + string(username) + "] is succesfully logged"


	fmt.Println(hostName + " > New connected user[" + string(username) + "]")
	messages <- hostName + " > New connected user [" + string(username) + "]"
	entering <- ch
	if clientsArr[0] == ch{
		ch <- hostName + " > You're the new IRC Server channel admin"
		fmt.Println(hostName + " > [" + string(username) + "] was promoted as the IRC Server channel ADMIN")
	}
	input := bufio.NewScanner(conn)
	inText := ""
	inputLoop:
	for input.Scan() && !kickedClients[ch]{
		if inputErr := input.Err(); inputErr != nil{
			log.Fatal(inputErr)
			ch <- hostName + " > There was an error sending your message"
			continue
		}
		inText = input.Text()
		if inText == ""{
			continue
		}
		if string(inText[0]) == "/"{
			commandWArgs := strings.Split(inText, " ")
			switch command := commandWArgs[0]; command{
				case "/users":
					if len(usernames) == 1{
						ch <- hostName + " > You are the only user in the channel"
						break
					}
					usersOutput := hostName + " > "
					for user := range usernames{
						userKey, _ := usernames[user]
						usersOutput += userKey + " "
					}
					ch <- usersOutput
					continue inputLoop
				case "/time":
					currentTime := time.Now().In(location).Format("15:04")
					ch <- hostName + " > Local Time: " + timezone + " " + currentTime
					continue inputLoop
				case "/user":
					if len(commandWArgs) < 2{
						ch <- hostName + " > Not enough arguments in command [/user]. Usage: /user [username]"
						continue inputLoop
					}
					userNotFound := true
					userToGet := make([]byte, 1024)
					copy(userToGet[:], commandWArgs[1])
					for user := range usernames{
						if bytes.Compare(userToGet, []byte(usernames[user])) == 0{
							ch <- hostName + " > username: " + string(usernames[user]) + ", IP: " + connections[user].RemoteAddr().String()
							userNotFound = false
							continue inputLoop
						}
					}
					if userNotFound{
						ch <- hostName + " > Impossible to get  user's info, [" + commandWArgs[1] + "] user not found"
					}
				case "/msg":
					if len(commandWArgs) < 3{
						ch <- hostName + " > Not enoguh arguments in command [/msg]. Usage: /msg [username] [message to send]"
						continue inputLoop
					}
					userNotFound := true
					userToSend := make([]byte, 1024)
					copy(userToSend[:], commandWArgs[1])
					for user := range usernames{
						if bytes.Compare(userToSend, []byte(usernames[user])) == 0{
							completeMsg := ""
							for i := 2; i < len(commandWArgs); i++{
								completeMsg += commandWArgs[i] + " "
							}
							user <- "[private] " + string(username) + " > " + completeMsg
							userNotFound = false
							break
						}
					}
					if userNotFound{
						ch <- hostName + " > Impossible to send message, [" + commandWArgs[1] + "] user not found"
						continue inputLoop
					}
					continue inputLoop
				case "/kick":
					if clientsArr[0] != ch{
						ch <- hostName + " > You are not the IRC Server channel ADMIN!!"
						continue inputLoop
					}
					if len(commandWArgs) < 2{
						ch <- hostName + " > Not enough arguments in command [/kick]. Usage: /kick [username]"
						continue inputLoop
					}
					userNotFound := true
					userToKick := make([]byte, 1024)
					copy(userToKick[:], commandWArgs[1])
					for user := range usernames{
						if bytes.Compare(userToKick, []byte(usernames[user])) == 0{
							kickedClients[user] = true
							user <- hostName + " > You're kicked from channel"
							fmt.Println(hostName + " > " + "[" + usernames[user] + "] was kicked from channel")
	              					messages <- hostName + " > " + "[" + usernames[user] + "] was kicked channel"
							userNotFound = false
							break
						}
					}
					if userNotFound{
						ch <- hostName + " > Impossible to kick user, [" + commandWArgs[1] + "] user not found"
					}
					continue inputLoop
				default:
					ch <- hostName + " > Unrecognized command: " + command
			}
		}
		messages <- usernames[ch] + " > " + input.Text()
	}

	if !kickedClients[ch]{
		leaving <- ch
		fmt.Println(hostName + " > " + "[" + usernames[ch] + "] left channel")
		messages <- hostName + " > " + "[" + usernames[ch] + "] left channel"
	}else{
		leaving <- ch
	}
	if clientsArr[0] == ch{
		clientsArr = removeClient(ch)
		if len(clientsArr) >= 1{
        		fmt.Println(hostName + " > [" + usernames[clientsArr[0]] + "] was promoted as the IRC Server channel ADMIN")
        		messages <- hostName + " > [" + usernames[clientsArr[0]] + "] was promoted as the IRC Server channel ADMIN"
		}
	}else{
		clientsArr = removeClient(ch)
	}
	conn.Close()
}

//!+Function to update the clients array and channel ADMIN
func removeClient(c chan string) []client{
	if len(clientsArr) == 1{
		return make([]client, 0)
	}
	i := 0
	for i = 0; i < len(clientsArr); i++{
		if clientsArr[i] == c{
			break
		}
	}
	if i == 0{
		return clientsArr[1:]
	}else{
		tmp := clientsArr[:i]
		for j := i + 1; j < len(clientsArr); j++{
			tmp = append(tmp, clientsArr[j])
		}
		return tmp
	}
}


func clientWriter(conn net.Conn, ch <-chan string) { //recieves a read-only channel
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}
//!-HandleConn

//!+main
func main() {
	args := os.Args[1:]
	//First we validate if all args were entered
	if len(args) < 4{
		fmt.Println("Usage: go run server.go -host [host ip address/localhost] -port [port number to use]")
		os.Exit(3)
	}

	instruction := args[1] + ":" + args[3]

	listener, err := net.Listen("tcp", instruction)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(hostName + " > Simple IRC Server started at " + instruction)
	fmt.Println(hostName + " > Ready for receiving new clients")

	value, ok := os.LookupEnv("TZ")
	if ok{
		timezone = value
		location, _ = time.LoadLocation(timezone)
	}else{
		timezone = "America/Mexico_City"
		location, _ = time.LoadLocation(timezone)
	}
	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
