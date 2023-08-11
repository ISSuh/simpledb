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

	"github.com/ISSuh/simpledb/internal/api"
	"github.com/ISSuh/simpledb/internal/node"
	"github.com/ISSuh/simpledb/internal/option"
)

type ClusterOption struct {
	Address           string                   `yaml:"address"`
	NodeManagerOption option.NodeManagerOption `yaml:"node_manager"`
}

func NewClusterOption() *ClusterOption {
	return &ClusterOption{Address: "", NodeManagerOption: option.NewNodeManagerOption()}
}

type Cluster struct {
	option ClusterOption

	route  []api.Route
	server *api.Server

	nodeManager *node.NodeManager
}

func NewCluster(option *ClusterOption) *Cluster {
	cluster := &Cluster{
		option:      *option,
		route:       nil,
		server:      nil,
		nodeManager: node.NewNodeManager(option.NodeManagerOption),
	}

	cluster.install()
	cluster.server = api.NewServer(cluster)
	return cluster
}

func (cluster *Cluster) Serve() error {
	return cluster.server.Serve(cluster.option.Address)
}

func (router *Cluster) Route() []api.Route {
	return router.route
}

func (cluster *Cluster) install() {
	cluster.route = []api.Route{
		// node managing handler
		api.NewRoute(http.MethodGet, "/node/{id}", cluster.Node),
		api.NewRoute(http.MethodPut, "/node/{id}", cluster.NewNode),
		api.NewRoute(http.MethodDelete, "/node/{id}", cluster.RemoveNode),
		api.NewRoute(http.MethodGet, "/node", cluster.NodeList),

		// storage handler
		api.NewRoute(http.MethodGet, "/storage/{key}", cluster.GetItem),
		api.NewRoute(http.MethodPut, "/storage/{key}", cluster.PutItem),
		api.NewRoute(http.MethodDelete, "/storage/{key}", cluster.RemoveItem),
	}
}
