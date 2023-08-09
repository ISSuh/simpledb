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
	"net/http"

	"github.com/ISSuh/simpledb/internal/node"
	"github.com/gorilla/mux"
)

type Cluster struct {
	option ClusterOption
	router *mux.Router

	nodeManager *node.NodeManager
}

func NewCluster(option *ClusterOption) *Cluster {
	return &Cluster{
		option: *option,
		router: mux.NewRouter(),
		nodeManager: node.NewNodeManager(),
	}
}

func (cluster *Cluster) Serve() error {
	cluster.install()

	http.Handle("/", cluster.router)
	return http.ListenAndServe(cluster.option.address, nil)
}

func (cluster *Cluster) install() {
	cluster.router.HandleFunc("/node/{id}", cluster.Node).Methods("GET")
	cluster.router.HandleFunc("/node/{id}", cluster.NewNode).Methods("PUT")
	cluster.router.HandleFunc("/node/{id}", cluster.RemoveNode).Methods("DELETE")
	cluster.router.HandleFunc("/node", cluster.NodeList).Methods("GET")
}
