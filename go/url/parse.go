package main
import (
	"net/url"
	"fmt"
)
func main() {
	s := "http://360.cn/play/index.html?tag=1"
	u, err := url.Parse(s)
	if err != nil {
		panic(err)
	}
	fmt.Println(u.Host)
	fmt.Println(u.Path)
}
