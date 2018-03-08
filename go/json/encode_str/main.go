package main

import (
	"encoding/json"
	"fmt"
)

func main() {
	str := "hello"

	data, err := json.Marshal(str)
	if err != nil {
		panic(err)
	}

	fmt.Println("string marshal result:", string(data))

	i := 10
	data, err = json.Marshal(i)
	if err != nil {
		panic(err)
	}

	fmt.Println("number marshal result:", string(data))
}
