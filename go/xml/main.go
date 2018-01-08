package main

import (
	"encoding/xml"
)
import "fmt"
import "log"

func main() {

	str := `<?xml version="1.0" encoding="UTF-8"?>
	<resources>
		<name>
			<![CDATA[360安全卫士]]>
		</name>

	</resources>`
	type StringResources struct {
		XMLName xml.Name `xml:"resources"`
		Name    string   `xml:"name"`
	}

	var result StringResources
	err := xml.Unmarshal([]byte(str), &result)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(result.Name)
}
