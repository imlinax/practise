package main

import (
	"fmt"
	"github.com/garyburd/redigo/redis"
	"math/rand"
	"time"
)

var letterRunes = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")

func main() {
	redisConnStr := "10.74.52.61:6666"
	c, err := redis.Dial("tcp", redisConnStr)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer c.Close()
	for i := 0; i < 1000*1000*100; i++ {
		keyStr := RandStringRunes(60)
		v, err := c.Do("SET", keyStr, keyStr)
		if err != nil {
			fmt.Println(err)
			return
		}
		v, err = c.Do("DEL", keyStr)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(v, ": ", i)

	}
}

func init() {
	rand.Seed(time.Now().UnixNano())
}

func RandStringRunes(n int) string {
	b := make([]rune, n)
	for i := range b {
		b[i] = letterRunes[rand.Intn(len(letterRunes))]
	}
	return string(b)
}
