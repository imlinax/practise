package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"strconv"
)

type JsonFile struct {
	Data UnicodeString `json:"data"`
}

type UnicodeString struct {
	UString string
}

func (u *UnicodeString) UnmarshalJSON(b []byte) error {
	return json.Unmarshal(b, &u.UString)
	//u.UString = string(b)
	// return nil
}

func (u UnicodeString) MarshalJSON() ([]byte, error) {
	return []byte(strconv.QuoteToASCII(u.UString)), nil
}
func main() {
	data, err := ioutil.ReadFile("input.json")
	if err != nil {
		panic(err)
	}

	var jf JsonFile
	err = json.Unmarshal(data, &jf)
	if err != nil {
		panic(err)
	}
	fmt.Println("unmarshal success")

	data, err = json.Marshal(jf)
	if err != nil {
		panic(err)
	}

	fmt.Println("marshal success")
	err = ioutil.WriteFile("output.json", data, 0644)
	if err != nil {
		panic(err)
	}

	fmt.Println("write file success")

}
