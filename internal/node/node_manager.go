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
	"strconv"
	"sync"
	"time"

	"github.com/ISSuh/simpledb/internal/option"
	"github.com/sirupsen/logrus"
)

type NodeManager struct {
	option        option.NodeManagerOption
	nodeIdCounter int
	nodeMetas     map[int]NodeMetadata
	nodeObservers map[int]*NodeObserver
	leaderId      int

	mutex sync.Mutex
}

func NewNodeManager(option option.NodeManagerOption) *NodeManager {
	return &NodeManager{
		option:        option,
		nodeIdCounter: 0,
		nodeMetas:     make(map[int]NodeMetadata),
		nodeObservers: make(map[int]*NodeObserver),
	}
}

func (manager *NodeManager) AddNode(nodeMeta NodeMetadata) error {
	logrus.Infoln("NodeManager.AddNode - id : ", nodeMeta.Id)
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	if _, ok := manager.nodeMetas[nodeMeta.Id]; ok {
		return errors.New("already exist node. " + strconv.Itoa(nodeMeta.Id))
	}

	manager.nodeMetas[nodeMeta.Id] = nodeMeta
	manager.nodeObservers[nodeMeta.Id] = NewNodeObserver(nodeMeta, manager)
	return nil
}

func (manager *NodeManager) RemoveNode(id int) {
	logrus.Infoln("NodeManager.RemoveNode - id : ", id)
	manager.mutex.Lock()
	defer manager.mutex.Unlock()
	delete(manager.nodeMetas, id)
	delete(manager.nodeObservers, id)
}

func (manager *NodeManager) Node(id int) *NodeMetadata {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	if _, ok := manager.nodeMetas[id]; !ok {
		logrus.Errorln("NodeManager.Node - not exist node. ", id)
		return nil
	}

	node := manager.nodeMetas[id]
	return &node
}

func (manager *NodeManager) NodeList() []NodeMetadata {
	manager.mutex.Lock()
	defer manager.mutex.Unlock()

	var list []NodeMetadata
	for _, node := range manager.nodeMetas {
		list = append(list, node)
	}
	return list
}

func (manager *NodeManager) HeartBeatDuration() time.Duration {
	return time.Duration(manager.option.HeartbeatDuration) * time.Second
}

func (manager *NodeManager) HeartBeatRequestTimeout() time.Duration {
	return time.Duration(manager.option.HeartbeatRequestTimout) * time.Millisecond
}

func (manager *NodeManager) Notify(message NodeHeartbeatMessage, status NodeStatus) {
	needRemove := false

	switch status {
	case RUNNING:
		if !message.Status {
			needRemove = true
		} else {
			manager.leaderId = message.Id
		}
	case DOWN:
		needRemove = true
	}

	if needRemove {
		manager.RemoveNode(message.Id)
		if manager.leaderId == message.Id {
			manager.leaderId = -1
		}
	}
}
