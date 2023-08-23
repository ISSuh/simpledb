# simpledb
## Overview
Try implement distribute key value storage for study.
Main storage use [LSM-tree based engine](https://github.com/ISSuh/lsm-tree) and [Raft implementation](https://github.com/ISSuh/raft) was used for distributed data management and processing.

Under development project.

## Design
![arch](./docs/arch.png)

## Install

```bash
$ git clone https://github.com/ISSuh/simpledb.git
$ cd simpledb
$ make
```

## Run

```bash
# run cluster
$ ./simpledb-cluster ./config/cluster.yaml

# run simpledb
$ ./simpledb ./config/db.yaml
```

## Config file
simpledb and simpledb-cluster needed config file. The config file is basically yaml fomat.

```yaml
# config/cluster.yaml
# simpledb-cluster config

# http server address
address: 0.0.0.0:33660

# node manager option
node_manager:

  # heartbeat request duration.
  # second.
  heartbeat_duration: 30

  # heartbeat request timeout.
  # millisecond
  heartbeat_request_timeout: 3000

```

```yaml
# config/db.yaml
# simpledb config

# HTTP server address
address: 127.0.0.1:33665

# simpledb standalone mode
# Set this option true if you don't need any functionality of distribution.
# if this option is true,
# do not connect cluster and not create Raft Node object
standalone: false

# raft node option
# this option is used when standalone option is false
node:
  # node id
  node_id: 0

  # RPC server address
  rpc_address: 127.0.0.1:33666

  # cluster address for connect
  cluster_address: 127.0.0.1:33660

# storage option
storage:
  # default directory path where placed files
  path: ../temp/id0

  # max level of files
  # start level is 0
  # the last level has unlimit number of files
  level: 7

  # max block size on table
  block_size: 4K

  # max L0 table size
  table_size: 2M

  # limited number of files on level
  # key is level, value is limited number
  # the last level has unlimit number of files
  limited_file_num_on_l0: 2

  # offset of calculated table size whne inscrease level
  # if tableSize value is 10Mb and tableSizeOffset value is 10,
  # the Max L0 file size is 10Mb and L1 file size is 100Mb(10Mb * 10)
  table_size_offset: 10

  # max level value on skiplist
  level_on_skiplist: 5

  # limited number of memtable
  memtable_size: 4M
```