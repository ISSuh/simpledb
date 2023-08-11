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
	"github.com/ISSuh/simpledb/internal/option"
)

type SimpleDbOption struct {
	nodeOption    option.NodeOption    `yaml:"address"`
	storageOption option.StorageOption `yaml:"address"`
}

func NewSimpleDbOption() *SimpleDbOption {
	return &SimpleDbOption{
		nodeOption:    option.NodeOption{},
		storageOption: option.NewStorageOption(""),
	}
}

type SimpleDBNode struct {
	option SimpleDbOption

	route  []api.Route
	server *api.Server
}

func NewSimpleDBNode(option *SimpleDbOption) *SimpleDBNode {
	node := &SimpleDBNode{
		option: *option,
		route:  nil,
		server: nil,
	}

	node.install()
	node.server = api.NewServer(node)
	return node
}

func (node *SimpleDBNode) Serve() error {
	return node.server.Serve(node.option.nodeOption.Address)
}

func (node *SimpleDBNode) Route() []api.Route {
	return node.route
}

func (node *SimpleDBNode) install() {
	node.route = []api.Route{
		// node managing handler
		api.NewRoute(http.MethodGet, "/heartbeat", node.Heartbeat),

		// storage handler
		api.NewRoute(http.MethodGet, "/storage/{key}", node.GetItem),
		api.NewRoute(http.MethodPut, "/storage/{key}", node.PutItem),
		api.NewRoute(http.MethodDelete, "/storage/{key}", node.RemoveItem),
	}
}
