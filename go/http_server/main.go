package main

import (
	"fmt"
	"net/http"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, req *http.Request) {
		fmt.Fprint(w, "hello world")
	})
	http.HandleFunc("/.*index", func(w http.ResponseWriter, req *http.Request) {
		fmt.Fprint(w, "hello index")
	})
	err := http.ListenAndServe("127.0.0.1:8081", nil)
	if err != nil {
		fmt.Println(err)
	}
}
