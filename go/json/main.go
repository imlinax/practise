package main

import (
	"encoding/json"
	"fmt"
)

type obj struct {
	Name string
}
type export struct {
	Desc string
	Obj  *obj `json:"obj,omitempty"`
}

type nu struct {
	Count json.Number `json:"count"`
}

func main() {

	var ex export
	ex.Obj = new(obj)
	ex.Obj.Name = "ahah"
	data, err := json.Marshal(ex)
	if err != nil {
		panic(err)
	}

	fmt.Println(string(data))

	var nu nu
	js := `{"count": 12345}`
	err = json.Unmarshal([]byte(js), &nu)
	if err != nil {
		panic(err)
	}

	n, err := nu.Count.Int64()
	fmt.Println("js count is ", n)

	js1 := `{"count": "12345"}`
	err = json.Unmarshal([]byte(js1), &nu)
	if err != nil {
		panic(err)
	}

	n, err = nu.Count.Int64()
	fmt.Println("js1 count is ", n)

	str, err := json.Marshal(nu)
	if err != nil {
		panic(err)
	}
	fmt.Println("marshal result : ", string(str))

	type Str struct {
		S string `json:"s"`
	}

	var s Str
	js = `{"s":""}`
	err = json.Unmarshal([]byte(js), &s)
	if err != nil {
		panic(err)
	}
	fmt.Println("s.s: ", s.S)
}
