package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Println(os.TempDir())
	if err := os.Remove("test.json"); err != nil {
		if os.IsExist(err) {
			fmt.Println("remove fail")
		} else {
			fmt.Println("file not exist")
		}
	}
}
