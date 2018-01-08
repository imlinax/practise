package main

import (
"fmt"
)

func main() {
	rint := 12955555
	str := fmt.Sprintf("\\u%04x", int64(rint))
	fmt.Println(str)
}


