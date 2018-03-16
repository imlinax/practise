package main

import (
	"fmt"
	"github.com/garyburd/redigo/redis"
	"strconv"
	"strings"
)

/*
计算哪些软件分类下的软件全部被隐藏掉了
输入：
	1. softMap[cate1] = []int{soft1, soft2}
	   softMap[cate2] = []int{soft2, soft3}
	   softMap[cate3] = []int{soft1, soft4}
	2. blockedSofts = []int {soft2, soft3,soft4}
输出：
	blockedCates = []int{cate2}

注意：
	1. 同一款软件可能属于多个分类， 比如soft1同时属于cate1和cate3， soft2同时属于cate1和cate2
	2. 总软件数量约为2w款，总分类数量约为200,各分类下软件数量不固定，但是基本上都小于2000
	3. 期望运行时间低于50ms,最终实现算法是php，性能上有将近十倍的差距，所以期望go可以在5ms内完成。
*/
func getBlockedCates(softMap map[int64][]int64, blockedSofts []int64) (blockedCates []int64) {
	blockMap := make(map[int64]bool, len(blockedSofts))
	for _, v := range blockedSofts {
		blockMap[v] = true
	}

	for cate, softs := range softMap {
		for i := 0; i < len(softs); i++ {
			if _, ok := blockMap[softs[i]]; ok == true {
				softs = append(softs[:i], softs[i+1:]...)
				i--
			}
		}
		softMap[cate] = softs
	}

	for cate, softs := range softMap {
		if len(softs) == 0 {
			blockedCates = append(blockedCates, cate)
		}
	}
	return
}

func initSoftMap(redisConnStr string) (softMap map[int64][]int64) {
	softMap = make(map[int64][]int64, 0)
	r, err := redis.Dial("tcp", redisConnStr)
	if err != nil {
		fmt.Println(err)
		return
	}
	keys, err := redis.Strings(r.Do("KEYS", "cloud_cate_children:*"))
	if err != nil {
		fmt.Println(err)
		return
	}

	for _, key := range keys {
		keyCate := strings.Split(key, ":")
		cateID, err := strconv.ParseInt(keyCate[1], 10, 32)
		if err != nil {
			continue
		}

		softs, err := redis.Ints(r.Do("LRANGE", key, 0, -1))
		if err != nil {
			fmt.Println(err)
			return
		}
		softMap[cateID] = []int64{}
		for _, soft := range softs {
			softMap[cateID] = append(softMap[cateID], int64(soft))
		}
	}
	return
}
func main() {
	softMap := initSoftMap("172.24.2.217:6378")
	blockedSofts := []int64{103356856, 100114658, 10257}
	blockedCates := getBlockedCates(softMap, blockedSofts)
	fmt.Println(blockedCates)
}
