package main

import "testing"
import "fmt"

func BenchmarkGetBlockedCates(b *testing.B) {
	softMap := initSoftMap("172.24.2.217:6378")
	totalSofts := make(map[int64]bool)
	for _, softs := range softMap {
		for _, soft := range softs {
			totalSofts[soft] = true
		}
	}
	blockedSofts := make([]int64, 0)
	blockN := 10000
	for soft := range totalSofts {
		if blockN == 0 {
			break
		}
		blockedSofts = append(blockedSofts, soft)
		blockN--
	}

	fmt.Println("total len: ", len(totalSofts), " softs len: ", len(blockedSofts))
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		getBlockedCates(softMap, blockedSofts)
	}
}
