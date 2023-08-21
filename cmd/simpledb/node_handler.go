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

func (node *SimpleDBNode) Heartbeat(w http.ResponseWriter, r *http.Request, args map[string]string) {
	logrus.Infoln("SimpleDBNode.Heartbeat")
	api.HandleResponse(w, nil, http.StatusOK)
}

func (node *SimpleDBNode) RegistNewPeer(w http.ResponseWriter, r *http.Request, args map[string]string) {
	id := args["id"]
	logrus.Infoln("SimpleDBNode.RegistedNewPeer - id : ", id)

	defer r.Body.Close()
	body, err := io.ReadAll(r.Body)
	if err != err {
		logrus.Errorln("SimpleDBNode.RegistNewPeer - err : ", err)
		api.HandleResponse(w, nil, http.StatusBadRequest)
		return
	} else {
		logrus.Infoln("SimpleDBNode.PutItem - value : ", string(body))
	}

	api.HandleResponse(w, nil, http.StatusOK)
}
