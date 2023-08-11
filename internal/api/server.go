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

package api

import (
	"net/http"

	"github.com/gorilla/mux"
)

type Server struct {
	router Router
}

func NewServer(router Router) *Server {
	return &Server{
		router: router,
	}
}

func (server *Server) Serve(address string) error {
	muxer := server.createMux()
	http.Handle("/", muxer)
	return http.ListenAndServe(address, nil)
}

func (server *Server) createMux() *mux.Router {
	muxer := mux.NewRouter()
	for _, route := range server.router.Route() {
		muxer.Path(route.Path()).Methods(route.Method()).Handler(server.bindHttpHandler(route.Handler()))
	}
	return muxer
}

func (server *Server) bindHttpHandler(apiHandler ApiHandler) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		args := mux.Vars(r)
		if args == nil {
			args = make(map[string]string)
		}
		apiHandler(w, r, args)
	}
}
