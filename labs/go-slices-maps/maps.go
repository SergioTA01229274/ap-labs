//Sergio Ivan Tostado Nieto
//Student ID: A01229274

package main

import (
	"fmt"
	"strings"
	"os"
)

func WordCount(word string) map[string]int {
	res := make(map[string]int)
	word_arr := strings.Split(word, " ")
	for i := 0; i < len(word_arr); i++{
		_, ok := res[word_arr[i]]
		if(!ok){
			res[word_arr[i]] = 1
		}else{
			res[word_arr[i]] += 1;
		}
	}
	return res
}



func main() {
	arg := os.Args[1]
	for j := 2; j < len(os.Args); j++{
		arg += " "
		arg += os.Args[j]
	}
	fmt.Printf("%v\n",WordCount(arg))
}
