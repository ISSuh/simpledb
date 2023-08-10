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
	"log"
	"net/http"
	"strconv"

	"github.com/ISSuh/simpledb/internal/node"
	"github.com/gorilla/mux"
)

func (cluster *Cluster) NewNode(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	path := params["id"]
	log.Println("NewNode - id ", path)

	if _, err := strconv.Atoi(path); err != nil {
		response(w, "invalid path", http.StatusBadRequest)
		return
	}

	var node node.NodeMetadata
	decoder := json.NewDecoder(r.Body)
	if err := decoder.Decode(&node); err != nil {
		log.Println("NewNode - invalid message,. ", r.Body)
		response(w, "invalid message", http.StatusBadRequest)
		return
	}

	if err := cluster.nodeManager.AddNode(node); err != nil {
		response(w, "already exist", http.StatusBadRequest)
	}
	response(w, "", http.StatusOK)
}

func (cluster *Cluster) RemoveNode(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	path := params["id"]
	log.Println("RemoveNode - id : ", path)

	var id int
	var err error
	if id, err = strconv.Atoi(path); err != nil {
		response(w, "Bad Request. invalid path", http.StatusBadRequest)
		return
	}

	cluster.nodeManager.RemoveNode(id)
	response(w, "", http.StatusOK)
}

func (cluster *Cluster) Node(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	path := params["id"]
	log.Println("Node - id : ", path)

	var id int
	var err error
	if id, err = strconv.Atoi(path); err != nil {
		response(w, "invalid path", http.StatusBadRequest)
		return
	}

	node := cluster.nodeManager.Node(id)
	if node == nil {
		response(w, "invalid id", http.StatusBadRequest)
		return
	}

	body, err := json.Marshal(node)
	if err != nil {
		response(w, "data marshaling fail", http.StatusInternalServerError)
		return
	}

	response(w, string(body), http.StatusOK)
}

func (cluster *Cluster) NodeList(w http.ResponseWriter, r *http.Request) {
	log.Println("NodeList")

	list := cluster.nodeManager.NodeList()
	if len(list) <= 0 {
		response(w, "", http.StatusOK)
		return
	}

	body, err := json.Marshal(list)
	if err != nil {
		response(w, "data marshaling fail", http.StatusInternalServerError)
		return
	}
	response(w, string(body), http.StatusOK)
}
