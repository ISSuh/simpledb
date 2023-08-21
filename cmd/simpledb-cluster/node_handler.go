/*
MIT License

Copyright (c) 2023 ISSuh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

package main

import (
	"encoding/json"
	"net/http"
	"strconv"

	"github.com/ISSuh/simpledb/internal/api"
	"github.com/ISSuh/simpledb/internal/node"
	"github.com/sirupsen/logrus"
)

const (
	HttpSheme = "http://"
)

func (cluster *Cluster) NewNode(w http.ResponseWriter, r *http.Request, args map[string]string) {
	path := args["id"]
	logrus.Infoln("Cluster.NewNode - id: ", path)

	if _, err := strconv.Atoi(path); err != nil {
		logrus.Errorln("Cluster.NewNode - ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}

	// parse body
	var node node.NodeMetadata
	decoder := json.NewDecoder(r.Body)
	if err := decoder.Decode(&node); err != nil {
		logrus.Errorln("Cluster.NewNode - ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	} else {
		logrus.Infoln("Cluster.NewNode - node : ", node)
	}

	// // notify new node to other nodes
	// for _, peer := range cluster.nodeManager.NodeList() {
	// 	id := peer.Id
	// 	address := peer.Address
	// 	requestNotifyPeerUrl := HttpSheme + address + "/node/peer/" + strconv.Itoa(id)
	// 	repuest, err := http.NewRequest(http.MethodPost, requestNotifyPeerUrl, nil)
	// 	if err != nil {
	// 		logrus.Errorln("Cluster.NewNode - ", err)
	// 		continue
	// 	}

	// 	httpClient := &http.Client{}
	// 	resp, err := httpClient.Do(repuest)
	// 	if (err != nil) || (resp.StatusCode != http.StatusOK) {
	// 		logrus.Errorln("Cluster.NewNode - err : ", err, " / response : ", resp)
	// 		continue
	// 	}
	// }

	// get other node list
	nodeMetas := cluster.nodeManager.NodeList()

	// regist new node
	if err := cluster.nodeManager.AddNode(node); err != nil {
		logrus.Errorln("Cluster.NewNode - ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
	}

	// struct json body
	jsonBody, err := json.Marshal(nodeMetas)
	if err != nil {
		logrus.Errorln("Cluster.NewNode - ", err)
		api.HandleResponse(w, nil, http.StatusInternalServerError)
	}

	api.HandleResponse(w, jsonBody, http.StatusOK)
}

func (cluster *Cluster) RemoveNode(w http.ResponseWriter, r *http.Request, args map[string]string) {
	path := args["id"]
	logrus.Infoln("Cluster.RemoveNode - id: ", path)

	var id int
	var err error
	if id, err = strconv.Atoi(path); err != nil {
		logrus.Errorln("Cluster.Node - ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}

	cluster.nodeManager.RemoveNode(id)
	api.HandleResponse(w, nil, http.StatusOK)
}

func (cluster *Cluster) Node(w http.ResponseWriter, r *http.Request, args map[string]string) {
	path := args["id"]
	logrus.Infoln("Cluster.Node - id: ", path)

	var id int
	var err error
	if id, err = strconv.Atoi(path); err != nil {
		logrus.Errorln("Cluster.Node - ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}

	node := cluster.nodeManager.Node(id)
	if node == nil {
		logrus.Errorln("Cluster.Node - invalid node id. ", id)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}

	body, err := json.Marshal(node)
	if err != nil {
		logrus.Errorln("Cluster.Node - ", err)
		api.HandleResponse(w, nil, http.StatusInternalServerError)
		return
	}

	api.HandleResponse(w, body, http.StatusOK)
}

func (cluster *Cluster) NodeList(w http.ResponseWriter, r *http.Request, args map[string]string) {
	logrus.Infoln("Cluster.NodeList")

	list := cluster.nodeManager.NodeList()
	if len(list) <= 0 {
		api.HandleResponse(w, nil, http.StatusOK)
		return
	}

	body, err := json.Marshal(list)
	if err != nil {
		logrus.Errorln("Cluster.Node - ", err)
		api.HandleResponse(w, nil, http.StatusInternalServerError)
		return
	}
	api.HandleResponse(w, body, http.StatusOK)
}
