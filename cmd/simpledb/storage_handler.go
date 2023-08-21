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
	"io"
	"net/http"

	"github.com/ISSuh/simpledb/internal/api"
	"github.com/sirupsen/logrus"
)

func (node *SimpleDBNode) GetItem(w http.ResponseWriter, r *http.Request, args map[string]string) {
	key := args["key"]
	logrus.Infoln("SimpleDBNode.GetItem - key : ", key)

	value, err := node.engine.Get(key)
	if err != nil {
		logrus.Errorln("SimpleDBNode.GetItem - err : ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}
	api.HandleResponse(w, value, http.StatusOK)
}

func (node *SimpleDBNode) PutItem(w http.ResponseWriter, r *http.Request, args map[string]string) {
	key := args["key"]
	logrus.Infoln("SimpleDBNode.PutItem - key : ", key)

	defer r.Body.Close()
	value, err := io.ReadAll(r.Body)
	if err != nil {
		logrus.Errorln("SimpleDBNode.PutItem - err : ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	} else {
		logrus.Infoln("SimpleDBNode.PutItem - value : ", string(value))
	}

	if err := node.engine.Put(key, value); err != nil {
		logrus.Errorln("SimpleDBNode.PutItem - err : ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	}
	api.HandleResponse(w, nil, http.StatusOK)
}

func (node *SimpleDBNode) RemoveItem(w http.ResponseWriter, r *http.Request, args map[string]string) {
	key := args["key"]
	logrus.Infoln("SimpleDBNode.RemoveItem - key : ", key)

	if err := node.engine.Remove(key); err != nil {
		logrus.Errorln("SimpleDBNode.RemoveItem - err : ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
	}
	api.HandleResponse(w, nil, http.StatusOK)
}
