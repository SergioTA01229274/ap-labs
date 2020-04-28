package main

import (
        "io"
        "log"
        "net"
        "time"
        "os"
        "fmt"
	"errors"
)

func handleConn(c net.Conn, tz string) {
        defer c.Close()
        for {

                _, tErr := time.LoadLocation(tz)
                if tErr != nil {
			fmt.Println("Error when trying to load the timezone: " + tz)
                        log.Print(tErr)
			break
                }
                _, err := io.WriteString(c, tz + " " + time.Now().Format("15:04:05\n"))

		if err != nil {
                        log.Print(err)
			break
                }
                time.Sleep(1 * time.Second)
		errors.New("EOF") //doubt
        }
}

func main() {
        args := os.Args[1:]
        if len(args) < 2 {
                fmt.Println("Usage: go run clock2.go -port [port number to use]")
                os.Exit(1)
        }

        port := args[1]
        ListenInstruction := "localhost:" + port

        listener, err := net.Listen("tcp", ListenInstruction)
        if err != nil {
                log.Fatal(err)
        }

        timezone := os.Getenv("TZ")
        for {
                conn, err := listener.Accept()
                if err != nil {
                        log.Print(err) // e.g., connection aborted
                        continue
                }
                go handleConn(conn, timezone) // handle connections concurrently
        }
}

