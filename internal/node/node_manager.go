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

package node

import (
	"errors"
	"log"
	"strconv"
	"sync"
)

type NodeManager struct {
	nodeIdCounter int
	nodes         map[int]NodeInfo

	mutex sync.Mutex
}

func NewNodeManager() *NodeManager {
	return &NodeManager{
		nodeIdCounter: 0,
		nodes:         make(map[int]NodeInfo),
	}
}

func (manager *NodeManager) AddNode(node NodeInfo) error {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	if _, ok := manager.nodes[node.Id]; ok {
		return errors.New("Already exist node. " + strconv.Itoa(node.Id))
	}

	manager.nodes[node.Id] = node
	return nil
}

func (manager *NodeManager) RemoveNode(id int) {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()
	delete(manager.nodes, id)
}

func (manager *NodeManager) Node(id int) *NodeInfo {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	if _, ok := manager.nodes[id]; !ok {
		log.Println("node - not exist node. ", id)
		return nil
	}

	node := manager.nodes[id]
	return &node
}

func (manager *NodeManager) NodeList() []NodeInfo {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	var list []NodeInfo
	for _, node := range manager.nodes {
		list = append(list, node)
	}
	return list
}
