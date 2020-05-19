// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"sync"
	"strings"
	"strconv"
	"gopl.io/ch5/links"
)

var (
	tokens = make(chan struct{}, 20)
	wg sync.WaitGroup
)

func crawl(url string) []string {
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token
	if err != nil {
		log.Print(err)
	}
	defer wg.Done()
	return list
}

//!+
func main() {
	Args := os.Args[1:]
	lim, errC := strconv.Atoi(strings.Split(Args[0], "=")[1])
	if errC != nil{
		log.Print(errC)
		os.Exit(1)
	}
	if len(Args) < 2{
		fmt.Println("Usage: go run crawl3.go -depth=[number] [link to crawl from]")
		os.Exit(1)
	}
	if lim < 1{
		fmt.Println("Error: depth argument must be greater than 1")
		os.Exit(1)
	}
	if lim > 3{
		fmt.Println("Warning: This version of a web crawler is designed to manage a depth delimiter. If depth is greater than 3 the excution time will be too long.")
		os.Exit(1)
	}

	worklist := make(chan linksOfLevel)

	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++
	go func() { worklist <- linksOfLevel{Args[1:], 0} }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, url := range list.links_to_crawl {
			if !seen[url] {
				seen[url] = true
				fmt.Println(url)
				if list.depthWanted < lim{
					n++
					wg.Add(1)
					go func(webUrl string) {
						linksToCrawlNext := crawl(webUrl)
						worklist <- linksOfLevel{linksToCrawlNext, list.depthWanted + 1}
					}(url)
				}
			}
		}
	}
	wg.Wait()
}

type linksOfLevel struct{
        links_to_crawl []string
        depthWanted int
}

