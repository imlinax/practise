package main

import (
	"fmt"
	mgo "gopkg.in/mgo.v2"
	"gopkg.in/mgo.v2/bson"
)

func main() {
	session, err := mgo.Dial("172.24.2.188:27017")
	if err != nil {
		fmt.Print(err)
		return
	}

	defer session.Close()
	var softID int64
	softID = 2
	collecttionSoftApp := session.DB("app_store").C("storage_soft")
	bulk := collecttionSoftApp.Bulk()
	bulk.Upsert(bson.M{"soft_id": softID},
		bson.M{"$set": bson.M{
			"soft_brief": "it's me!",
		},
		})
	_, err = bulk.Run()
	if err != nil {
		fmt.Print(err)
		return
	}

}
