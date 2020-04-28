//Author: Sergio Ivan Tostado Nieto


package main

import (
        "io"
        "log"
        "net"
        "os"
        "fmt"
        "strings"
	"sync"
)


func requestTime(dst io.Writer, src io.Reader, c chan bool){
		if <- c{
			_, err := io.Copy(dst, src)
       			if err != nil {
          			log.Fatal(err)
       			}
		}
}


func main(){
        if len(os.Args) < 2{
                fmt.Println("Usage: go run clockWall.go [first connection] [second connection] ...")
                os.Exit(1)
        }
        Args := os.Args[1:]
	connections := make([]net.Conn, len(Args))
	channels := make([]chan bool, len(Args))
	for j := 0; j < len(connections); j++{
		conn, err := net.Dial("tcp", strings.Split(Args[j], "=")[1])
                if err != nil{
                        log.Fatal(err)
                }
                connections[j] = conn
		channels[j] = make(chan bool, 1)
	}

	var wg sync.WaitGroup
	wg.Add(len(connections))
	for i := 0; i < len(connections); i++{
		go requestTime(os.Stdout, connections[i], channels[i])
	}

	for k := 0; k < len(channels); k++{
		channels[k] <- true
	}

	wg.Wait()
	defer func(chans [] chan bool){
		for i := 0; i < len(channels); i++{
			close(channels[i])
		}
	}(channels)

	defer func(conns [] net.Conn){
		for i := 0; i < len(conns); i++{
			conns[i].Close()
		}
	}(connections)
}

