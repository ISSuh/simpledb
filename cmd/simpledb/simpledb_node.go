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
	"bytes"
	"encoding/json"
	"errors"
	"io"
	"net/http"
	"strconv"
	"time"

	"github.com/ISSuh/simpledb/internal/api"
	"github.com/ISSuh/simpledb/internal/engine"
	"github.com/ISSuh/simpledb/internal/engine/lsm"
	"github.com/ISSuh/simpledb/internal/node"
	"github.com/ISSuh/simpledb/internal/option"
	"github.com/sirupsen/logrus"
)

const (
	HttpSheme = "http://"
)

type SimpleDbOption struct {
	Address        string               `yaml:"address"`
	StandAloneMode bool                 `yaml:"standalone"`
	NodeOption     option.NodeOption    `yaml:"node"`
	StorageOption  option.StorageOption `yaml:"storage"`
}

func NewSimpleDbOption() *SimpleDbOption {
	return &SimpleDbOption{
		Address:        "",
		StandAloneMode: false,
		NodeOption:     option.NodeOption{},
		StorageOption:  option.NewStorageOption(""),
	}
}

type SimpleDBNode struct {
	option SimpleDbOption

	route  []api.Route
	server *api.Server

	dbNode *node.Node
	engine engine.StorageEngine
}

func NewSimpleDBNode(option *SimpleDbOption) *SimpleDBNode {
	n := &SimpleDBNode{
		option: *option,
		route:  nil,
		server: nil,
		dbNode: nil,
		engine: lsm.NewLsmStorageStorage(),
	}

	if !option.StandAloneMode {
		n.dbNode = node.NewNode(option.NodeOption)
	}

	if err := n.engine.Open(n.option.StorageOption); err != nil {
		return nil
	}

	n.install()
	n.server = api.NewServer(n)
	return n
}

func (n *SimpleDBNode) Serve() error {
	if !n.option.StandAloneMode {
		go n.ConnectToCluster()
	}
	return n.server.Serve(n.option.Address)
}

func (n *SimpleDBNode) Stop() {
	if !n.option.StandAloneMode {
		go n.Stop()
	}

	n.engine.Close()
}

func (n *SimpleDBNode) Route() []api.Route {
	return n.route
}

func (n *SimpleDBNode) install() {
	n.route = []api.Route{
		// storage handler
		api.NewRoute(http.MethodGet, "/storage/{key}", n.GetItem),
		api.NewRoute(http.MethodPut, "/storage/{key}", n.PutItem),
		api.NewRoute(http.MethodDelete, "/storage/{key}", n.RemoveItem),
	}

	if !n.option.StandAloneMode {
		// node managing handler
		n.route = append(n.route, api.NewRoute(http.MethodGet, "/node/heartbeat", n.Heartbeat))
		n.route = append(n.route, api.NewRoute(http.MethodPost, "/node/peer/{id}", n.RegistNewPeer))
	}
}

func (n *SimpleDBNode) ConnectToCluster() {
	nodeId := n.option.NodeOption.NodeId
	nodeAddress := n.option.NodeOption.RpcAddress
	clusterAddress := n.option.NodeOption.ClusterAddress

	registNodeUrl := HttpSheme + clusterAddress + "/node/" + strconv.Itoa(nodeId)
	nodeMeta := node.NodeMetadata{Id: nodeId,	Address: nodeAddress}
	nodeMetaJson, _ := json.Marshal(nodeMeta)
	buffer := bytes.NewBuffer(nodeMetaJson)
	body, err := n.requestToCluster(http.MethodPost, registNodeUrl, buffer)
	if err != nil {
		logrus.Error(err)
		return
	}

	var peerMetas []node.NodeMetadata
	if err := json.Unmarshal(body, &peerMetas); err != nil {
		logrus.Error(err)
		return
	}

	n.dbNode.ConnectPeerNode(peerMetas)
}

func (n *SimpleDBNode) requestToCluster(method, url string, body io.Reader) ([]byte, error) {
	req, err := http.NewRequest(method, url, body)
	if err != nil {
		return nil, err
	}

	client := http.Client{Timeout: time.Duration(5 * time.Second)}
	res, err := client.Do(req)
	if err != nil {
		return nil, err
	}
	return n.handleResponse(res)
}

func (n *SimpleDBNode) handleResponse(resp *http.Response) ([]byte, error) {
	switch resp.StatusCode {
	case http.StatusOK:
		return n.extractBody(resp)
	case http.StatusInternalServerError:
		return nil, errors.New("http.StatusInternalServerError")
	case http.StatusBadRequest:
		return nil, errors.New("http.StatusBadRequest")
	default:
		return nil, errors.New("unsupport error")
	}
}

func (n *SimpleDBNode) extractBody(resp *http.Response) ([]byte, error) {
	defer resp.Body.Close()
	buffer, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}
	return buffer, nil
}
