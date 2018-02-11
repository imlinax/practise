package main

import (
	"fmt"
	"regexp"
)

func extractHostNames(URL string) (hostnames []string) {
	var reg = regexp.MustCompile(`(http|https)://([^/]+)/`)
	all := reg.FindAllSubmatch([]byte(URL), -1)
	for i := 0; i < len(all); i++ {
		one := all[i]
		hostnames = append(hostnames, string(one[2]))
	}
	return
}
func main() {
	// Compile the expression once, usually at init time.
	// Use raw strings to avoid having to quote the backslashes.
	var reg= regexp.MustCompile(`(http|https)://([^/]+)/`)

	s := []byte(`pfdafaf http://www.com/fd/fdafaf;fads http://abc.com/fda; https://qq.com/fdafafd`)

	for _, one := range reg.FindAll(s, -1) {
		fmt.Println(string(one))
	}

	// 查找所有匹配结果及其分组字符串
	all := reg.FindAllSubmatch(s, -1)
	for i := 0; i < len(all); i++ {
		fmt.Println()
		one := all[i]
		for i := 0; i < len(one); i++ {
			fmt.Printf("%d: %s\n", i, one[i])
		}
	}


	fmt.Println("extract host names")
	hostnames := extractHostNames(string(s))
	for _, hostname := range hostnames {
		fmt.Println(hostname)
	}
}
