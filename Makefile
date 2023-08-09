# ====================================================================================
# Targets
# ====================================================================================

all:
	go build ./cmd/simpledb
	go build ./cmd/simpledb-cluster

test:
	go test -v ./...

coverage:
	go test ./... -coverprofile=coverage.out
	go tool cover -html=coverage.out

fmt:
	go fmt ./...

clean:
	rm -rf simpledb
	rm -rf simpledb-cluster
