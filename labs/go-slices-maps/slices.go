//Author: Sergio Ivan Tostado Nieto
//Student ID: A01229274


package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	slice := make([][]uint8, dy)
	for i := 0; i < dy; i++ {
		slice[i] = make([]uint8,dx)
	}
	var f uint8 = 5
	for j := 0; j < dy; j++{
		for k := 0; k < len(slice[j]); k++{
			slice[j][k] = f * 2
		}
	f += 4
	}
	return slice
}

func main() {
	pic.Show(Pic)
}
