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
	"net"
	"net/http"
	"time"

	"github.com/sirupsen/logrus"
)

const (
	HttpSheme    = "http://"
	HeartBeatUrl = "/node/heartbeat"
)

type NodeStatus int

const (
	RetryCountLimit = 1
)

const (
	RUNNING = iota
	DOWN
)

type NodeHeartbeatMessage struct {
	Timestamp int64 `json:"timestamp"`
	Id        int   `json:"id"`
	Status    bool  `json:"status"`
	LeaderId  int   `json:"leader_id"`
}

type NodeObserverNotifier interface {
	HeartBeatDuration() time.Duration
	HeartBeatRequestTimeout() time.Duration
	Notify(message NodeHeartbeatMessage, status NodeStatus)
}

type NodeObserver struct {
	nodeMetadata NodeMetadata
	notifier     NodeObserverNotifier
	httpClient   *http.Client
	running      chan bool

	retryCount int
}

func NewNodeObserver(nodeMeta NodeMetadata, notifier NodeObserverNotifier) *NodeObserver {
	observer := &NodeObserver{
		nodeMetadata: nodeMeta,
		notifier:     notifier,
		running:      make(chan bool),
		retryCount:   0,
	}

	transport := http.Transport{
		Dial: observer.dialTimeout,
	}

	observer.httpClient = &http.Client{
		Transport: &transport,
	}

	go observer.heartbeat()
	return observer
}

func (observer *NodeObserver) dialTimeout(network, addr string) (net.Conn, error) {
	return net.DialTimeout(network, addr, observer.notifier.HeartBeatRequestTimeout())
}

func (observer *NodeObserver) heartbeat() {
	logrus.Infoln("NodeObserver.heartbeat [", observer.nodeMetadata.Id, "] - start")
	url := HttpSheme + observer.nodeMetadata.Address + HeartBeatUrl
	repuest, err := http.NewRequest(http.MethodGet, url, nil)
	if err != nil {
		logrus.Errorln("NodeObserver.heartbeat [", observer.nodeMetadata.Id, "] - err : ", err)
		return
	}

	for {
		time.Sleep(observer.notifier.HeartBeatDuration())

		resp, err := observer.httpClient.Do(repuest)
		if (err != nil) || (resp.StatusCode != http.StatusOK) {
			logrus.Warningln("NodeObserver.heartbeat [", observer.nodeMetadata.Id, "] - err : ", err, " / response : ", resp)
			observer.retryCount++
		} else {
			observer.retryCount = 0
		}

		if observer.retryCount >= RetryCountLimit {
			message := NodeHeartbeatMessage{
				Timestamp: 0,
				Id:        observer.nodeMetadata.Id,
				Status:    false,
				LeaderId:  -1,
			}

			observer.notifier.Notify(message, DOWN)
			break
		}
	}
	logrus.Infoln("NodeObserver.heartbeat [", observer.nodeMetadata.Id, "] - stop")
}
