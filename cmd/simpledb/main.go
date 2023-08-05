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
	"log"
	"net/http"

	"github.com/ISSuh/lsm-tree/logging"
	"github.com/ISSuh/lsm-tree/storage"
	"github.com/gorilla/mux"
)

type StorageHandler struct {
	storage *storage.Storage
}

func newHandler(option storage.Option) *StorageHandler {
	return &StorageHandler{
		storage: storage.NewStorage(option),
	}
}

func (handler StorageHandler) Get(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	key := params["key"]

	value := handler.storage.Get(key)
	w.WriteHeader(http.StatusOK)
	w.Write(value)
}

func (handler StorageHandler) Set(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	key := params["key"]

	value, err := io.ReadAll(r.Body)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		return
	}

	handler.storage.Set(key, value)
	w.WriteHeader(http.StatusOK)
	w.Write([]byte{})
}

func (handler StorageHandler) Remove(w http.ResponseWriter, r *http.Request) {
	params := mux.Vars(r)
	key := params["key"]

	handler.storage.Remove(key)
	w.WriteHeader(http.StatusOK)
	w.Write([]byte{})
}

func main() {
	logging.Info("storage server")
	storageOption := storage.NewOption()
	storageOption.Path = "./temp"
	storageHandler := newHandler(storageOption)

	router := mux.NewRouter()
	router.HandleFunc("/api/{key}", storageHandler.Get).Methods("GET")
	router.HandleFunc("/api/{key}", storageHandler.Set).Methods("POST")
	router.HandleFunc("/api/{key}", storageHandler.Remove).Methods("DELETE")

	http.Handle("/", router)
	err := http.ListenAndServe(":33660", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
