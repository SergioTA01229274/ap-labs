//Author: Sergio Ivan Tostado Nieto
//Student ID: A01229274
//Delivery: First partial challenge, Directory scannner in Golang

package main

import (
	"fmt"
	"os"
	"log"
	"io/ioutil"
)
var tmp_dirs[]string

func scanDir(dir string) map[string] int{

	files, err := ioutil.ReadDir(dir)
	categories := make(map[string]int)
	categories["Directories"] = 0
	categories["Symbolic Links"] = 0
        categories["Devices"] = 0
        categories["Sockets"] = 0
        categories["Reg Files"] = 0

	if err != nil {
		log.Fatal(err)
	}

	tmp_dir_flag := false
	for j := 0; j < len(files); j++ {
		switch mode := files[j].Mode(); {
			case mode&os.ModeDir != 0:
				tmp_wd, _ := os.Getwd()
				if !tmp_dir_flag {
					tmp_dir_flag = true
					tmp_dirs = make([]string, 1)
					tmp_dirs[0] = tmp_wd + "/" + files[j].Name()
					categories["Directories"] += 1
					continue
				}
				tmp_dirs = append(tmp_dirs, tmp_wd + "/" + files[j].Name())
				categories["Directories"] += 1
			case mode&os.ModeSymlink != 0:
				categories["Symbolic Links"] += 1
			case mode&os.ModeDevice != 0:
				categories["Devices"] += 1
			case mode&os.ModeSocket != 0:
				categories["Sockets"] += 1
			default:
				categories["Reg Files"] += 1
		}
	}

	for len(tmp_dirs) > 0 {
		dir_2_scan := tmp_dirs[0]
		count_files(dir_2_scan, categories)
		tmp_dirs = tmp_dirs[1:]
	}
	return categories
}

func count_files(tmp_dir string, categos map[string]int) {
	os.Chdir(tmp_dir)
	tmp_files, tmp_err := ioutil.ReadDir(tmp_dir)
	if tmp_err != nil {
                log.Fatal(tmp_err)
        }

	for j := 0; j < len(tmp_files); j++ {
                switch mode := tmp_files[j].Mode(); {
                       	case mode&os.ModeDir != 0:
                               	tmp_wd, _ := os.Getwd()
                               	tmp_dirs = append(tmp_dirs, tmp_wd + "/" + tmp_files[j].Name())
                               	categos["Directories"] += 1
                       	case mode&os.ModeSymlink != 0:
                               	categos["Symbolic Links"] += 1
                       	case mode&os.ModeDevice != 0:
                               	categos["Devices"] += 1
                       	case mode&os.ModeSocket != 0:
                               	categos["Sockets"] += 1
                       	default:
                               	categos["Reg Files"] += 1
               	}
       	}
}


func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}
	os.Chdir(os.Args[1])
	scan_res := scanDir(os.Args[1])
	fmt.Println("Directory scanner tool")
	fmt.Println("")
	fmt.Println("+-------------------------+--------------------+")
        fmt.Printf( "|Path                     |%s\n", os.Args[1])
        fmt.Println("+-------------------------+--------------------+")
        fmt.Printf( "|Directories              |         %v         |\n", scan_res["Directories"])
        fmt.Printf( "|Symbolic Links           |         %v         |\n", scan_res["Symbolic Links"])
        fmt.Printf( "|Devices                  |         %v         |\n", scan_res["Devices"])
        fmt.Printf( "|Sockets                  |         %v         |\n", scan_res["Sockets"])
        fmt.Printf( "|Other files              |         %v         |\n", scan_res["Reg Files"])
        fmt.Println("+-------------------------+--------------------+")

}
